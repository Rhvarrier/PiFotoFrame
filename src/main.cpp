#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDebug>

#include <Photo.hpp>
#include <Weather.hpp>
#include <QDir>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <Globals.hpp>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QCoreApplication::setApplicationName("PiFotoFrame");
    ConfigFile::FILE_PATH = QCoreApplication::applicationFilePath() + ".conf";

    Weather* weather = Weather::getInstance();
    Photo* photo = Photo::getInstance();

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("Weather", weather);
    engine.rootContext()->setContextProperty("Photo", photo);
    engine.load(url);
    return app.exec();
}
