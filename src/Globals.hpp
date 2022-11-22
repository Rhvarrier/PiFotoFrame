#pragma once
#include <QFileSystemWatcher>
#include <QTimer>

class ConfigFile: public QObject
{
    Q_OBJECT
public:
    static ConfigFile& GetInstance();
    static QString FILE_PATH;

signals:
   void fileModified();
private:
   explicit ConfigFile(QObject *parent = nullptr);
   ~ConfigFile() = default;
   ConfigFile(const ConfigFile&)= delete;
   ConfigFile& operator=(const ConfigFile&)= delete;

   QFileSystemWatcher m_oConfigFileWatcher;

};

class MonitorController : public QObject
{
    Q_OBJECT
public:
   static MonitorController& GetInstance();

signals:
   void stateChanged(bool enable);

private:
    explicit MonitorController(QObject *parent = nullptr);
    ~MonitorController() = default;
    MonitorController(const MonitorController&)= delete;
    MonitorController& operator=(const MonitorController&)= delete;

    QTimer m_onTimer;
    QTimer m_offTimer;
    void EnableDisplay(bool enable);
    void InitTimer();
};
