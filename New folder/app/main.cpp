#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "../src/editor/canvas/CanvasViewModel.h"
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Basic");

    CanvasViewModel canvasViewModel;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(
        "canvasViewModel",
        &canvasViewModel
        );

    engine.loadFromModule(
        "EverSightDesigner",
        "Main"
        );

    return QCoreApplication::exec();
}
