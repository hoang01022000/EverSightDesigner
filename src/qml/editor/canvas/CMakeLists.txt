## Canvas CMake - registers canvas QML module and includes widgets

add_subdirectory(widgets)

set(CANVAS_QML_FILES
    src/qml/editor/canvas/DesignCanvas.qml
    src/qml/editor/canvas/RuntimePreviewSurface.qml
    src/qml/editor/canvas/WorkspaceTopBar.qml
    src/qml/editor/canvas/WorkspaceStatusBar.qml
    src/qml/editor/canvas/WidgetNode.qml
    src/qml/editor/canvas/SplitterHandle.qml
    ${CANVAS_WIDGETS_QML_FILES}
    PARENT_SCOPE
)
