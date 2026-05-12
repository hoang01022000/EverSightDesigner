#include "CanvasViewModel.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

CanvasViewModel::CanvasViewModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int CanvasViewModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return m_widgets.size();
}

QVariant CanvasViewModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return {};

    const WidgetItem& widget = m_widgets.at(index.row());

    switch (role) {

    case IdRole:
        return widget.id;

    case TypeRole:
        return widget.type;

    case XRole:
        return widget.x;

    case YRole:
        return widget.y;

    case WidthRole:
        return widget.width;

    case HeightRole:
        return widget.height;

    case TitleRole:
        return widget.title;

    case DataSourceRole:
        return widget.dataSource;

    case ControlTypeRole:
        return widget.controlType;

    case ButtonColorRole:
        return widget.buttonColor;

    case BorderColorRole:
        return widget.borderColor;

    case IconColorRole:
        return widget.iconColor;

    case AutoFillRole:
        return widget.autoFill;
    }

    return {};
}

QHash<int, QByteArray> CanvasViewModel::roleNames() const
{
    return {
        { IdRole, "widgetId" },
        { TypeRole, "widgetType" },
        { XRole, "widgetX" },
        { YRole, "widgetY" },
        { WidthRole, "widgetWidth" },
        { HeightRole, "widgetHeight" },
        { TitleRole, "widgetTitle" },
        { DataSourceRole, "widgetDataSource" },
        { ControlTypeRole, "widgetControlType" },
        { ButtonColorRole, "widgetButtonColor" },
        { BorderColorRole, "widgetBorderColor" },
        { IconColorRole, "widgetIconColor" },
        { AutoFillRole, "widgetAutoFill" }
    };
}

WidgetItem CanvasViewModel::createWidget(const QString& type) const
{
    WidgetItem item;
    item.type = type;
    item.id = -1;

    const QString formattedType = type.trimmed();

    if (formattedType == "CameraView") {
        item.width = 320;
        item.height = 200;
        item.title = "Camera View";
        item.buttonColor = "#282c2f";
        item.borderColor = "#474b50";
        item.iconColor = "#dbe0e6";
    } else if (formattedType == "TrendChart") {
        item.width = 320;
        item.height = 180;
        item.title = "Trend Chart";
        item.buttonColor = "#2b2f33";
        item.borderColor = "#4f5358";
    } else if (formattedType == "MultiTrend") {
        item.width = 340;
        item.height = 180;
        item.title = "Multi Trend";
        item.buttonColor = "#2b2f33";
        item.borderColor = "#4f5358";
    } else if (formattedType == "DataTable") {
        item.width = 340;
        item.height = 180;
        item.title = "Data Table";
        item.buttonColor = "#32363a";
        item.borderColor = "#4b4f54";
    } else if (formattedType == "RunControl") {
        item.width = 160;
        item.height = 72;
        item.title = "Run Control";
        item.buttonColor = "#1f3d60";
        item.iconColor = "#f3f9ff";
    } else if (formattedType == "Button") {
        item.width = 140;
        item.height = 56;
        item.title = "Button";
        item.buttonColor = "#ff8a00";
        item.borderColor = "#ffb15c";
    } else if (formattedType == "Toggle") {
        item.width = 140;
        item.height = 56;
        item.title = "Toggle";
        item.buttonColor = "#3d5a7f";
        item.borderColor = "#5b7d9b";
    } else if (formattedType == "TextInput") {
        item.width = 220;
        item.height = 48;
        item.title = "Text Input";
        item.buttonColor = "#2f3438";
        item.borderColor = "#52585f";
    } else if (formattedType == "Indicator") {
        item.width = 120;
        item.height = 72;
        item.title = "Indicator";
        item.buttonColor = "#1d3e24";
        item.iconColor = "#dff0d8";
    } else if (formattedType == "Label") {
        item.width = 180;
        item.height = 48;
        item.title = "Text Label";
        item.buttonColor = "#2f3134";
        item.iconColor = "#f5f5f5";
    } else if (formattedType == "TrafficLight") {
        item.width = 100;
        item.height = 120;
        item.title = "Traffic Light";
        item.buttonColor = "#212529";
        item.borderColor = "#4e5b68";
    } else if (formattedType == "GroupBox") {
        item.width = 360;
        item.height = 220;
        item.title = "Group Box";
        item.buttonColor = "#2d3136";
        item.borderColor = "#5b5f65";
    } else if (formattedType == "TabContainer") {
        item.width = 360;
        item.height = 220;
        item.title = "Tab Container";
        item.buttonColor = "#2b2f33";
        item.borderColor = "#54595f";
    } else if (formattedType == "ChildInterface") {
        item.width = 360;
        item.height = 240;
        item.title = "Child Interface";
        item.buttonColor = "#2d3034";
        item.borderColor = "#4f5358";
    } else if (formattedType == "NumericReadout") {
        item.width = 180;
        item.height = 80;
        item.title = "Numeric Readout";
        item.buttonColor = "#253141";
        item.borderColor = "#4a5468";
    } else if (formattedType == "ParameterSet") {
        item.width = 220;
        item.height = 140;
        item.title = "Parameters";
        item.buttonColor = "#2c3239";
        item.borderColor = "#515960";
    } else if (formattedType == "Condition") {
        item.width = 180;
        item.height = 72;
        item.title = "Condition";
        item.buttonColor = "#3b3222";
        item.borderColor = "#705d35";
    } else {
        item.width = 160;
        item.height = 92;
        item.title = formattedType.isEmpty() ? "Widget" : formattedType;
        item.buttonColor = "#2f2f2f";
        item.borderColor = "#272727";
    }

    return item;
}

void CanvasViewModel::addWidget(const QString& type)
{
    addWidgetAt(type, 160 + (m_widgets.size() % 6) * 24, 120 + (m_widgets.size() % 6) * 24);
}

void CanvasViewModel::addWidgetAt(const QString& type, qreal x, qreal y)
{
    beginInsertRows(QModelIndex(), m_widgets.size(), m_widgets.size());

    WidgetItem item = createWidget(type);
    item.id = m_nextId++;
    item.x = x;
    item.y = y;

    m_widgets.append(item);
    endInsertRows();

    emit widgetCountChanged();
    selectWidget(item.id);
}

void CanvasViewModel::selectWidget(int id)
{
    if (m_selectedWidgetId == id)
        return;

    m_selectedWidgetId = id;
    emit selectedWidgetChanged();
}

void CanvasViewModel::clearSelection()
{
    selectWidget(-1);
}

void CanvasViewModel::removeSelectedWidget()
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0)
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_widgets.removeAt(row);
    endRemoveRows();
    emit widgetCountChanged();

    m_selectedWidgetId = -1;
    emit selectedWidgetChanged();
}

void CanvasViewModel::duplicateSelectedWidget()
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0)
        return;

    WidgetItem item = m_widgets.at(row);
    item.id = m_nextId++;
    item.title += " Copy";
    item.x += 24;
    item.y += 24;

    beginInsertRows(QModelIndex(), m_widgets.size(), m_widgets.size());
    m_widgets.append(item);
    endInsertRows();

    emit widgetCountChanged();
    selectWidget(item.id);
}

void CanvasViewModel::moveSelectedForward()
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0 || row >= m_widgets.size() - 1)
        return;

    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 2);
    m_widgets.move(row, row + 1);
    endMoveRows();
}

void CanvasViewModel::moveSelectedBackward()
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row <= 0)
        return;

    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row - 1);
    m_widgets.move(row, row - 1);
    endMoveRows();
}

void CanvasViewModel::alignSelected(const QString& mode, qreal canvasWidth, qreal canvasHeight)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0)
        return;

    WidgetItem& widget = m_widgets[row];

    if (mode == "left") {
        widget.x = 0;
    } else if (mode == "hcenter") {
        widget.x = (canvasWidth - widget.width) / 2.0;
    } else if (mode == "right") {
        widget.x = canvasWidth - widget.width;
    } else if (mode == "top") {
        widget.y = 0;
    } else if (mode == "vcenter") {
        widget.y = (canvasHeight - widget.height) / 2.0;
    } else if (mode == "bottom") {
        widget.y = canvasHeight - widget.height;
    }

    const QModelIndex modelIndex = index(row, 0);
    emit dataChanged(modelIndex, modelIndex, { XRole, YRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateWidgetGeometry(int id, qreal x, qreal y, qreal width, qreal height)
{
    const int row = indexOfWidget(id);
    if (row < 0)
        return;

    WidgetItem& widget = m_widgets[row];
    widget.x = x;
    widget.y = y;
    widget.width = qMax<qreal>(24, width);
    widget.height = qMax<qreal>(24, height);

    const QModelIndex modelIndex = index(row, 0);
    emit dataChanged(modelIndex, modelIndex, { XRole, YRole, WidthRole, HeightRole });

    if (m_selectedWidgetId == id)
        emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedTitle(const QString& title)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0)
        return;

    m_widgets[row].title = title;

    const QModelIndex modelIndex = index(row, 0);
    emit dataChanged(modelIndex, modelIndex, { TitleRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedData(const QString& dataSource, const QString& controlType)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0)
        return;

    m_widgets[row].dataSource = dataSource;
    m_widgets[row].controlType = controlType;

    const QModelIndex modelIndex = index(row, 0);
    emit dataChanged(modelIndex, modelIndex, { DataSourceRole, ControlTypeRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedAppearance(const QString& buttonColor, const QString& borderColor, const QString& iconColor)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0)
        return;

    m_widgets[row].buttonColor = buttonColor;
    m_widgets[row].borderColor = borderColor;
    m_widgets[row].iconColor = iconColor;

    const QModelIndex modelIndex = index(row, 0);
    emit dataChanged(modelIndex, modelIndex, { ButtonColorRole, BorderColorRole, IconColorRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedAutoFill(bool autoFill)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0)
        return;

    m_widgets[row].autoFill = autoFill;

    const QModelIndex modelIndex = index(row, 0);
    emit dataChanged(modelIndex, modelIndex, { AutoFillRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::clear()
{
    if (m_widgets.isEmpty())
        return;

    beginResetModel();
    m_widgets.clear();
    m_selectedWidgetId = -1;
    endResetModel();
    emit widgetCountChanged();
    emit selectedWidgetChanged();
}

bool CanvasViewModel::saveToFile(const QString& filePath) const
{
    QJsonArray widgets;

    for (const WidgetItem& widget : m_widgets) {
        QJsonObject object;
        object["id"] = widget.id;
        object["type"] = widget.type;
        object["title"] = widget.title;
        object["x"] = widget.x;
        object["y"] = widget.y;
        object["width"] = widget.width;
        object["height"] = widget.height;
        object["dataSource"] = widget.dataSource;
        object["controlType"] = widget.controlType;
        object["buttonColor"] = widget.buttonColor;
        object["borderColor"] = widget.borderColor;
        object["iconColor"] = widget.iconColor;
        object["autoFill"] = widget.autoFill;
        widgets.append(object);
    }

    QJsonObject root;
    root["schema"] = "EverSightDesigner.Layout.v1";
    root["nextId"] = m_nextId;
    root["widgets"] = widgets;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    file.write(QJsonDocument(root).toJson(QJsonDocument::Indented));
    return true;
}

bool CanvasViewModel::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    const QJsonDocument document = QJsonDocument::fromJson(file.readAll());
    if (!document.isObject())
        return false;

    const QJsonObject root = document.object();
    const QJsonArray widgets = root["widgets"].toArray();

    QList<WidgetItem> loadedWidgets;
    int maxId = 0;

    for (const QJsonValue& value : widgets) {
        const QJsonObject object = value.toObject();

        WidgetItem widget;
        widget.id = object["id"].toInt();
        widget.type = object["type"].toString();
        widget.title = object["title"].toString(widget.type);
        widget.x = object["x"].toDouble();
        widget.y = object["y"].toDouble();
        widget.width = object["width"].toDouble(160);
        widget.height = object["height"].toDouble(92);
        widget.dataSource = object["dataSource"].toString("AllProcess");
        widget.controlType = object["controlType"].toString("Both Display");
        widget.buttonColor = object["buttonColor"].toString("#2f2f2f");
        widget.borderColor = object["borderColor"].toString("#272727");
        widget.iconColor = object["iconColor"].toString("#f6f6f6");
        widget.autoFill = object["autoFill"].toBool(false);

        if (widget.id <= 0 || widget.type.isEmpty())
            continue;

        maxId = qMax(maxId, widget.id);
        loadedWidgets.append(widget);
    }

    beginResetModel();
    m_widgets = loadedWidgets;
    m_nextId = qMax(root["nextId"].toInt(maxId + 1), maxId + 1);
    m_selectedWidgetId = -1;
    endResetModel();

    emit widgetCountChanged();
    emit selectedWidgetChanged();
    return true;
}

int CanvasViewModel::selectedWidgetId() const
{
    return m_selectedWidgetId;
}

QString CanvasViewModel::selectedWidgetType() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->type : QString();
}

QString CanvasViewModel::selectedWidgetTitle() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->title : QString();
}

qreal CanvasViewModel::selectedWidgetX() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->x : 0;
}

qreal CanvasViewModel::selectedWidgetY() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->y : 0;
}

qreal CanvasViewModel::selectedWidgetWidth() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->width : 0;
}

qreal CanvasViewModel::selectedWidgetHeight() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->height : 0;
}

QString CanvasViewModel::selectedDataSource() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->dataSource : QString();
}

QString CanvasViewModel::selectedControlType() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->controlType : QString();
}

QString CanvasViewModel::selectedButtonColor() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->buttonColor : QString();
}

QString CanvasViewModel::selectedBorderColor() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->borderColor : QString();
}

QString CanvasViewModel::selectedIconColor() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->iconColor : QString();
}

bool CanvasViewModel::selectedAutoFill() const
{
    const WidgetItem* widget = selectedWidget();
    return widget ? widget->autoFill : false;
}

bool CanvasViewModel::hasSelection() const
{
    return selectedWidget() != nullptr;
}

int CanvasViewModel::widgetCount() const
{
    return m_widgets.size();
}

int CanvasViewModel::indexOfWidget(int id) const
{
    for (int i = 0; i < m_widgets.size(); ++i) {
        if (m_widgets.at(i).id == id)
            return i;
    }

    return -1;
}

const WidgetItem* CanvasViewModel::selectedWidget() const
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0)
        return nullptr;

    return &m_widgets.at(row);
}
