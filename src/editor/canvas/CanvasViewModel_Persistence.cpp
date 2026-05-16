#include "CanvasViewModel.h"
#include "WidgetEngine.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QVariantMap>


#define m_widgets       m_widgetStore.widgets()
#define m_nextId        m_widgetStore.nextIdRef()
#define DESIGN_WIDTH    (this->canvasWidth())
#define EDITABLE_HEIGHT (this->canvasHeight() - WidgetEngine::WORKSPACE_STATUS_BAR_HEIGHT)

using namespace eversight;

static QJsonObject layoutNodeToJson(const LayoutNode& node)
{
    QJsonObject obj;
    obj["id"] = node.id;
    obj["parentId"] = node.parentId;
    obj["row"] = node.row;
    obj["column"] = node.column;
    obj["rowSpan"] = node.rowSpan;
    obj["columnSpan"] = node.columnSpan;
    obj["x"] = node.x;
    obj["y"] = node.y;
    obj["width"] = node.width;
    obj["height"] = node.height;
    obj["assignedWidgetId"] = node.assignedWidgetId;

    QJsonArray children;
    for (const LayoutNode& child : node.children)
        children.append(layoutNodeToJson(child));
    obj["children"] = children;
    return obj;
}

static QString localFilePath(const QString& filePath)
{
    const QUrl url(filePath);
    return url.isLocalFile() ? url.toLocalFile() : filePath;
}

static LayoutNode layoutNodeFromJson(const QJsonObject& obj)
{
    LayoutNode node;
    node.id = obj["id"].toInt(1);
    node.parentId = obj["parentId"].toInt(-1);
    node.row = obj["row"].toInt();
    node.column = obj["column"].toInt();
    node.rowSpan = obj["rowSpan"].toInt(1);
    node.columnSpan = obj["columnSpan"].toInt(1);
    node.x = obj["x"].toDouble();
    node.y = obj["y"].toDouble();
    node.width = obj["width"].toDouble(1.0);
    node.height = obj["height"].toDouble(1.0);
    node.assignedWidgetId = obj["assignedWidgetId"].toInt(-1);

    const QJsonArray children = obj["children"].toArray();
    for (const QJsonValue& childValue : children)
        node.children.append(layoutNodeFromJson(childValue.toObject()));
    return node;
}

static QJsonObject canvasLayoutToJson(const CanvasLayoutModel& layout)
{
    QJsonObject obj;
    obj["basicLayout"] = int(layout.basicLayout);
    obj["splitTemplate"] = layout.splitTemplate;
    obj["customRows"] = layout.customRows;
    obj["customColumns"] = layout.customColumns;
    obj["canvasWidth"] = layout.canvasWidth;
    obj["canvasHeight"] = layout.canvasHeight;
    obj["showGrid"] = layout.showGrid;
    obj["gridLineColor"] = layout.gridLineColor;
    obj["backgroundColor"] = layout.backgroundColor;
    obj["nextNodeId"] = layout.nextNodeId;
    obj["root"] = layoutNodeToJson(layout.root);
    return obj;
}

static CanvasLayoutModel canvasLayoutFromJson(const QJsonObject& obj, const QJsonObject& rootObj = {})
{
    CanvasLayoutModel layout;
    layout.basicLayout = static_cast<CanvasLayoutModel::BasicLayout>(obj["basicLayout"].toInt(CanvasLayoutModel::L1));
    layout.splitTemplate = obj["splitTemplate"].toInt();
    layout.customRows = qBound(1, obj["customRows"].toInt(1), 10);
    layout.customColumns = qBound(1, obj["customColumns"].toInt(1), 10);
    const qreal savedCanvasWidth = obj.contains("canvasWidth")
            ? obj["canvasWidth"].toDouble()
            : rootObj["canvasWidth"].toDouble(rootObj["designWidth"].toDouble(WidgetEngine::DEFAULT_DESIGN_WIDTH));
    const qreal savedCanvasHeight = obj.contains("canvasHeight")
            ? obj["canvasHeight"].toDouble()
            : rootObj["canvasHeight"].toDouble(rootObj["designHeight"].toDouble(WidgetEngine::DEFAULT_DESIGN_HEIGHT));
    layout.canvasWidth = qMax<qreal>(320.0, savedCanvasWidth);
    layout.canvasHeight = qMax<qreal>(240.0, savedCanvasHeight);
    layout.showGrid = obj["showGrid"].toBool(false);
    layout.gridLineColor = obj["gridLineColor"].toString("#e0e0e0");
    layout.backgroundColor = obj["backgroundColor"].toString("#ffffff");
    layout.nextNodeId = qMax(2, obj["nextNodeId"].toInt(2));
    if (obj.contains("root"))
        layout.root = layoutNodeFromJson(obj["root"].toObject());
    layout.selectedCellIds.clear();
    return layout;
}


bool CanvasViewModel::saveToFile(const QString& filePath) const
{
    QJsonArray widgets;
    for (const WidgetItem& w : m_widgets) {
        QJsonObject obj;
        obj["id"]          = w.id;
        obj["parentRegionId"] = w.parentRegionId;
        obj["type"]        = w.type;
        obj["title"]       = w.title;
        obj["x"]           = w.x;
        obj["y"]           = w.y;
        obj["width"]       = w.width;
        obj["height"]      = w.height;
        obj["dataSource"]  = w.dataSource;
        obj["controlType"] = w.controlType;
        obj["buttonColor"] = w.buttonColor;
        obj["borderColor"] = w.borderColor;
        obj["iconColor"]   = w.iconColor;
        obj["autoFill"]    = w.autoFill;
        obj["properties"]  = QJsonObject::fromVariantMap(w.properties);
        widgets.append(obj);
    }
    QJsonObject root;
    root["schema"]  = "EverSightDesigner.Layout.v1";
    root["nextId"]  = m_widgetStore.nextId();
    root["widgets"] = widgets;
    if (const CanvasLayoutModel* layout = activeLayout()) {
        root["canvasWidth"] = layout->canvasWidth;
        root["canvasHeight"] = layout->canvasHeight;
        root["layout"] = canvasLayoutToJson(*layout);
    }
    QFile file(localFilePath(filePath));
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) return false;
    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

bool CanvasViewModel::loadFromFile(const QString& filePath)
{
    QFile file(localFilePath(filePath));
    if (!file.open(QIODevice::ReadOnly)) return false;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) return false;
    const QJsonObject root    = doc.object();
    const QJsonArray  widgets = root["widgets"].toArray();
    QList<WidgetItem> loaded;
    int maxId = 0;
    for (const QJsonValue& val : widgets) {
        const QJsonObject obj = val.toObject();
        WidgetItem w;
        w.id          = obj["id"].toInt();
        w.parentRegionId = obj["parentRegionId"].toInt(defaultLeafRegionId());
        w.type        = obj["type"].toString();
        w.title       = obj["title"].toString(w.type);
        w.x           = obj["x"].toDouble();
        w.y           = obj["y"].toDouble();
        w.width       = obj["width"].toDouble(160);
        w.height      = obj["height"].toDouble(92);
        w.dataSource  = obj["dataSource"].toString("AllProcess");
        w.controlType = obj["controlType"].toString("Both Display");
        w.buttonColor = obj["buttonColor"].toString("#2f2f2f");
        w.borderColor = obj["borderColor"].toString("#272727");
        w.iconColor   = obj["iconColor"].toString("#f6f6f6");
        w.autoFill    = obj["autoFill"].toBool(false);
        w.properties  = obj["properties"].toObject().toVariantMap();
        if (w.id <= 0 || w.type.isEmpty()) continue;
        maxId = qMax(maxId, w.id);
        loaded.append(w);
    }
    beginResetModel();
    m_widgets          = loaded;
    m_nextId           = qMax(root["nextId"].toInt(maxId + 1), maxId + 1);
    m_selectedWidgetId = -1;
    m_selectedWidgetIds.clear();
    if (root.contains("layout")) {
        if (m_tabs.isEmpty()) {
            CanvasTabModel tab;
            tab.id = 1;
            tab.title = QStringLiteral("Tab 1");
            m_tabs.append(tab);
            m_activeTab = 0;
        }
        m_tabs[m_activeTab].layout = canvasLayoutFromJson(root["layout"].toObject(), root);
    } else if (root.contains("canvasWidth") || root.contains("canvasHeight")
               || root.contains("designWidth") || root.contains("designHeight")) {
        CanvasLayoutModel* layout = activeLayout();
        if (layout) {
            layout->canvasWidth = qMax<qreal>(320.0, root["canvasWidth"].toDouble(root["designWidth"].toDouble(WidgetEngine::DEFAULT_DESIGN_WIDTH)));
            layout->canvasHeight = qMax<qreal>(240.0, root["canvasHeight"].toDouble(root["designHeight"].toDouble(WidgetEngine::DEFAULT_DESIGN_HEIGHT)));
        }
    }
    // Rebuild shadow screen model to mirror loaded widgets
    m_screen.canvas.widgets.clear();
    for (const WidgetItem &w : qAsConst(m_widgets))
        m_screen.canvas.widgets.append(eversight::toWidgetModel(w));
    endResetModel();
    emit widgetCountChanged();
    emit selectedWidgetChanged();
    emit canvasSizeChanged();
    emit layoutChanged();
    m_undoStack.clear();
    m_redoStack.clear();
    emit undoRedoChanged();
    return true;
}


#undef EDITABLE_HEIGHT
#undef DESIGN_WIDTH
#undef m_nextId
#undef m_widgets
