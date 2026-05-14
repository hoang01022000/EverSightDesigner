#include "WidgetTypeRegistry.h"

namespace eversight {

static QVariantMap propertyDefinition(
    const QString& key,
    const QString& label,
    const QString& editor,
    const QVariantList& options = {})
{
    QVariantMap definition;
    definition.insert("key", key);
    definition.insert("label", label);
    definition.insert("editor", editor);
    if (!options.isEmpty())
        definition.insert("options", options);
    return definition;
}

static QVariantList widgetPropertyDefinitions(const QString& typeId)
{
    if (typeId == "Image" || typeId == "MultipleImages") {
        return {
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
        };
    }
    if (typeId == "RunControl") {
        return {
            propertyDefinition("dataSource", "Data Source", "choice", {"Flow", "AllProcess"}),
            propertyDefinition("controlType", "Control Type", "choice", {"Once", "Continuously", "Both"}),
            propertyDefinition("iconColor", "Icon Color", "color")
        };
    }
    if (typeId == "RunStatus") {
        return {
            propertyDefinition("dataSource", "Data Source", "text"),
            propertyDefinition("freeBackgroundColor", "Free Background", "color"),
            propertyDefinition("freeForegroundColor", "Free Foreground", "color"),
            propertyDefinition("busyBackgroundColor", "Busy Background", "color"),
            propertyDefinition("busyForegroundColor", "Busy Foreground", "color"),
            propertyDefinition("filletRadius", "Fillet Radius", "int")
        };
    }
    if (typeId == "OKNG") {
        return {
            propertyDefinition("dataSource", "Data Source", "text"),
            propertyDefinition("okContent", "OK Content", "text"),
            propertyDefinition("ngContent", "NG Content", "text"),
            propertyDefinition("okColor", "OK Color", "color"),
            propertyDefinition("ngColor", "NG Color", "color"),
            propertyDefinition("font", "Font", "text"),
            propertyDefinition("background", "Background", "color"),
            propertyDefinition("border", "Border", "color")
        };
    }
    if (typeId == "Textbox") {
        return {
            propertyDefinition("dataType", "Data Type", "choice", {"String", "Int", "Float"}),
            propertyDefinition("dataSource", "Data Source", "text"),
            propertyDefinition("formatString", "Format String", "choice", {"%s", "%d", "%f"}),
            propertyDefinition("text", "Custom Text", "text")
        };
    }
    if (typeId == "Button") {
        return {
            propertyDefinition("triggerCommands", "Trigger Commands", "text"),
            propertyDefinition("imageData", "ImageData", "text"),
            propertyDefinition("stretchMode", "Stretch Mode", "choice", {"None", "Fill", "Uniform", "UniformToFill"}),
            propertyDefinition("clickColor", "Click Color", "color")
        };
    }
    if (typeId == "SwitchControl") {
        return {
            propertyDefinition("dataSource", "Data Source (Bool)", "text")
        };
    }
    if (typeId == "ParamsSettings") {
        return {
            propertyDefinition("parameterType", "Parameter Type", "choice", {"Int", "Float"}),
            propertyDefinition("dataSource", "Data Source", "text"),
            propertyDefinition("paramName", "Param Name", "text"),
            propertyDefinition("decimalPlaces", "Decimal Places", "int")
        };
    }
    if (typeId == "TrafficLight") {
        return {
            propertyDefinition("dataSource", "Data Source", "text"),
            propertyDefinition("trafficLightContent", "Traffic Light Content", "text")
        };
    }
    if (typeId == "ConditionalLight") {
        return {
            propertyDefinition("parameterType", "Parameter Type", "choice", {"Int", "Float"}),
            propertyDefinition("dataSource", "Data Source", "text"),
            propertyDefinition("gtThreshold", "> Threshold", "real"),
            propertyDefinition("ltThreshold", "< Threshold", "real"),
            propertyDefinition("gtThresholdColor", "> Color", "color"),
            propertyDefinition("ltThresholdColor", "< Color", "color"),
            propertyDefinition("defaultColor", "Default Color", "color")
        };
    }
    if (typeId == "GroupBox") {
        return {
            propertyDefinition("groupTitle", "Title", "text"),
            propertyDefinition("headerBackground", "Header Background", "color")
        };
    }
    if (typeId == "ChildInterface") {
        return {
            propertyDefinition("subinterface", "Edit Subinterface", "text"),
            propertyDefinition("passwordEnabled", "Password", "bool"),
            propertyDefinition("displayIcon", "Display Icon", "bool")
        };
    }
    if (typeId == "TabControl") {
        return {
            propertyDefinition("tabPageQuantity", "Tab Page Quantity", "int"),
            propertyDefinition("selectBackground", "Select Background", "color"),
            propertyDefinition("align", "Align", "choice", {"Left", "Center", "Right"}),
            propertyDefinition("tabPageNames", "Tab Page Names", "text")
        };
    }
    if (typeId == "Table") {
        return {
            propertyDefinition("rows", "Rows", "int"),
            propertyDefinition("columns", "Columns", "int"),
            propertyDefinition("columnConfig", "Column Config", "text"),
            propertyDefinition("lineTitle", "Line Title", "text"),
            propertyDefinition("colorEdit", "Color Edit", "color")
        };
    }
    if (typeId == "LineDiagramArray" || typeId == "MultiLineDiagram") {
        return {
            propertyDefinition("dataSourceList", "Data Source List", "text"),
            propertyDefinition("upperLimit", "Upper Limit", "real"),
            propertyDefinition("lowerLimit", "Lower Limit", "real"),
            propertyDefinition("latestRunTimes", "Latest Runtime Data", "int")
        };
    }
    if (typeId == "ProductionStatistics") {
        return {
            propertyDefinition("dataSourceList", "Data Source List", "text"),
            propertyDefinition("showCurrent", "Show Current", "bool"),
            propertyDefinition("showMax", "Show Max", "bool"),
            propertyDefinition("showMin", "Show Min", "bool"),
            propertyDefinition("showAverage", "Show Avg", "bool"),
            propertyDefinition("showRange", "Show Range", "bool"),
            propertyDefinition("showStdDev", "Show Std Dev", "bool")
        };
    }
    if (typeId == "PieControl") {
        return {
            propertyDefinition("dataSourceList", "Data Source List", "text"),
            propertyDefinition("defaultColors", "Default Colors", "text")
        };
    }
    if (typeId == "ParamSettingsArray" || typeId == "LabelArray") {
        return {
            propertyDefinition("dataSourceList", "Data Source List", "text"),
            propertyDefinition("orientation", "Orientation", "choice", {"Horizontal", "Vertical"}),
            propertyDefinition("spacing", "Spacing", "int")
        };
    }
    if (typeId == "StaticImage") {
        return {
            propertyDefinition("imageData", "ImageData", "text"),
            propertyDefinition("stretchMode", "Stretch Mode", "choice", {"None", "Fill", "Uniform", "UniformToFill"})
        };
    }
    if (typeId == "WorldClock") {
        return {
            propertyDefinition("format", "Time Format", "text"),
            propertyDefinition("timeZone", "Time Zone", "text")
        };
    }
    if (typeId == "Log") {
        return {
            propertyDefinition("displayTitleBar", "Display TitleBar", "bool"),
            propertyDefinition("logColorSystem", "Log Color System", "text")
        };
    }

    return {};
}

static WidgetTypeDescriptor make(
    const QString& typeId, const QString& displayName, const QString& iconText,
    WidgetCategory category, qreal w, qreal h,
    const QString& bg, const QString& border, const QString& font,
    const QVariantMap& props = {})
{
    WidgetTypeDescriptor d;
    d.typeId                 = typeId;
    d.displayName            = displayName;
    d.iconText               = iconText;
    d.category               = category;
    d.defaultWidth           = w;
    d.defaultHeight          = h;
    d.defaultBackgroundColor = bg;
    d.defaultBorderColor     = border;
    d.defaultFontColor       = font;
    d.defaultProperties      = props;
    return d;
}

WidgetTypeRegistry::WidgetTypeRegistry()
{
    using C = WidgetCategory;

    m_descriptors = {
        make("Image",               "Image",                "IMG", C::Foundation, 320,200, "#282c2f","#474b50","#dbe0e6", {{"dataSource",QString("")},{"showStatusBar",false},{"showToolsBar",true},{"keepLastResult",false},{"autoFill",false}}),
        make("MultipleImages",      "Multiple Images",      "N",   C::Foundation, 400,220, "#282c2f","#474b50","#dbe0e6", {{"imageQuantity",2},{"autoFill",false}}),
        make("RunControl",          "Run Control",          "▶",   C::Foundation, 160, 72, "#1f3d60","#2d5a8a","#f3f9ff", {{"dataSource",QString("AllProcess")},{"controlType",QString("Both Display")}}),
        make("RunStatus",           "Run Status",           "●",   C::Foundation, 120, 72, "#1a3528","#2e5c45","#dff0d8", {{"dataSource",QString("AllProcess")}}),
        make("OKNG",                "OK / NG",              "OK",  C::Foundation, 120, 72, "#1d3e24","#2e5c35","#dff0d8", {{"dataSource",QString("")},{"okContent",QString("OK")},{"ngContent",QString("NG")}}),
        make("Textbox",             "Textbox",              "T",   C::Foundation, 220, 48, "#2f3438","#52585f","#f5f5f5", {{"dataType",QString("string")},{"dataSource",QString("")},{"formatString",QString("")},{"text",QString("Label")},{"autoFill",false}}),
        make("Button",              "Button",               "BTN", C::Foundation, 140, 56, "#ff8a00","#ffb15c","#ffffff", {{"showText",QString("Button")},{"triggerType",QString("Solution")}}),
        make("SwitchControl",       "Switch Control",       "SW",  C::Foundation, 140, 56, "#3d5a7f","#5b7d9b","#f0f4f8", {{"dataSource",QString("")}}),
        make("ParamsSettings",      "Params Settings",      "SET", C::Foundation, 220,140, "#2c3239","#515960","#e8ecf0", {{"parameterType",QString("float")},{"dataSource",QString("")},{"paramName",QString("Parameter")},{"decimalPlaces",3}}),
        make("VariableSettings",    "Variable Settings",    "var", C::Foundation, 180, 80, "#253141","#4a5468","#d8e3f0", {{"dataSource",QString("")}}),
        make("CharacterSettings",   "Character Settings",   "A",   C::Foundation, 180, 48, "#2f3134","#4a4e53","#f5f5f5", {{"dataSource",QString("")}}),
        make("TrafficLight",        "Traffic Light",        "TL",  C::Foundation, 100,120, "#212529","#4e5b68","#e8ecf0", {{"dataSource",QString("")}}),
        make("ConditionalLight",    "Conditional Light",    "IF",  C::Foundation, 180, 72, "#3b3222","#705d35","#f5e9c8", {{"parameterType",QString("float")},{"dataSource",QString("")},{"gtThreshold",0.0},{"ltThreshold",0.0}}),

        make("GroupBox",            "Group Box",            "BOX", C::Layout,     360,220, "#2d3136","#5b5f65","#e8ecf0", {{"groupTitle",QString("Group")}}),
        make("ChildInterface",      "Child Interface",      "CH",  C::Layout,     160, 56, "#2d3034","#4f5358","#e8ecf0", {{"windowName",QString("Sub Window")},{"passwordEnabled",false}}),
        make("TabControl",          "Tab Control",          "TAB", C::Layout,     360,220, "#2b2f33","#54595f","#e8ecf0", {{"tabPageQuantity",2}}),

        make("Table",               "Table",                "TBL", C::Chart,      340,180, "#32363a","#4b4f54","#e8ecf0", {{"rows",10},{"columns",4},{"lineTitle",QString("Result")}}),
        make("LineDiagramArray",    "Line Diagram Array",   "LIN", C::Chart,      320,180, "#2b2f33","#4f5358","#e8ecf0", {{"latestRunTimes",50}}),
        make("MultiLineDiagram",    "Multi Line Diagram",   "ML",  C::Chart,      340,180, "#2b2f33","#4f5358","#e8ecf0", {{"latestRunTimes",50}}),
        make("ProductionStatistics","Production Statistics","STA", C::Chart,      300,160, "#2b2f33","#4f5358","#e8ecf0", {{"showMax",true},{"showMin",true},{"showAverage",true}}),
        make("PieControl",          "Pie Control",          "PIE", C::Chart,      240,240, "#2b2f33","#4f5358","#e8ecf0", {}),
        make("ParamSettingsArray",  "Param Settings Array", "PSA", C::Chart,      300,140, "#2c3239","#515960","#e8ecf0", {{"orientation",QString("Horizontal")},{"spacing",8}}),
        make("LabelArray",          "Label Array",          "LA",  C::Chart,      300,140, "#2f3134","#4a4e53","#f5f5f5", {{"orientation",QString("Horizontal")},{"spacing",8}}),

        make("StaticImage",         "Static Image",         "PIC", C::Other,      200,150, "#1e2124","#3a3e42","#e8ecf0", {{"stretchMode",QString("Uniform")}}),
        make("WorldClock",          "World Clock",          "CLK", C::Other,      200, 48, "#1e2124","#3a3e42","#f5f5f5", {{"format",QString("HH:mm:ss")},{"timeZone",QString("UTC")}}),
        make("Log",                 "Log",                  "LOG", C::Other,      400,200, "#1e2124","#3a3e42","#e8ecf0", {})
    };

    for (int i = 0; i < m_descriptors.size(); ++i)
        m_indexByTypeId.insert(m_descriptors[i].typeId, i);

    // Populate iconSource for descriptors from assets when available
    for (auto &d : m_descriptors) {
        const QString t = d.typeId;
        if (t == "Image") d.iconSource = "assets/icons/Image.png";
        else if (t == "MultipleImages") d.iconSource = "assets/icons/Multiple Images.png";
        else if (t == "StaticImage") d.iconSource = "assets/icons/Image.png";
        else if (t == "RunControl") d.iconSource = "assets/icons/Run Control.png";
        else if (t == "RunStatus") d.iconSource = "assets/icons/Run Status.png";
        else if (t == "OKNG") d.iconSource = "assets/icons/OK&NG.png";
        else if (t == "Textbox") d.iconSource = "assets/icons/Textbox.png";
        else if (t == "Button") d.iconSource = "assets/icons/Button.png";
        else if (t == "SwitchControl") d.iconSource = "assets/icons/Button.png";
        else if (t == "ParamsSettings") d.iconSource = "assets/icons/Params Settings.png";
        else if (t == "VariableSettings") d.iconSource = "assets/icons/Variable Settings.png";
        else if (t == "CharacterSettings") d.iconSource = "assets/icons/Character Settings.png";
        else if (t == "TrafficLight") d.iconSource = "assets/icons/Trafic Light.png";
        else if (t == "ConditionalLight") d.iconSource = "assets/icons/Conditional Light.png";
        else if (t == "GroupBox") d.iconSource = "assets/icons/Group Box.png";
        else if (t == "ChildInterface") d.iconSource = "assets/icons/Child Interface.png";
        else if (t == "TabControl") d.iconSource = "assets/icons/Tab Control.png";
        else if (t == "Table") d.iconSource = "assets/icons/Table.png";
        else if (t == "LineDiagramArray") d.iconSource = "assets/icons/Line Diagram Array.png";
        else if (t == "MultiLineDiagram") d.iconSource = "assets/icons/Multi Line Diagram.png";
        else if (t == "ProductionStatistics") d.iconSource = "assets/icons/Production Statistics.png";
        else if (t == "PieControl") d.iconSource = "assets/icons/Pie Control.png";
        else if (t == "ParamSettingsArray") d.iconSource = "assets/icons/Param Settings Array.png";
        else if (t == "LabelArray") d.iconSource = "assets/icons/Label Array.png";
        else d.iconSource = "assets/icons/Image.png";

        d.componentSource = "widgets/PlaceholderWidget.qml";
        if (t == "Image" || t == "MultipleImages")
            d.componentSource = "widgets/Foundation/ImageWidget.qml";
        else if (t == "RunControl")
            d.componentSource = "widgets/Foundation/RunControlWidget.qml";
        else if (t == "RunStatus")
            d.componentSource = "widgets/Foundation/RunStatusWidget.qml";
        else if (t == "OKNG")
            d.componentSource = "widgets/Foundation/OKNGWidget.qml";
        else if (t == "Textbox")
            d.componentSource = "widgets/Foundation/TextboxWidget.qml";
        else if (t == "Button" || t == "SwitchControl")
            d.componentSource = "widgets/Foundation/ButtonWidget.qml";
        else if (t == "TrafficLight")
            d.componentSource = "widgets/Foundation/TrafficLightWidget.qml";
        else if (t == "Table")
            d.componentSource = "widgets/Chart/TableWidget.qml";

        d.propertyDefinitions = widgetPropertyDefinitions(t);
    }
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
    for (const auto& d : m_descriptors)
        if (d.category == category)
            result.append(&d);
    return result;
}

} // namespace eversight
