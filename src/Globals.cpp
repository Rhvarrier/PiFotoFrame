#include <Globals.hpp>
#include <QCoreApplication>
#include <QSettings>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <Weather.hpp>

ConfigFile* ConfigFile::_me = nullptr;
QString ConfigFile::FILE_PATH = QString::Null();
namespace
{
 bool auto_powder_down = true;
}

ConfigFile *ConfigFile::getInstance()
{
    if (!_me)
    {
        _me = new ConfigFile();
    }
    return _me;
}

ConfigFile::ConfigFile(QObject* parent)
    :QObject(parent)

{
    QFileSystemWatcher config_file_watcher(this);
    config_file_watcher.addPath(ConfigFile::FILE_PATH);
    connect(&config_file_watcher, &QFileSystemWatcher::fileChanged, [=](){emit fileModified();qDebug() << "config file modified";});
}

MonitorController* MonitorController::_me = nullptr;
MonitorController *MonitorController::getInstance()
{
    if (!_me)
    {
        _me = new MonitorController();
    }
    return _me;
}

MonitorController::MonitorController(QObject *parent)
    :QObject(parent),
      _onTimer(this),
      _offTimer(this)
{
    _onTimer.callOnTimeout(this, [this]{
        enableDisplay(true);
    });
    _offTimer.callOnTimeout(this, [this]{
        enableDisplay(false);
    });
    connect(ConfigFile::getInstance(), &ConfigFile::fileModified, [=](){
        initTimer();
    });
    initTimer();
}

void MonitorController::enableDisplay(bool enable)
{
    if (auto_powder_down)
    {
        QString cmd = "vcgencmd";
        QString args = (enable ? "display_power 1":" display_power 0");
        QProcess::execute(cmd, QStringList() << args);
        // Disables the Weather updates if the screen is off
        emit stateChanged(true);
    }
}

void MonitorController::initTimer()
{
    // init Settings
    QString settings_file_path = ConfigFile::FILE_PATH;
    QSettings settings(settings_file_path, QSettings::IniFormat);
    settings.sync();
    settings.beginGroup("MonitorControl");
    QString on_time_str = settings.value("MonitorControl.switch_on_time").toString();
    QString off_time_str = settings.value("MonitorControl.switch_off_time").toString();
    auto_powder_down = settings.value("MonitorControl.auto_power_down").toBool();
    settings.endGroup();

    //Initialize Timer
    QTime setTime = QTime::fromString(off_time_str, "hh:mm");
    qDebug() << setTime;
    QDateTime off_time = QDateTime::currentDateTime();
    if(off_time.time() > setTime){
        off_time = off_time.addDays(1);
    }
    off_time.setTime(setTime);
    int diff = QDateTime::currentDateTime().msecsTo(off_time);
    qDebug() << diff;
    QTimer::singleShot(diff, [this](){
       enableDisplay(false);
       _offTimer.start(86400000);
    });

    setTime = QTime::fromString(on_time_str, "hh:mm");
    qDebug() << setTime;
    QDateTime on_time = QDateTime::currentDateTime();
    if(on_time.time() > setTime){
        on_time = off_time.addDays(1);
    }
    on_time.setTime(setTime);
    diff = QDateTime::currentDateTime().msecsTo(on_time);
    qDebug() << diff;
    QTimer::singleShot(diff, [this](){
        enableDisplay(true);
       _onTimer.start(86400000);
    });

}
