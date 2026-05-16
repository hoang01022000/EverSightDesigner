## Shell CMake - exports QML (TopBar/Main/DesignerView) and any shell assets to parent

## Shell CMake - exports QML (TopBar/Main/DesignerView) and any shell assets to parent

set(SHELL_QML_FILES
    src/qml/editor/shell/Main.qml
    src/qml/editor/shell/DesignerView.qml
    src/qml/editor/shell/TopBar.qml
    PARENT_SCOPE
)

set(SHELL_ASSET_FILES
    src/qml/editor/shell/assets/icons/Open.png
    src/qml/editor/shell/assets/icons/Save.png
    src/qml/editor/shell/assets/icons/Undo.png
    src/qml/editor/shell/assets/icons/Redo.png
    src/qml/editor/shell/assets/icons/Remove.png
    src/qml/editor/shell/assets/icons/toolbarFronted_normal.png
    src/qml/editor/shell/assets/icons/toolbarFronted_disable.png
    src/qml/editor/shell/assets/icons/toolbarStart_normal.png
    src/qml/editor/shell/assets/icons/toolbarStart_disable.png
    src/qml/editor/shell/assets/icons/toolBarStop_normal.png
    src/qml/editor/shell/assets/icons/toolbarStop_disable.png
    PARENT_SCOPE
)
