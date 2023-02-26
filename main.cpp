#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QEvent>
#include <QQmlContext>
#include <QFileOpenEvent>
#include <QGuiApplication>
#include <QSurfaceFormat>
#include "src/texture/TextureView.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QSurfaceFormat fmt;
    fmt.setVersion( 3, 3 );
    fmt.setProfile( QSurfaceFormat::CoreProfile );
    QSurfaceFormat::setDefaultFormat( fmt );

    QQmlApplicationEngine engine;
    qmlRegisterType<TextureView>("filament", 1, 0, "TextureView");
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl) QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
    int result = app.exec();
    return result;
}
