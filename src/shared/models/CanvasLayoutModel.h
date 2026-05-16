#pragma once

#include <QList>
#include <QString>
#include <QtGlobal>

namespace eversight {

struct LayoutNode
{
    int id = 1;
    int parentId = -1;
    int row = 0;
    int column = 0;
    int rowSpan = 1;
    int columnSpan = 1;
    qreal x = 0.0;
    qreal y = 0.0;
    qreal width = 1.0;
    qreal height = 1.0;
    int assignedWidgetId = -1;
    QList<LayoutNode> children;

    bool isLeaf() const { return children.isEmpty(); }
};

struct CanvasLayoutModel
{
    enum BasicLayout {
        Custom = 0,
        L1 = 1,
        L2Horizontal,
        L2Vertical,
        L3Horizontal,
        L3Vertical,
        L4,
        L6Wide,
        L6Tall,
        L9
    } basicLayout = L1;

    int splitTemplate = 0; // 0..7
    int customRows = 1;
    int customColumns = 1;
    qreal canvasWidth = 1280.0;
    qreal canvasHeight = 720.0;
    bool showGrid = false;
    QString gridLineColor = "#e0e0e0";
    QString backgroundColor = "#ffffff";
    LayoutNode root;
    QList<int> selectedCellIds;
    int nextNodeId = 2;
};

} // namespace eversight
