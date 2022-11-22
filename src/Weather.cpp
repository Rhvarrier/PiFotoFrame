#include <Weather.hpp>
#include <Globals.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <QDebug>

#include <chrono>
#include <QTimer>
#include <QCoreApplication>
#include <QSettings>
#include <QFileInfo>

namespace
{
    constexpr float ZERO_KELVIN = 273.15;
    QString weather_url;
    QString api_key;
    bool enable_weather = true;
    // Query current location using IP address. This will only be done once.
    QString location_url;
    // Latitude and Longitude
    QString lat = "", lon = "", City = "";

    std::chrono::milliseconds weather_update_interval_in_ms;

    bool standby_weather = false;
    void onStandby(bool enable)
    {
        standby_weather = enable;
    }

    QString niceTemperatureString(double t)
    {
        return QString::number(qRound(t-ZERO_KELVIN)) + QChar(0xB0);
    }

    [[maybe_unused]] void debugWeatherData(WeatherData& data)
    {
        qDebug() << "Temperature: " << data._temperature;
        qDebug() << "Min Temperature: " << data._minTemperature;
        qDebug() << "Max Temperature: " << data._maxTemperature;
        qDebug() << "Condition: " << data._condition;
    }

}

Weather* Weather::_me = nullptr;
Weather::Weather()
    : _network_acess_man(new QNetworkAccessManager(this)),
     _today(),
     _tomorrow(),
     _timer(this)
{
    initProperties();
    initLocation();
    initTimer();
    connect(&ConfigFile::GetInstance(), &ConfigFile::fileModified, [=]()
    {
        initProperties();
        initLocation();
        initTimer();
    });
    connect(&MonitorController::GetInstance(), &MonitorController::stateChanged, [=](bool enable)
    {
        onStandby(!enable);
    });
}

void Weather::initProperties()
{
        QString settings_file_path = ConfigFile::FILE_PATH;
        QSettings settings(settings_file_path, QSettings::IniFormat);
        settings.sync();

        settings.beginGroup("Weather");
        location_url = settings.value("Weather.location.url").toString();
        weather_url = settings.value("Weather.provider.url").toString();
        api_key = settings.value("Weather.provider.api_key").toString();
        int update_period_in_min = settings.value("Weather.update.period_in_min", 1).toInt();
        weather_update_interval_in_ms = std::chrono::minutes(update_period_in_min);
        enable_weather = settings.value("Weather.enable").toBool();
        settings.endGroup();

 }

bool Weather::updateData(QString &data_to_update, const QString& data_from_source) const
{
    if (data_to_update != data_from_source)
    {
        data_to_update = data_from_source;
        return true;
    }
    return false;
}

void Weather::initTimer()
{
    _timer.setTimerType(Qt::TimerType::VeryCoarseTimer);
    _timer.setSingleShot(false);
    _timer.setInterval(weather_update_interval_in_ms);
    _timer.callOnTimeout(this, [this](){updateWeather();});
    _timer.start();
}

Weather* Weather::getInstance()
{
    if (!_me)
    {
        _me = new Weather();
    }
    return _me;
}

void Weather::initLocation()
{
    QUrl location_query_url(location_url);
    QNetworkReply *rep = _network_acess_man->get(QNetworkRequest(location_query_url));
    connect(rep, &QNetworkReply::finished,
            this, [this, rep]() { handleLocationNetworkData(rep); });
}

void Weather::handleLocationNetworkData(QNetworkReply *networkReply)
{
    if (!networkReply)
    {
        qDebug() << "handleLocationNetworkData reply nul";
        return;
    }
    if(!networkReply->error())
    {
        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());

        if (document.isObject())
        {
            QJsonObject obj = document.object();
            if (obj.contains(QStringLiteral("lat"))) {
                lat = QString::number(obj.value(QStringLiteral("lat")).toDouble());
            }
            if (obj.contains(QStringLiteral("lon"))) {
                lon = QString::number(obj.value(QStringLiteral("lon")).toDouble());
            }
            if (obj.contains(QStringLiteral("city"))) {
                City = obj.value(QStringLiteral("city")).toString();
            }
            qDebug() << City;
            emit cityUpdated();
            updateWeather();
        }
    }
    else
    {
        qDebug() << "Network Reply error: " << networkReply->errorString();
    }
    networkReply->deleteLater();
}


QString Weather::city() const
{
    return City;
}

void Weather::updateWeather()
{
    if (enable_weather && !standby_weather && (lat!="") && (lon!=""))
    {
        QUrl current_weather_url(weather_url);
        QUrlQuery query;
        query.addQueryItem("lat", lat);
        query.addQueryItem("lon", lon);
        query.addQueryItem("mode", "json");
        query.addQueryItem("APPID", api_key);
        current_weather_url.setQuery(query);
        //qDebug() << "url = " << current_weather_url;
        QNetworkReply *rep = _network_acess_man->get(QNetworkRequest(current_weather_url));
        connect(rep, &QNetworkReply::finished,
                    this, [this, rep]() { handleWeatherNetworkData(rep); });
    }
}


void Weather::handleWeatherNetworkData(QNetworkReply *networkReply)
{
    qDebug() << "update weather";

    if (!networkReply)
        return;

    if(!networkReply->error())
    {
        QJsonDocument document = QJsonDocument::fromJson(networkReply->readAll());
        if (document.isObject()) {
            QJsonObject obj = document.object();
            //qDebug() << obj;
            parseTodayWeather(obj);
            parseTomorrowWeather(obj);
        }
    }
    else
    {
        qDebug() << "Network Reply error: " << networkReply->errorString();
    }
    networkReply->deleteLater();
}

void Weather::parseTodayWeather(QJsonObject obj)
{
    bool data_updated = false;
    QJsonValue root, temp;
    if (obj.contains(QStringLiteral("current"))) {
        root = obj.value(QStringLiteral("current"));
        temp =  root.toObject().value(QStringLiteral("weather")).toArray()[0].toObject().value("description");
        data_updated |= updateData(_today._condition , temp.toString());
        if (data_updated)
        {
            updateIconFromCondition(false);
        }
        temp = root.toObject().value(QStringLiteral("temp"));
        data_updated |= updateData(_today._temperature , niceTemperatureString(temp.toDouble()));
        temp =  root.toObject().value(QStringLiteral("weather")).toArray()[0].toObject().value("description");
        data_updated |= updateData(_today._condition , temp.toString());
    }
    if (obj.contains(QStringLiteral("daily"))) {
        root = obj.value(QStringLiteral("daily")).toArray()[0];
        temp = root.toObject().value(QStringLiteral("temp")).toObject().value("min");
        data_updated |= updateData(_today._minTemperature , niceTemperatureString(temp.toDouble()));
        temp = root.toObject().value(QStringLiteral("temp")).toObject().value("max");
        data_updated |= updateData(_today._maxTemperature , niceTemperatureString(temp.toDouble()));
    }

    if (data_updated)
    {
      emit todayWeatherUpdated();
      //debugWeatherData(_today);
      //qDebug() << city();
    }

}

void Weather::parseTomorrowWeather(QJsonObject obj)
{
    bool data_updated = false;
    QJsonValue root, temp;
    if (obj.contains(QStringLiteral("daily"))) {
        root = obj.value(QStringLiteral("daily")).toArray()[1];
        temp =  root.toObject().value(QStringLiteral("weather")).toArray()[0].toObject().value("description");
        data_updated |= updateData(_tomorrow._condition , temp.toString());
        if (data_updated)
        {
            updateIconFromCondition(true);
        }
        temp = root.toObject().value(QStringLiteral("temp")).toObject().value("min");
        data_updated |= updateData(_tomorrow._minTemperature , niceTemperatureString(temp.toDouble()));
        temp = root.toObject().value(QStringLiteral("temp")).toObject().value("max");
        data_updated |= updateData(_tomorrow._maxTemperature , niceTemperatureString(temp.toDouble()));
    }

    if (data_updated)
        emit tomorrowWeatherUpdated();

}

void Weather::updateIconFromCondition(bool forecast)
{
   QString& condition = forecast ? _tomorrow._condition:_today._condition;
   QString icon_url;
   if (condition.contains("thunderstorm", Qt::CaseInsensitive))
   {
       icon_url = "Thunderstorm.png";
   }
   else if (condition.contains("drizzle", Qt::CaseInsensitive))
   {
       icon_url = "Shower.png";
   }
   else if (condition.contains("freezing rain", Qt::CaseInsensitive))
   {
       icon_url = "Snow.png";
   }
   else if (condition.contains("rain", Qt::CaseInsensitive))
   {
       icon_url = "Rain.png";
   }
   else if (condition.contains("snow", Qt::CaseInsensitive) || condition.contains("sleet", Qt::CaseInsensitive))
   {
       icon_url = "Snow.png";
   }
   else if (condition.contains("clear", Qt::CaseInsensitive))
   {
       icon_url = "Clear.png";
   }
   else if (condition.contains("few clouds", Qt::CaseInsensitive))
   {
       icon_url = "Sunny_Cloudy.png";
   }
   else
   {
       icon_url = "Cloudy.png";
   }

   if(forecast)
   {
       emit tomorrowWeatherIconUpdated(icon_url);
   }
   else
   {
       emit todayWeatherIconUpdated(icon_url);
   }
}
