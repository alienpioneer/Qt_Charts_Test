#include <QApplication>
#include <QQuickView>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <controller.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);

    UiController controller;
    const QUrl url(QStringLiteral("qrc:/main.qml"));

    auto connectionFunc = [url](QObject *obj, const QUrl &objUrl)
    {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    };

//    QQuickView viewer;
//    viewer.engine()->rootContext()->setContextProperty("controller", &controller);
//    QObject::connect(viewer.engine(), &QQmlEngine::quit, &viewer, &QWindow::close);
////    viewer.setTitle(QStringLiteral("Test"));
////    viewer.setColor(QColor("#404040"));
//    viewer.setSource(QUrl("qrc:/main.qml"));
////    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
//    viewer.show();

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("controller", &controller);


    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app, connectionFunc, Qt::QueuedConnection);

    engine.load(url);

    return app.exec();
}
