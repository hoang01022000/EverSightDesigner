## Widgets CMake - exports list of widget QML files to parent (canvas)

set(CANVAS_WIDGETS_QML_FILES
    src/qml/editor/canvas/widgets/BaseWidget.qml
    src/qml/editor/canvas/widgets/Foundation/ButtonWidget.qml
    src/qml/editor/canvas/widgets/Foundation/ImageWidget.qml
    src/qml/editor/canvas/widgets/Foundation/TextboxWidget.qml
    src/qml/editor/canvas/widgets/Foundation/TrafficLightWidget.qml
    src/qml/editor/canvas/widgets/Foundation/RunControlWidget.qml
    src/qml/editor/canvas/widgets/Foundation/RunStatusWidget.qml
    src/qml/editor/canvas/widgets/Foundation/OKNGWidget.qml
    src/qml/editor/canvas/widgets/Chart/TableWidget.qml
    src/qml/editor/canvas/widgets/PlaceholderWidget.qml
    PARENT_SCOPE
)
