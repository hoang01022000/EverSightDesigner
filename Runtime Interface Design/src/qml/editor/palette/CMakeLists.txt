## Palette CMake - exports QML and asset file lists to parent

## Palette CMake - exports QML and asset file lists to parent

set(PALETTE_QML_FILES
    src/qml/editor/palette/PalettePanel.qml
    src/qml/editor/palette/PaletteSection.qml
    src/qml/editor/palette/PaletteItem.qml
    PARENT_SCOPE
)

set(PALETTE_ASSET_FILES
    "src/qml/editor/palette/assets/icons/Image.png"
    "src/qml/editor/palette/assets/icons/Multiple Images.png"
    "src/qml/editor/palette/assets/icons/Run Control.png"
    "src/qml/editor/palette/assets/icons/Run Status.png"
    "src/qml/editor/palette/assets/icons/OK&NG.png"
    "src/qml/editor/palette/assets/icons/Textbox.png"
    "src/qml/editor/palette/assets/icons/Button.png"
    "src/qml/editor/palette/assets/icons/Switch Control.png"
    "src/qml/editor/palette/assets/icons/Params Settings.png"
    "src/qml/editor/palette/assets/icons/Variable Settings.png"
    "src/qml/editor/palette/assets/icons/Character Settings.png"
    "src/qml/editor/palette/assets/icons/Trafic Light.png"
    "src/qml/editor/palette/assets/icons/Conditional Light.png"
    "src/qml/editor/palette/assets/icons/Group Box.png"
    "src/qml/editor/palette/assets/icons/Child Interface.png"
    "src/qml/editor/palette/assets/icons/Tab Control.png"
    "src/qml/editor/palette/assets/icons/Table.png"
    "src/qml/editor/palette/assets/icons/Line Diagram Array.png"
    "src/qml/editor/palette/assets/icons/Multi Line Diagram.png"
    "src/qml/editor/palette/assets/icons/Production Statistics.png"
    "src/qml/editor/palette/assets/icons/Pie Control.png"
    "src/qml/editor/palette/assets/icons/Param Settings Array.png"
    "src/qml/editor/palette/assets/icons/Label Array.png"
    PARENT_SCOPE
)
