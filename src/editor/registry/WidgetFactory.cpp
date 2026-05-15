#include "WidgetFactory.h"
#include "PropertyDefinition.h"

namespace eversight {

WidgetDescriptor WidgetFactory::createDescriptor(const QString& typeId,
                                                 const QString& displayName,
                                                 const QString& iconText,
                                                 WidgetCategory category,
                                                 qreal defaultWidth,
                                                 qreal defaultHeight,
                                                 const QString& backgroundColor,
                                                 const QString& borderColor,
                                                 const QString& fontColor,
                                                 const QVariantMap& defaultProperties)
{
    WidgetDescriptor descriptor;
    descriptor.typeId                 = typeId;
    descriptor.displayName            = displayName;
    descriptor.iconText               = iconText;
    descriptor.category               = category;
    descriptor.defaultWidth           = defaultWidth;
    descriptor.defaultHeight          = defaultHeight;
    descriptor.defaultBackgroundColor = backgroundColor;
    descriptor.defaultBorderColor     = borderColor;
    descriptor.defaultFontColor       = fontColor;
    descriptor.defaultProperties      = defaultProperties;
    completeDescriptor(descriptor);
    return descriptor;
}

void WidgetFactory::completeDescriptor(WidgetDescriptor& descriptor)
{
    const QString& typeId = descriptor.typeId;
    const QString iconRoot = QStringLiteral("qrc:/qt/qml/EverSightDesigner/src/qml/editor/palette/assets/icons/");

    if (typeId == "Image") descriptor.iconSource = iconRoot + "Image.png";
    else if (typeId == "MultipleImages") descriptor.iconSource = iconRoot + "Multiple Images.png";
    else if (typeId == "StaticImage") descriptor.iconSource = iconRoot + "Image.png";
    else if (typeId == "RunControl") descriptor.iconSource = iconRoot + "Run Control.png";
    else if (typeId == "RunStatus") descriptor.iconSource = iconRoot + "Run Status.png";
    else if (typeId == "OKNG") descriptor.iconSource = iconRoot + "OK&NG.png";
    else if (typeId == "Textbox") descriptor.iconSource = iconRoot + "Textbox.png";
    else if (typeId == "Button") descriptor.iconSource = iconRoot + "Button.png";
    else if (typeId == "SwitchControl") descriptor.iconSource = iconRoot + "Switch Control.png";
    else if (typeId == "ParamsSettings") descriptor.iconSource = iconRoot + "Params Settings.png";
    else if (typeId == "VariableSettings") descriptor.iconSource = iconRoot + "Variable Settings.png";
    else if (typeId == "CharacterSettings") descriptor.iconSource = iconRoot + "Character Settings.png";
    else if (typeId == "TrafficLight") descriptor.iconSource = iconRoot + "Trafic Light.png";
    else if (typeId == "ConditionalLight") descriptor.iconSource = iconRoot + "Conditional Light.png";
    else if (typeId == "GroupBox") descriptor.iconSource = iconRoot + "Group Box.png";
    else if (typeId == "ChildInterface") descriptor.iconSource = iconRoot + "Child Interface.png";
    else if (typeId == "TabControl") descriptor.iconSource = iconRoot + "Tab Control.png";
    else if (typeId == "Table") descriptor.iconSource = iconRoot + "Table.png";
    else if (typeId == "LineDiagramArray") descriptor.iconSource = iconRoot + "Line Diagram Array.png";
    else if (typeId == "MultiLineDiagram") descriptor.iconSource = iconRoot + "Multi Line Diagram.png";
    else if (typeId == "ProductionStatistics") descriptor.iconSource = iconRoot + "Production Statistics.png";
    else if (typeId == "PieControl") descriptor.iconSource = iconRoot + "Pie Control.png";
    else if (typeId == "ParamSettingsArray") descriptor.iconSource = iconRoot + "Param Settings Array.png";
    else if (typeId == "LabelArray") descriptor.iconSource = iconRoot + "Label Array.png";
    else descriptor.iconSource = iconRoot + "Image.png";

    descriptor.componentSource = "widgets/PlaceholderWidget.qml";
    if (typeId == "Image" || typeId == "MultipleImages")
        descriptor.componentSource = "widgets/Foundation/ImageWidget.qml";
    else if (typeId == "RunControl")
        descriptor.componentSource = "widgets/Foundation/RunControlWidget.qml";
    else if (typeId == "RunStatus")
        descriptor.componentSource = "widgets/Foundation/RunStatusWidget.qml";
    else if (typeId == "OKNG")
        descriptor.componentSource = "widgets/Foundation/OKNGWidget.qml";
    else if (typeId == "Textbox")
        descriptor.componentSource = "widgets/Foundation/TextboxWidget.qml";
    else if (typeId == "Button" || typeId == "SwitchControl")
        descriptor.componentSource = "widgets/Foundation/ButtonWidget.qml";
    else if (typeId == "TrafficLight")
        descriptor.componentSource = "widgets/Foundation/TrafficLightWidget.qml";
    else if (typeId == "Table")
        descriptor.componentSource = "widgets/Chart/TableWidget.qml";

    descriptor.propertyDefinitions = PropertyDefinition::forWidget(typeId);
}

} // namespace eversight
