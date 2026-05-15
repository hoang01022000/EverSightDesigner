#include "PropertyDefinition.h"

namespace eversight {

QVariantMap PropertyDefinition::create(const QString& key,
                                       const QString& label,
                                       const QString& editor,
                                       const QVariantList& options)
{
    QVariantMap definition;
    definition.insert("key", key);
    definition.insert("label", label);
    definition.insert("editor", editor);
    if (!options.isEmpty())
        definition.insert("options", options);
    return definition;
}

QVariantList PropertyDefinition::forWidget(const QString& typeId)
{
    if (typeId == "Image" || typeId == "MultipleImages") {
        return {
            create("dataSource", "Data Source", "text"),
            create("graphicalConfiguration", "Graphical Configuration", "text"),
            create("overlay1", "Overlay 1", "text"),
            create("overlay2", "Overlay 2", "text"),
            create("overlay3", "Overlay 3", "text"),
            create("keepLastResult", "Keep Last Result", "bool"),
            create("showStatusBar", "Show Status Bar", "bool"),
            create("showToolsBar", "Show Tools Bar", "bool"),
            create("displayAuxiliaryLine", "Display Auxiliary Line", "bool"),
            create("historicalImages", "Historical Images", "bool")
        };
    }
    if (typeId == "RunControl") {
        return {
            create("dataSource", "Data Source", "choice", {"Flow", "AllProcess"}),
            create("controlType", "Control Type", "choice", {"Once", "Continuously", "Both"}),
            create("iconColor", "Icon Color", "color")
        };
    }
    if (typeId == "RunStatus") {
        return {
            create("dataSource", "Data Source", "text"),
            create("freeBackgroundColor", "Free Background", "color"),
            create("freeForegroundColor", "Free Foreground", "color"),
            create("busyBackgroundColor", "Busy Background", "color"),
            create("busyForegroundColor", "Busy Foreground", "color"),
            create("filletRadius", "Fillet Radius", "int")
        };
    }
    if (typeId == "OKNG") {
        return {
            create("dataSource", "Data Source", "text"),
            create("okContent", "OK Content", "text"),
            create("ngContent", "NG Content", "text"),
            create("okColor", "OK Color", "color"),
            create("ngColor", "NG Color", "color"),
            create("font", "Font", "text"),
            create("background", "Background", "color"),
            create("border", "Border", "color")
        };
    }
    if (typeId == "Textbox") {
        return {
            create("dataType", "Data Type", "choice", {"String", "Int", "Float"}),
            create("dataSource", "Data Source", "text"),
            create("formatString", "Format String", "choice", {"%s", "%d", "%f"}),
            create("text", "Custom Text", "text")
        };
    }
    if (typeId == "Button") {
        return {
            create("triggerCommands", "Trigger Commands", "text"),
            create("imageData", "ImageData", "text"),
            create("stretchMode", "Stretch Mode", "choice", {"None", "Fill", "Uniform", "UniformToFill"}),
            create("clickColor", "Click Color", "color")
        };
    }
    if (typeId == "SwitchControl") {
        return { create("dataSource", "Data Source (Bool)", "text") };
    }
    if (typeId == "ParamsSettings") {
        return {
            create("parameterType", "Parameter Type", "choice", {"Int", "Float"}),
            create("dataSource", "Data Source", "text"),
            create("paramName", "Param Name", "text"),
            create("decimalPlaces", "Decimal Places", "int")
        };
    }
    if (typeId == "TrafficLight") {
        return {
            create("dataSource", "Data Source", "text"),
            create("trafficLightContent", "Traffic Light Content", "text")
        };
    }
    if (typeId == "ConditionalLight") {
        return {
            create("parameterType", "Parameter Type", "choice", {"Int", "Float"}),
            create("dataSource", "Data Source", "text"),
            create("gtThreshold", "> Threshold", "real"),
            create("ltThreshold", "< Threshold", "real"),
            create("gtThresholdColor", "> Color", "color"),
            create("ltThresholdColor", "< Color", "color"),
            create("defaultColor", "Default Color", "color")
        };
    }
    if (typeId == "GroupBox") {
        return {
            create("groupTitle", "Title", "text"),
            create("headerBackground", "Header Background", "color")
        };
    }
    if (typeId == "ChildInterface") {
        return {
            create("subinterface", "Edit Subinterface", "text"),
            create("passwordEnabled", "Password", "bool"),
            create("displayIcon", "Display Icon", "bool")
        };
    }
    if (typeId == "TabControl") {
        return {
            create("tabPageQuantity", "Tab Page Quantity", "int"),
            create("selectBackground", "Select Background", "color"),
            create("align", "Align", "choice", {"Left", "Center", "Right"}),
            create("tabPageNames", "Tab Page Names", "text")
        };
    }
    if (typeId == "Table") {
        return {
            create("rows", "Rows", "int"),
            create("columns", "Columns", "int"),
            create("columnConfig", "Column Config", "text"),
            create("lineTitle", "Line Title", "text"),
            create("colorEdit", "Color Edit", "color")
        };
    }
    if (typeId == "LineDiagramArray" || typeId == "MultiLineDiagram") {
        return {
            create("dataSourceList", "Data Source List", "text"),
            create("upperLimit", "Upper Limit", "real"),
            create("lowerLimit", "Lower Limit", "real"),
            create("latestRunTimes", "Latest Runtime Data", "int")
        };
    }
    if (typeId == "ProductionStatistics") {
        return {
            create("dataSourceList", "Data Source List", "text"),
            create("showCurrent", "Show Current", "bool"),
            create("showMax", "Show Max", "bool"),
            create("showMin", "Show Min", "bool"),
            create("showAverage", "Show Avg", "bool"),
            create("showRange", "Show Range", "bool"),
            create("showStdDev", "Show Std Dev", "bool")
        };
    }
    if (typeId == "PieControl") {
        return {
            create("dataSourceList", "Data Source List", "text"),
            create("defaultColors", "Default Colors", "text")
        };
    }
    if (typeId == "ParamSettingsArray" || typeId == "LabelArray") {
        return {
            create("dataSourceList", "Data Source List", "text"),
            create("orientation", "Orientation", "choice", {"Horizontal", "Vertical"}),
            create("spacing", "Spacing", "int")
        };
    }
    if (typeId == "StaticImage") {
        return {
            create("imageData", "ImageData", "text"),
            create("stretchMode", "Stretch Mode", "choice", {"None", "Fill", "Uniform", "UniformToFill"})
        };
    }
    if (typeId == "WorldClock") {
        return {
            create("format", "Time Format", "text"),
            create("timeZone", "Time Zone", "text")
        };
    }
    if (typeId == "Log") {
        return {
            create("displayTitleBar", "Display TitleBar", "bool"),
            create("logColorSystem", "Log Color System", "text")
        };
    }

    return {};
}

} // namespace eversight
