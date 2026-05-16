cmake_minimum_required(VERSION 3.26)

project(EverSightDesigner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_QML_GENERATE_QMLLS_INI ON)

find_package(Qt6 REQUIRED COMPONENTS Quick QuickControls2 QuickDialogs2 QuickLayouts)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appEverSightDesigner
    app/main.cpp
)

# Include per-folder qml modules (each subdir registers its own QML_FILES)
add_subdirectory(src/qml/editor/palette)
add_subdirectory(src/qml/editor/shell)
add_subdirectory(src/qml/editor/canvas)

set(EDITOR_QML_FILES
    ${SHELL_QML_FILES}
    ${PALETTE_QML_FILES}
    src/qml/editor/inspector/InspectorPanel.qml
    src/qml/editor/inspector/AppearanceSection.qml
    src/qml/editor/inspector/BaseSettingsSection.qml
    src/qml/editor/inspector/ColorPickerPopup.qml
    src/qml/editor/inspector/ColorSwatch.qml
    src/qml/editor/inspector/DataSourceSection.qml
    src/qml/editor/inspector/DynamicPropertySection.qml
    src/qml/editor/inspector/IdentitySection.qml
    src/qml/editor/inspector/InspectorDivider.qml
    src/qml/editor/inspector/InspectorSectionHeader.qml
    src/qml/editor/inspector/LayoutSplitStyleSection.qml
    src/qml/editor/inspector/LayoutSection.qml
    ${CANVAS_QML_FILES}
)

set(EDITOR_RESOURCE_FILES
    ${SHELL_ASSET_FILES}
    ${PALETTE_ASSET_FILES}
)

# Single QML module for the application: collect QML files from subdirs
qt_add_qml_module(appEverSightDesigner
    URI EverSightDesigner
    VERSION 1.0
    QML_FILES
        ${EDITOR_QML_FILES}
    RESOURCES
        ${EDITOR_RESOURCE_FILES}
)

# Add QML/resource files as explicit target sources so IDEs show them in the project tree.
target_sources(appEverSightDesigner PRIVATE
    ${EDITOR_QML_FILES}
    ${EDITOR_RESOURCE_FILES}
)

target_sources(appEverSightDesigner PRIVATE
    src/shared/models/WidgetItem.h
    src/shared/models/WidgetInstance.h
    src/shared/models/WidgetModel.h
    src/shared/models/ZoneModel.h
    src/shared/models/ScreenModel.h
    src/shared/enums/ZoneType.h
    src/shared/models/CanvasLayoutModel.h
    src/shared/models/CanvasTabModel.h
    src/shared/models/FixedBarState.h
    src/shared/enums/WidgetCategory.h
    src/editor/registry/WidgetDescriptor.h
    src/editor/registry/WidgetTypeDescriptor.h
    src/editor/registry/WidgetTypeRegistry.h
    src/editor/registry/WidgetTypeRegistry.cpp
    src/editor/registry/WidgetPaletteModel.h
    src/editor/registry/WidgetPaletteModel.cpp
    src/shared/models/ActionModel.h
    src/editor/shell/TopBarViewModel.h
    src/editor/shell/TopBarViewModel.cpp
    src/editor/canvas/CanvasViewModel.h
    src/editor/canvas/CanvasViewModel_Tabs.cpp
    src/editor/canvas/CanvasViewModel_Layout.cpp
    src/editor/canvas/CanvasViewModel_Widgets.cpp
    src/editor/canvas/CanvasViewModel_Inspector.cpp
    src/editor/canvas/CanvasViewModel_Persistence.cpp
    src/editor/canvas/CanvasViewModel_State.cpp
    src/editor/canvas/EditorController.h
    src/editor/canvas/EditorController.cpp
    src/editor/canvas/LayoutManager.h
    src/editor/canvas/LayoutManager.cpp
    src/editor/canvas/PreviewManager.h
    src/editor/canvas/PreviewManager.cpp
    src/editor/canvas/UndoStack.h
    src/editor/canvas/WidgetStore.h
    src/editor/canvas/WidgetEngine.h
    src/editor/canvas/WidgetEngine.cpp
    src/editor/canvas/FixedBarViewModel.h
    src/editor/canvas/FixedBarViewModel.cpp
)

set_target_properties(appEverSightDesigner PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appEverSightDesigner
    PRIVATE Qt6::Quick Qt6::QuickControls2 Qt6::QuickDialogs2 Qt6::QuickLayouts
)

install(TARGETS appEverSightDesigner
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
