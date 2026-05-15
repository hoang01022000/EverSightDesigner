#include "WidgetTypeRegistry.h"
#include "WidgetFactory.h"

namespace eversight {

WidgetTypeRegistry::WidgetTypeRegistry()
{
    using C = WidgetCategory;
    using F = WidgetFactory;

    m_descriptors = {
        F::createDescriptor("Image",               "Image",                "IMG", C::Foundation, 320,200, "#282c2f","#474b50","#dbe0e6", {{"dataSource",QString("")},{"showStatusBar",false},{"showToolsBar",true},{"keepLastResult",false},{"autoFill",false}}),
        F::createDescriptor("MultipleImages",      "Multiple Images",      "N",   C::Foundation, 400,220, "#282c2f","#474b50","#dbe0e6", {{"imageQuantity",2},{"autoFill",false}}),
        F::createDescriptor("RunControl",          "Run Control",          ">",   C::Foundation, 160, 72, "#1f3d60","#2d5a8a","#f3f9ff", {{"dataSource",QString("AllProcess")},{"controlType",QString("Both Display")}}),
        F::createDescriptor("RunStatus",           "Run Status",           "o",   C::Foundation, 120, 72, "#1a3528","#2e5c45","#dff0d8", {{"dataSource",QString("AllProcess")}}),
        F::createDescriptor("OKNG",                "OK / NG",              "OK",  C::Foundation, 120, 72, "#1d3e24","#2e5c35","#dff0d8", {{"dataSource",QString("")},{"okContent",QString("OK")},{"ngContent",QString("NG")}}),
        F::createDescriptor("Textbox",             "Textbox",              "T",   C::Foundation, 220, 48, "#2f3438","#52585f","#f5f5f5", {{"dataType",QString("string")},{"dataSource",QString("")},{"formatString",QString("")},{"text",QString("Label")},{"autoFill",false}}),
        F::createDescriptor("Button",              "Button",               "BTN", C::Foundation, 140, 56, "#ff8a00","#ffb15c","#ffffff", {{"showText",QString("Button")},{"triggerType",QString("Solution")}}),
        F::createDescriptor("SwitchControl",       "Switch Control",       "SW",  C::Foundation, 140, 56, "#3d5a7f","#5b7d9b","#f0f4f8", {{"dataSource",QString("")}}),
        F::createDescriptor("ParamsSettings",      "Params Settings",      "SET", C::Foundation, 220,140, "#2c3239","#515960","#e8ecf0", {{"parameterType",QString("float")},{"dataSource",QString("")},{"paramName",QString("Parameter")},{"decimalPlaces",3}}),
        F::createDescriptor("VariableSettings",    "Variable Settings",    "var", C::Foundation, 180, 80, "#253141","#4a5468","#d8e3f0", {{"dataSource",QString("")}}),
        F::createDescriptor("CharacterSettings",   "Character Settings",   "A",   C::Foundation, 180, 48, "#2f3134","#4a4e53","#f5f5f5", {{"dataSource",QString("")}}),
        F::createDescriptor("TrafficLight",        "Traffic Light",        "TL",  C::Foundation, 100,120, "#212529","#4e5b68","#e8ecf0", {{"dataSource",QString("")}}),
        F::createDescriptor("ConditionalLight",    "Conditional Light",    "IF",  C::Foundation, 180, 72, "#3b3222","#705d35","#f5e9c8", {{"parameterType",QString("float")},{"dataSource",QString("")},{"gtThreshold",0.0},{"ltThreshold",0.0}}),

        F::createDescriptor("GroupBox",            "Group Box",            "BOX", C::Layout,     360,220, "#2d3136","#5b5f65","#e8ecf0", {{"groupTitle",QString("Group")}}),
        F::createDescriptor("ChildInterface",      "Child Interface",      "CH",  C::Layout,     160, 56, "#2d3034","#4f5358","#e8ecf0", {{"windowName",QString("Sub Window")},{"passwordEnabled",false}}),
        F::createDescriptor("TabControl",          "Tab Control",          "TAB", C::Layout,     360,220, "#2b2f33","#54595f","#e8ecf0", {{"tabPageQuantity",2}}),

        F::createDescriptor("Table",               "Table",                "TBL", C::Chart,      340,180, "#32363a","#4b4f54","#e8ecf0", {{"rows",10},{"columns",4},{"lineTitle",QString("Result")}}),
        F::createDescriptor("LineDiagramArray",    "Line Diagram Array",   "LIN", C::Chart,      320,180, "#2b2f33","#4f5358","#e8ecf0", {{"latestRunTimes",50}}),
        F::createDescriptor("MultiLineDiagram",    "Multi Line Diagram",   "ML",  C::Chart,      340,180, "#2b2f33","#4f5358","#e8ecf0", {{"latestRunTimes",50}}),
        F::createDescriptor("ProductionStatistics","Production Statistics","STA", C::Chart,      300,160, "#2b2f33","#4f5358","#e8ecf0", {{"showMax",true},{"showMin",true},{"showAverage",true}}),
        F::createDescriptor("PieControl",          "Pie Control",          "PIE", C::Chart,      240,240, "#2b2f33","#4f5358","#e8ecf0", {}),
        F::createDescriptor("ParamSettingsArray",  "Param Settings Array", "PSA", C::Chart,      300,140, "#2c3239","#515960","#e8ecf0", {{"orientation",QString("Horizontal")},{"spacing",8}}),
        F::createDescriptor("LabelArray",          "Label Array",          "LA",  C::Chart,      300,140, "#2f3134","#4a4e53","#f5f5f5", {{"orientation",QString("Horizontal")},{"spacing",8}}),

        F::createDescriptor("StaticImage",         "Static Image",         "PIC", C::Other,      200,150, "#1e2124","#3a3e42","#e8ecf0", {{"stretchMode",QString("Uniform")}}),
        F::createDescriptor("WorldClock",          "World Clock",          "CLK", C::Other,      200, 48, "#1e2124","#3a3e42","#f5f5f5", {{"format",QString("HH:mm:ss")},{"timeZone",QString("UTC")}}),
        F::createDescriptor("Log",                 "Log",                  "LOG", C::Other,      400,200, "#1e2124","#3a3e42","#e8ecf0", {})
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
