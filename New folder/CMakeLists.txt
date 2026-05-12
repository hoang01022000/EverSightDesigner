cmake_minimum_required(VERSION 3.16)

project(EverSightDesigner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appEverSightDesigner
    app/main.cpp
)

qt_add_qml_module(appEverSightDesigner
    URI EverSightDesigner

    QML_FILES
        src/qml/editor/shell/Main.qml
        src/qml/editor/shell/DesignerView.qml
        src/qml/editor/shell/TopBar.qml
        src/qml/editor/palette/PalettePanel.qml
        src/qml/editor/inspector/InspectorPanel.qml
        src/qml/editor/canvas/DesignCanvas.qml
        src/qml/editor/palette/PaletteItem.qml
        src/qml/editor/palette/PaletteSection.qml
        src/qml/editor/canvas/WidgetNode.qml

    SOURCES
        src/shared/models/WidgetItem.h
        src/editor/canvas/CanvasViewModel.h
        src/editor/canvas/CanvasViewModel.cpp
        SOURCES src/shared/models/WidgetItem.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appEverSightDesigner PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appEverSightDesigner
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appEverSightDesigner
    PRIVATE Qt6::Quick Qt6::QuickControls2
)

install(TARGETS appEverSightDesigner
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
