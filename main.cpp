#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QEvent>
#include <QQmlContext>
#include <QFileOpenEvent>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include "src/FilamentWindow.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QSurfaceFormat fmt;
    fmt.setVersion( 3, 3 );
    fmt.setProfile( QSurfaceFormat::CoreProfile );
    QSurfaceFormat::setDefaultFormat( fmt );

    QQmlApplicationEngine engine;
    qmlRegisterType<FilamentWindow>("filamentWindow", 1, 0, "FilamentWindow");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    int result = app.exec();
    return result;
}
