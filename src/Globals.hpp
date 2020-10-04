#pragma once
#include <QFileSystemWatcher>
#include <QTimer>

class ConfigFile: public QObject
{
    Q_OBJECT
public:
    static ConfigFile* getInstance();
    static QString FILE_PATH;


signals:
   void fileModified();
private:
   explicit ConfigFile(QObject *parent = nullptr);
   ~ConfigFile() = default;
   ConfigFile(const ConfigFile&)= delete;
   ConfigFile& operator=(const ConfigFile&)= delete;
   static ConfigFile* _me;

};

class MonitorController : public QObject
{
    Q_OBJECT
public:
   static MonitorController* getInstance();

signals:
   void stateChanged(bool enable);

private:
    explicit MonitorController(QObject *parent = nullptr);
    ~MonitorController() = default;
    MonitorController(const MonitorController&)= delete;
    MonitorController& operator=(const MonitorController&)= delete;
    static MonitorController* _me;
    QTimer _onTimer;
    QTimer _offTimer;
    void enableDisplay(bool enable);
    void initTimer();
};
