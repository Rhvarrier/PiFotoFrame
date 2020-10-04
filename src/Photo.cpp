#include <Photo.hpp>
#include <Globals.hpp>
#include <QDir>
#include <QDebug>
#include <QRandomGenerator>
#include <QImageReader>

#include <QSettings>

namespace
{
    QString root_path;
    QStringList dir_names = QStringList();
    std::chrono::milliseconds photo_update_period_in_ms(std::chrono::minutes(2));
}
Photo* Photo::_me = nullptr;

Photo *Photo::getInstance()
{
    if (!_me)
    {
        _me = new Photo();
    }
    return _me;
}

Photo::Photo(QObject *parent) :
    QObject(parent),
    _photo_file_paths(),
    _photo_update_timer(),
    _already_dispalyed_photos(),
    _rand_tries(0)
{
    initProperties();
    initDir();
    initTimer();
    update();
    connect(ConfigFile::getInstance(), &ConfigFile::fileModified, [=](){
        initProperties();
        initDir();
        initTimer();
    });
}

void Photo::initProperties()
{
    QString settings_file_path = ConfigFile::FILE_PATH;
    QSettings settings(settings_file_path, QSettings::IniFormat);
    settings.sync();

    settings.beginGroup("Photo");
    root_path = settings.value("Photo.location.root_path").toString();
    dir_names = settings.value("Photo.location.dir_names").toStringList();
    int photo_update_period_in_sec = settings.value("Photo.update.period_in_sec", 60).toInt();
    photo_update_period_in_ms = std::chrono::seconds(photo_update_period_in_sec);
    settings.endGroup();
}

void Photo::initTimer()
{
    _photo_update_timer.setInterval(photo_update_period_in_ms);
    _photo_update_timer.setTimerType(Qt::TimerType::VeryCoarseTimer);
    _photo_update_timer.callOnTimeout(this, [this](){update();});
    _photo_update_timer.setSingleShot(false);
    _photo_update_timer.start();
}

void Photo::initDir()
{
    QStringList filters;
    foreach (QByteArray format, QImageReader::supportedImageFormats())
        filters += "*." + format;
    QStringList temp_list;
    foreach(QString dir,dir_names)
    {
        QDir photo_dir(root_path + "/" + dir);
        if (!photo_dir.exists())
        {
            qDebug() << photo_dir << " does not exist";
            return;
        }
         temp_list = photo_dir.entryList(filters);
         foreach (QString file_name, temp_list)
             _photo_file_paths << photo_dir.path() + "/" + file_name;

    }
}

void Photo::update()
{
    int rand_index = 0;
    rand_index = QRandomGenerator::global()->bounded(0, _photo_file_paths.size());
    QString path = _photo_file_paths[rand_index];

    if (QFileInfo::exists(path) && QFileInfo(path).isFile())
    {
        emit updateImage(path);
    }
    else
    {
        emit updateImage("");
        qDebug() << path << " not found";
    }

}
