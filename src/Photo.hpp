#pragma once

#include <QObject>
#include <QTimer>
#include <QStringList>
#include <QVector>


class Photo : public QObject
{
    Q_OBJECT
    public:
       static Photo *getInstance();

    signals:
       void updateImage(QString img_path);

private:
    explicit Photo(QObject *parent = nullptr);
    void initProperties();
    void initTimer();
    void initDir();
    void update();
    QStringList _photo_file_paths;
    QTimer _photo_update_timer;
    QVector<int> _already_dispalyed_photos;
    int _rand_tries;
    static Photo* _me;

};

