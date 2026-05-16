#include "WidgetTypeRegistry.h"

#include <initializer_list>

namespace eversight {
namespace {

QVariantMap propertyDefinition(const QString& key,
                               const QString& label,
                               const QString& editor,
                               const QVariantList& options = {})
{
    QVariantMap definition;
    definition.insert(QStringLiteral("key"), key);
    definition.insert(QStringLiteral("label"), label);
    definition.insert(QStringLiteral("editor"), editor);
    if (!options.isEmpty())
        definition.insert(QStringLiteral("options"), options);
    return definition;
}

QVariantMap appearanceField(const QString& key, const QString& label)
{
    QVariantMap field;
    field.insert(QStringLiteral("key"), key);
    field.insert(QStringLiteral("label"), label);
    return field;
}

QVariantList appearance(std::initializer_list<QVariantMap> fields)
{
    QVariantList result;
    for (const QVariantMap& field : fields)
        result.append(field);
    return result;
}

WidgetDescriptor widgetDescriptor(const QString& typeId,
                                  const QString& displayName,
                                  const QString& iconText,
                                  WidgetCategory category,
                                  qreal defaultWidth,
                                  qreal defaultHeight,
                                  const QString& backgroundColor,
                                  const QString& borderColor,
                                  const QString& fontColor,
                                  const QString& iconFileName,
                                  const QString& componentSource,
                                  const QVariantMap& defaultProperties,
                                  const QVariantList& propertyDefinitions,
                                  bool hasDataSource,
                                  const QVariantList& appearanceFields)
{
    const QString iconRoot = QStringLiteral("qrc:/qt/qml/EverSightDesigner/src/qml/editor/palette/assets/icons/");

    WidgetDescriptor descriptor;
    descriptor.typeId = typeId;
    descriptor.displayName = displayName;
    descriptor.iconText = iconText;
    descriptor.category = category;
    descriptor.defaultWidth = defaultWidth;
    descriptor.defaultHeight = defaultHeight;
    descriptor.defaultBackgroundColor = backgroundColor;
    descriptor.defaultBorderColor = borderColor;
    descriptor.defaultFontColor = fontColor;
    descriptor.iconSource = iconRoot + iconFileName;
    descriptor.componentSource = componentSource;
    descriptor.defaultProperties = defaultProperties;
    descriptor.propertyDefinitions = propertyDefinitions;
    descriptor.hasDataSource = hasDataSource;
    descriptor.appearanceFields = appearanceFields;
    return descriptor;
}

} // namespace

WidgetTypeRegistry::WidgetTypeRegistry()
{
    using C = WidgetCategory;

    const QVariantList frameAppearance = appearance({
        appearanceField(QStringLiteral("background"), QStringLiteral("Background")),
        appearanceField(QStringLiteral("border"), QStringLiteral("Border Color")),
        appearanceField(QStringLiteral("font"), QStringLiteral("Icon/Text Color"))
    });
    const QVariantList imageAppearance = appearance({
        appearanceField(QStringLiteral("background"), QStringLiteral("Background")),
        appearanceField(QStringLiteral("border"), QStringLiteral("Border Color"))
    });
    const QVariantList textAppearance = appearance({
        appearanceField(QStringLiteral("background"), QStringLiteral("Background")),
        appearanceField(QStringLiteral("border"), QStringLiteral("Border Color")),
        appearanceField(QStringLiteral("font"), QStringLiteral("Text Color"))
    });

    m_descriptors = {
        widgetDescriptor("Image", "Image", "IMG", C::Foundation, 320, 200, "#282c2f", "#474b50", "#dbe0e6",
                         "Image.png", "widgets/Foundation/ImageWidget.qml",
                         {{"dataSource", QString("")}, {"showStatusBar", false}, {"showToolsBar", true}, {"keepLastResult", false}, {"autoFill", false}},
                         {
                             propertyDefinition("dataSource", "Data Source", "text"),
                             propertyDefinition("graphicalConfiguration", "Graphical Configuration", "text"),
                             propertyDefinition("overlay1", "Overlay 1", "text"),
                             propertyDefinition("overlay2", "Overlay 2", "text"),
                             propertyDefinition("overlay3", "Overlay 3", "text"),
                             propertyDefinition("keepLastResult", "Keep Last Result", "bool"),
                             propertyDefinition("showStatusBar", "Show Status Bar", "bool"),
                             propertyDefinition("showToolsBar", "Show Tools Bar", "bool"),
                             propertyDefinition("displayAuxiliaryLine", "Display Auxiliary Line", "bool"),
                             propertyDefinition("historicalImages", "Historical Images", "bool")
                         },
                         true, imageAppearance),
        widgetDescriptor("MultipleImages", "Multiple Images", "N", C::Foundation, 400, 220, "#282c2f", "#474b50", "#dbe0e6",
                         "Multiple Images.png", "widgets/Foundation/ImageWidget.qml",
                         {{"imageQuantity", 2}, {"autoFill", false}, {"dataSource", QString("")}},
                         {
                             propertyDefinition("dataSource", "Data Source", "text"),
                             propertyDefinition("graphicalConfiguration", "Graphical Configuration", "text"),
                             propertyDefinition("overlay1", "Overlay 1", "text"),
                             propertyDefinition("overlay2", "Overlay 2", "text"),
                             propertyDefinition("overlay3", "Overlay 3", "text"),
                             propertyDefinition("keepLastResult", "Keep Last Result", "bool"),
                             propertyDefinition("showStatusBar", "Show Status Bar", "bool"),
                             propertyDefinition("showToolsBar", "Show Tools Bar", "bool"),
                             propertyDefinition("displayAuxiliaryLine", "Display Auxiliary Line", "bool"),
                             propertyDefinition("historicalImages", "Historical Images", "bool")
                         },
                         true, imageAppearance),
        widgetDescriptor("RunControl", "Run Control", ">", C::Foundation, 160, 72, "#1f3d60", "#2d5a8a", "#f3f9ff",
                         "Run Control.png", "widgets/Foundation/RunControlWidget.qml",
                         {{"dataSource", QString("AllProcess")}, {"controlType", QString("Both Display")}},
                         {
                             propertyDefinition("dataSource", "Data Source", "choice", {"Flow", "AllProcess"}),
                             propertyDefinition("controlType", "Control Type", "choice", {"Once", "Continuously", "Both"}),
                             propertyDefinition("iconColor", "Icon Color", "color")
                         },
                         true, frameAppearance),
        widgetDescriptor("RunStatus", "Run Status", "o", C::Foundation, 120, 72, "#1a3528", "#2e5c45", "#dff0d8",
                         "Run Status.png", "widgets/Foundation/RunStatusWidget.qml",
                         {{"dataSource", QString("AllProcess")}},
                         {
                             propertyDefinition("dataSource", "Data Source", "text"),
                             propertyDefinition("freeBackgroundColor", "Free Background", "color"),
                             propertyDefinition("freeForegroundColor", "Free Foreground", "color"),
                             propertyDefinition("busyBackgroundColor", "Busy Background", "color"),
                             propertyDefinition("busyForegroundColor", "Busy Foreground", "color"),
                             propertyDefinition("filletRadius", "Fillet Radius", "int")
                         },
                         true, frameAppearance),
        widgetDescriptor("OKNG", "OK / NG", "OK", C::Foundation, 120, 72, "#1d3e24", "#2e5c35", "#dff0d8",
                         "OK&NG.png", "widgets/Foundation/OKNGWidget.qml",
                         {{"dataSource", QString("")}, {"okContent", QString("OK")}, {"ngContent", QString("NG")}},
                         {
                             propertyDefinition("dataSource", "Data Source", "text"),
                             propertyDefinition("okContent", "OK Content", "text"),
                             propertyDefinition("ngContent", "NG Content", "text"),
                             propertyDefinition("okColor", "OK Color", "color"),
                             propertyDefinition("ngColor", "NG Color", "color"),
                             propertyDefinition("font", "Font", "text"),
                             propertyDefinition("background", "Background", "color"),
                             propertyDefinition("border", "Border", "color")
                         },
                         true, textAppearance),
        widgetDescriptor("Textbox", "Textbox", "T", C::Foundation, 220, 48, "#2f3438", "#52585f", "#f5f5f5",
                         "Textbox.png", "widgets/Foundation/TextboxWidget.qml",
                         {{"dataType", QString("string")}, {"dataSource", QString("")}, {"formatString", QString("")}, {"text", QString("Label")}, {"autoFill", false}},
                         {
                             propertyDefinition("dataType", "Data Type", "choice", {"String", "Int", "Float"}),
                             propertyDefinition("dataSource", "Data Source", "text"),
                             propertyDefinition("formatString", "Format String", "choice", {"%s", "%d", "%f"}),
                             propertyDefinition("text", "Custom Text", "text")
                         },
                         true, textAppearance),
        widgetDescriptor("Button", "Button", "BTN", C::Foundation, 140, 56, "#ff8a00", "#ffb15c", "#ffffff",
                         "Button.png", "widgets/Foundation/ButtonWidget.qml",
                         {{"showText", QString("Button")}, {"triggerType", QString("Solution")}},
                         {
                             propertyDefinition("triggerCommands", "Trigger Commands", "text"),
                             propertyDefinition("imageData", "ImageData", "text"),
                             propertyDefinition("stretchMode", "Stretch Mode", "choice", {"None", "Fill", "Uniform", "UniformToFill"}),
                             propertyDefinition("clickColor", "Click Color", "color")
                         },
                         false, frameAppearance),
        widgetDescriptor("SwitchControl", "Switch Control", "SW", C::Foundation, 140, 56, "#3d5a7f", "#5b7d9b", "#f0f4f8",
                         "Switch Control.png", "widgets/Foundation/SwitchControlWidget.qml",
                         {{"dataSource", QString("")}},
                         { propertyDefinition("dataSource", "Data Source (Bool)", "text") },
                         true, frameAppearance),
        widgetDescriptor("ParamsSettings", "Params Settings", "SET", C::Foundation, 220, 140, "#2c3239", "#515960", "#e8ecf0",
                         "Params Settings.png", "widgets/Foundation/ParamsSettingsWidget.qml",
                         {{"parameterType", QString("float")}, {"dataSource", QString("")}, {"paramName", QString("Parameter")}, {"decimalPlaces", 3}},
                         {
                             propertyDefinition("parameterType", "Parameter Type", "choice", {"Int", "Float"}),
                             propertyDefinition("dataSource", "Data Source", "text"),
                             propertyDefinition("paramName", "Param Name", "text"),
                             propertyDefinition("decimalPlaces", "Decimal Places", "int")
                         },
                         true, textAppearance),
        widgetDescriptor("VariableSettings", "Variable Settings", "var", C::Foundation, 180, 80, "#253141", "#4a5468", "#d8e3f0",
                         "Variable Settings.png", "widgets/Foundation/VariableSettingsWidget.qml",
                         {{"dataSource", QString("")}},
                         { propertyDefinition("dataSource", "Data Source", "text") },
                         true, textAppearance),
        widgetDescriptor("CharacterSettings", "Character Settings", "A", C::Foundation, 180, 48, "#2f3134", "#4a4e53", "#f5f5f5",
                         "Character Settings.png", "widgets/Foundation/CharacterSettingsWidget.qml",
                         {{"dataSource", QString("")}},
                         { propertyDefinition("dataSource", "Data Source", "text") },
                         true, textAppearance),
        widgetDescriptor("TrafficLight", "Traffic Light", "TL", C::Foundation, 100, 120, "#212529", "#4e5b68", "#e8ecf0",
                         "Trafic Light.png", "widgets/Foundation/TrafficLightWidget.qml",
                         {{"dataSource", QString("")}},
                         {
                             propertyDefinition("dataSource", "Data Source", "text"),
                             propertyDefinition("trafficLightContent", "Traffic Light Content", "text")
                         },
                         true, imageAppearance),
        widgetDescriptor("ConditionalLight", "Conditional Light", "IF", C::Foundation, 180, 72, "#3b3222", "#705d35", "#f5e9c8",
                         "Conditional Light.png", "widgets/Foundation/ConditionalLightWidget.qml",
                         {{"parameterType", QString("float")}, {"dataSource", QString("")}, {"gtThreshold", 0.0}, {"ltThreshold", 0.0}},
                         {
                             propertyDefinition("parameterType", "Parameter Type", "choice", {"Int", "Float"}),
                             propertyDefinition("dataSource", "Data Source", "text"),
                             propertyDefinition("gtThreshold", "> Threshold", "real"),
                             propertyDefinition("ltThreshold", "< Threshold", "real"),
                             propertyDefinition("gtThresholdColor", "> Color", "color"),
                             propertyDefinition("ltThresholdColor", "< Color", "color"),
                             propertyDefinition("defaultColor", "Default Color", "color")
                         },
                         true, frameAppearance),
        widgetDescriptor("GroupBox", "Group Box", "BOX", C::Layout, 360, 220, "#2d3136", "#5b5f65", "#e8ecf0",
                         "Group Box.png", "widgets/Layout/GroupBoxWidget.qml",
                         {{"groupTitle", QString("Group")}},
                         {
                             propertyDefinition("groupTitle", "Title", "text"),
                             propertyDefinition("headerBackground", "Header Background", "color")
                         },
                         false, textAppearance),
        widgetDescriptor("ChildInterface", "Child Interface", "CH", C::Layout, 160, 56, "#2d3034", "#4f5358", "#e8ecf0",
                         "Child Interface.png", "widgets/Layout/ChildInterfaceWidget.qml",
                         {{"windowName", QString("Sub Window")}, {"passwordEnabled", false}},
                         {
                             propertyDefinition("subinterface", "Edit Subinterface", "text"),
                             propertyDefinition("passwordEnabled", "Password", "bool"),
                             propertyDefinition("displayIcon", "Display Icon", "bool")
                         },
                         false, frameAppearance),
        widgetDescriptor("TabControl", "Tab Control", "TAB", C::Layout, 360, 220, "#2b2f33", "#54595f", "#e8ecf0",
                         "Tab Control.png", "widgets/Layout/TabControlWidget.qml",
                         {{"tabPageQuantity", 2}},
                         {
                             propertyDefinition("tabPageQuantity", "Tab Page Quantity", "int"),
                             propertyDefinition("selectBackground", "Select Background", "color"),
                             propertyDefinition("align", "Align", "choice", {"Left", "Center", "Right"}),
                             propertyDefinition("tabPageNames", "Tab Page Names", "text")
                         },
                         false, textAppearance),
        widgetDescriptor("Table", "Table", "TBL", C::Chart, 340, 180, "#32363a", "#4b4f54", "#e8ecf0",
                         "Table.png", "widgets/Chart/TableWidget.qml",
                         {{"rows", 10}, {"columns", 4}, {"lineTitle", QString("Result")}},
                         {
                             propertyDefinition("rows", "Rows", "int"),
                             propertyDefinition("columns", "Columns", "int"),
                             propertyDefinition("columnConfig", "Column Config", "text"),
                             propertyDefinition("lineTitle", "Line Title", "text"),
                             propertyDefinition("colorEdit", "Color Edit", "color")
                         },
                         false, textAppearance),
        widgetDescriptor("LineDiagramArray", "Line Diagram Array", "LIN", C::Chart, 320, 180, "#2b2f33", "#4f5358", "#e8ecf0",
                         "Line Diagram Array.png", "widgets/Chart/LineDiagramArrayWidget.qml",
                         {{"latestRunTimes", 50}},
                         {
                             propertyDefinition("dataSourceList", "Data Source List", "text"),
                             propertyDefinition("upperLimit", "Upper Limit", "real"),
                             propertyDefinition("lowerLimit", "Lower Limit", "real"),
                             propertyDefinition("latestRunTimes", "Latest Runtime Data", "int")
                         },
                         true, textAppearance),
        widgetDescriptor("MultiLineDiagram", "Multi Line Diagram", "ML", C::Chart, 340, 180, "#2b2f33", "#4f5358", "#e8ecf0",
                         "Multi Line Diagram.png", "widgets/Chart/MultiLineDiagramWidget.qml",
                         {{"latestRunTimes", 50}},
                         {
                             propertyDefinition("dataSourceList", "Data Source List", "text"),
                             propertyDefinition("upperLimit", "Upper Limit", "real"),
                             propertyDefinition("lowerLimit", "Lower Limit", "real"),
                             propertyDefinition("latestRunTimes", "Latest Runtime Data", "int")
                         },
                         true, textAppearance),
        widgetDescriptor("ProductionStatistics", "Production Statistics", "STA", C::Chart, 300, 160, "#2b2f33", "#4f5358", "#e8ecf0",
                         "Production Statistics.png", "widgets/Chart/ProductionStatisticsWidget.qml",
                         {{"showMax", true}, {"showMin", true}, {"showAverage", true}},
                         {
                             propertyDefinition("dataSourceList", "Data Source List", "text"),
                             propertyDefinition("showCurrent", "Show Current", "bool"),
                             propertyDefinition("showMax", "Show Max", "bool"),
                             propertyDefinition("showMin", "Show Min", "bool"),
                             propertyDefinition("showAverage", "Show Avg", "bool"),
                             propertyDefinition("showRange", "Show Range", "bool"),
                             propertyDefinition("showStdDev", "Show Std Dev", "bool")
                         },
                         true, textAppearance),
        widgetDescriptor("PieControl", "Pie Control", "PIE", C::Chart, 240, 240, "#2b2f33", "#4f5358", "#e8ecf0",
                         "Pie Control.png", "widgets/Chart/PieControlWidget.qml",
                         {},
                         {
                             propertyDefinition("dataSourceList", "Data Source List", "text"),
                             propertyDefinition("defaultColors", "Default Colors", "text")
                         },
                         true, imageAppearance),
        widgetDescriptor("ParamSettingsArray", "Param Settings Array", "PSA", C::Chart, 300, 140, "#2c3239", "#515960", "#e8ecf0",
                         "Param Settings Array.png", "widgets/Chart/ParamSettingsArrayWidget.qml",
                         {{"orientation", QString("Horizontal")}, {"spacing", 8}},
                         {
                             propertyDefinition("dataSourceList", "Data Source List", "text"),
                             propertyDefinition("orientation", "Orientation", "choice", {"Horizontal", "Vertical"}),
                             propertyDefinition("spacing", "Spacing", "int")
                         },
                         true, textAppearance),
        widgetDescriptor("LabelArray", "Label Array", "LA", C::Chart, 300, 140, "#2f3134", "#4a4e53", "#f5f5f5",
                         "Label Array.png", "widgets/Chart/LabelArrayWidget.qml",
                         {{"orientation", QString("Horizontal")}, {"spacing", 8}},
                         {
                             propertyDefinition("dataSourceList", "Data Source List", "text"),
                             propertyDefinition("orientation", "Orientation", "choice", {"Horizontal", "Vertical"}),
                             propertyDefinition("spacing", "Spacing", "int")
                         },
                         true, textAppearance),
        widgetDescriptor("StaticImage", "Static Image", "PIC", C::Other, 200, 150, "#1e2124", "#3a3e42", "#e8ecf0",
                         "Image.png", "widgets/Foundation/ImageWidget.qml",
                         {{"stretchMode", QString("Uniform")}},
                         {
                             propertyDefinition("imageData", "ImageData", "text"),
                             propertyDefinition("stretchMode", "Stretch Mode", "choice", {"None", "Fill", "Uniform", "UniformToFill"})
                         },
                         false, imageAppearance),
        widgetDescriptor("WorldClock", "World Clock", "CLK", C::Other, 200, 48, "#1e2124", "#3a3e42", "#f5f5f5",
                         "Image.png", "widgets/Foundation/WorldClockWidget.qml",
                         {{"format", QString("HH:mm:ss")}, {"timeZone", QString("UTC")}},
                         {
                             propertyDefinition("format", "Time Format", "text"),
                             propertyDefinition("timeZone", "Time Zone", "text")
                         },
                         false, textAppearance),
        widgetDescriptor("Log", "Log", "LOG", C::Other, 400, 200, "#1e2124", "#3a3e42", "#e8ecf0",
                         "Image.png", "widgets/Foundation/LogWidget.qml",
                         {},
                         {
                             propertyDefinition("displayTitleBar", "Display TitleBar", "bool"),
                             propertyDefinition("logColorSystem", "Log Color System", "text")
                         },
                         false, textAppearance)
    };

    for (int i = 0; i < m_descriptors.size(); ++i)
        m_indexByTypeId.insert(m_descriptors[i].typeId, i);
}

const WidgetTypeRegistry& WidgetTypeRegistry::instance()
{
    static WidgetTypeRegistry registry;
    return registry;
}

const WidgetTypeDescriptor* WidgetTypeRegistry::descriptor(const QString& typeId) const
{
    const auto it = m_indexByTypeId.find(typeId);
    return it != m_indexByTypeId.end() ? &m_descriptors.at(it.value()) : nullptr;
}

const QList<WidgetTypeDescriptor>& WidgetTypeRegistry::allDescriptors() const
{
    return m_descriptors;
}

QList<const WidgetTypeDescriptor*> WidgetTypeRegistry::descriptorsForCategory(WidgetCategory category) const
{
    QList<const WidgetTypeDescriptor*> result;
    for (const auto& descriptor : m_descriptors) {
        if (descriptor.category == category)
            result.append(&descriptor);
    }
    return result;
}

} // namespace eversight
