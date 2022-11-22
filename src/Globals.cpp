#include <Globals.hpp>
#include <QCoreApplication>
#include <QSettings>
#include <QDateTime>
#include <QDebug>
#include <QProcess>
#include <Weather.hpp>


QString ConfigFile::FILE_PATH = QString::Null();
namespace
{
 bool auto_powder_down = true;
}

ConfigFile& ConfigFile::GetInstance()
{
    static ConfigFile Instance;
    return Instance;
}

ConfigFile::ConfigFile(QObject* parent)
    :QObject(parent),
      m_oConfigFileWatcher(this)

{
    //qDebug() << "Add config file " << ConfigFile::FILE_PATH;
    m_oConfigFileWatcher.addPath(ConfigFile::FILE_PATH);
    connect(&m_oConfigFileWatcher, &QFileSystemWatcher::fileChanged, this, &ConfigFile::fileModified);
}


MonitorController& MonitorController::GetInstance()
{
    static MonitorController Instance;
    return Instance;
}

MonitorController::MonitorController(QObject *parent)
    :QObject(parent),
      m_onTimer(this),
      m_offTimer(this)
{
    m_onTimer.callOnTimeout(this, [this]{
        EnableDisplay(true);
    });
    m_offTimer.callOnTimeout(this, [this]{
        EnableDisplay(false);
    });
    connect(&ConfigFile::GetInstance(), &ConfigFile::fileModified, this, [=](){
        InitTimer();
    });
    InitTimer();
}

void MonitorController::EnableDisplay(bool enable)
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

void MonitorController::InitTimer()
{
    // init Settings
    QString settings_file_path = ConfigFile::FILE_PATH;
    QSettings settings(settings_file_path, QSettings::IniFormat);
    settings.sync();
    settings.beginGroup("MonitorControl");
    QString on_time_str = settings.value("MonitorControl.switch_on_time").toString();
    qDebug() << "MonitorControl.switch_on_time " << on_time_str;
    QString off_time_str = settings.value("MonitorControl.switch_off_time").toString();
    qDebug() << "MonitorControl.switch_off_time " << off_time_str;
    auto_powder_down = settings.value("MonitorControl.auto_power_down").toBool();
    qDebug() << "MonitorControl.auto_power_down " << auto_powder_down;
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
       EnableDisplay(false);
       m_offTimer.start(86400000);
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
        EnableDisplay(true);
       m_onTimer.start(86400000);
    });

}
