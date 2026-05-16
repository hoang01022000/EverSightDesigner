#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "../src/editor/canvas/CanvasViewModel.h"
#include "../src/editor/canvas/FixedBarViewModel.h"
#include "../src/editor/shell/TopBarViewModel.h"
#include "../src/editor/registry/WidgetPaletteModel.h"
#include <QQmlContext>
#include <QQuickStyle>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQuickStyle::setStyle("Basic");

    CanvasViewModel canvasViewModel;
    eversight::FixedBarViewModel fixedBarViewModel;
    TopBarViewModel topBarViewModel;
    eversight::WidgetPaletteModel paletteModel;
    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(
        "canvasViewModel",
        &canvasViewModel
        );

    engine.rootContext()->setContextProperty(
        "fixedBarViewModel",
        &fixedBarViewModel
        );

    engine.rootContext()->setContextProperty(
        "topBarViewModel",
        &topBarViewModel
        );

    engine.rootContext()->setContextProperty(
        "paletteModel",
        &paletteModel
    );

    engine.loadFromModule(
        "EverSightDesigner",
        "Main"
        );

    return QCoreApplication::exec();
}
