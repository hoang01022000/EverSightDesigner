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

    if (typeId == "Image") descriptor.iconSource = "assets/icons/Image.png";
    else if (typeId == "MultipleImages") descriptor.iconSource = "assets/icons/Multiple Images.png";
    else if (typeId == "StaticImage") descriptor.iconSource = "assets/icons/Image.png";
    else if (typeId == "RunControl") descriptor.iconSource = "assets/icons/Run Control.png";
    else if (typeId == "RunStatus") descriptor.iconSource = "assets/icons/Run Status.png";
    else if (typeId == "OKNG") descriptor.iconSource = "assets/icons/OK&NG.png";
    else if (typeId == "Textbox") descriptor.iconSource = "assets/icons/Textbox.png";
    else if (typeId == "Button") descriptor.iconSource = "assets/icons/Button.png";
    else if (typeId == "SwitchControl") descriptor.iconSource = "assets/icons/Button.png";
    else if (typeId == "ParamsSettings") descriptor.iconSource = "assets/icons/Params Settings.png";
    else if (typeId == "VariableSettings") descriptor.iconSource = "assets/icons/Variable Settings.png";
    else if (typeId == "CharacterSettings") descriptor.iconSource = "assets/icons/Character Settings.png";
    else if (typeId == "TrafficLight") descriptor.iconSource = "assets/icons/Trafic Light.png";
    else if (typeId == "ConditionalLight") descriptor.iconSource = "assets/icons/Conditional Light.png";
    else if (typeId == "GroupBox") descriptor.iconSource = "assets/icons/Group Box.png";
    else if (typeId == "ChildInterface") descriptor.iconSource = "assets/icons/Child Interface.png";
    else if (typeId == "TabControl") descriptor.iconSource = "assets/icons/Tab Control.png";
    else if (typeId == "Table") descriptor.iconSource = "assets/icons/Table.png";
    else if (typeId == "LineDiagramArray") descriptor.iconSource = "assets/icons/Line Diagram Array.png";
    else if (typeId == "MultiLineDiagram") descriptor.iconSource = "assets/icons/Multi Line Diagram.png";
    else if (typeId == "ProductionStatistics") descriptor.iconSource = "assets/icons/Production Statistics.png";
    else if (typeId == "PieControl") descriptor.iconSource = "assets/icons/Pie Control.png";
    else if (typeId == "ParamSettingsArray") descriptor.iconSource = "assets/icons/Param Settings Array.png";
    else if (typeId == "LabelArray") descriptor.iconSource = "assets/icons/Label Array.png";
    else descriptor.iconSource = "assets/icons/Image.png";

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
