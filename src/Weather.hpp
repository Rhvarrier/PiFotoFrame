#pragma once
/**
 * @brief The WeatherData is a singleton class used to fetch weather data
 */
#include <QtNetwork/QNetworkReply>
#include <QTimer>
#include <QQmlEngine>
#include <QJSEngine>

struct WeatherData
{
    QString _temperature;
    QString _maxTemperature;
    QString _minTemperature;
    QString _humidity;
    QString _condition;

    WeatherData()
        :_temperature(),
          _maxTemperature(),
          _minTemperature(),
          _humidity(),
          _condition()
    {}
};

class Weather: public QObject
{
    Q_OBJECT
        Q_PROPERTY(QString todayTemp
                      READ todayTemp
                      NOTIFY todayWeatherUpdated)
        Q_PROPERTY(QString todayHumidity
                  READ todayHumidity
                  NOTIFY todayWeatherUpdated)
        Q_PROPERTY(QString todayMaxTemp
                  READ todayMaxTemp
                  NOTIFY todayWeatherUpdated)
        Q_PROPERTY(QString todayMinTemp
                  READ todayMinTemp
                  NOTIFY todayWeatherUpdated)
        Q_PROPERTY(QString todayDescription
              READ todayDescription
              NOTIFY todayWeatherUpdated)
        Q_PROPERTY(QString tomorrowMaxTemp
                  READ tomorrowMaxTemp
                  NOTIFY tomorrowWeatherUpdated)
        Q_PROPERTY(QString tomorrowMinTemp
                  READ tomorrowMinTemp
                  NOTIFY tomorrowWeatherUpdated)
        Q_PROPERTY(QString tomorrowDescription
              READ tomorrowDescription
              NOTIFY tomorrowWeatherUpdated)

        Q_PROPERTY(QString city
                  READ city
                  NOTIFY cityUpdated)


    public:
        static Weather *getInstance();
        void updateWeather();

        QString todayTemp() const
        {
            return _today._temperature;
        }
        QString todayHumidity() const
        {
            return _today._humidity;
        }
        QString todayMaxTemp() const
        {
            return _today._maxTemperature;
        }

        QString todayMinTemp() const
        {
            return _today._minTemperature;
        }

        QString todayDescription() const
        {
            return _today._condition;
        }

        QString tomorrowMaxTemp() const
        {
            return _tomorrow._maxTemperature;
        }

        QString tomorrowMinTemp() const
        {
            return _today._minTemperature;
        }

        QString tomorrowDescription() const
        {
            return _tomorrow._condition;
        }

        QString city() const;

    signals:
        void todayWeatherUpdated();
        void tomorrowWeatherUpdated();
        void cityUpdated();

        void todayWeatherIconUpdated(QString icon_url);
        void tomorrowWeatherIconUpdated(QString icon_url);


    private:
        Weather();
        ~Weather() = default;
        Weather(const Weather&)= delete;
        Weather& operator=(const Weather&)= delete;

        // Helper function: returns true if both element value are different in which case only the value is changed
        bool updateData(QString& data_to_update, const QString& data_from_source) const;
        void initTimer();
        void initLocation();
        void handleLocationNetworkData(QNetworkReply *networkReply);

        void handleWeatherNetworkData(QNetworkReply *networkReply);
        // Helper functions to parse Weather data
        void parseTodayWeather(QJsonObject obj);
        void parseTomorrowWeather(QJsonObject obj);

        void updateIconFromCondition(bool forecast);
        void initProperties();

        QNetworkAccessManager* _network_acess_man;

        // Weather data elements
        WeatherData _today;
        WeatherData _tomorrow;
        QTimer _timer;
        static Weather *_me;
};
