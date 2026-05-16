#include "CanvasViewModel.h"
#include "../../editor/registry/WidgetTypeRegistry.h"
#include "EditorController.h"
#include "WidgetEngine.h"

#include <QVariantMap>


#define m_widgets       m_widgetStore.widgets()
#define m_nextId        m_widgetStore.nextIdRef()
#define DESIGN_WIDTH    (this->canvasWidth())
#define EDITABLE_HEIGHT (this->canvasHeight() - WidgetEngine::WORKSPACE_STATUS_BAR_HEIGHT)

using namespace eversight;

void CanvasViewModel::updateSelectedTitle(const QString& title)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].title == title) return;
    pushUndoState();
    m_widgets[row].title = title;
    m_widgets[row].properties.insert("text", title);
    emit dataChanged(index(row,0), index(row,0), { TitleRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedData(const QString& dataSource, const QString& controlType)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].dataSource == dataSource && m_widgets[row].controlType == controlType) return;
    pushUndoState();
    m_widgets[row].dataSource  = dataSource;
    m_widgets[row].controlType = controlType;
    m_widgets[row].properties.insert("dataSource", dataSource);
    m_widgets[row].properties.insert("controlType", controlType);
    emit dataChanged(index(row,0), index(row,0), { DataSourceRole, ControlTypeRole });
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedAppearance(const QString& buttonColor, const QString& borderColor, const QString& iconColor)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].buttonColor == buttonColor
            && m_widgets[row].borderColor == borderColor
            && m_widgets[row].iconColor == iconColor)
        return;
    pushUndoState();
    m_widgets[row].buttonColor = buttonColor;
    m_widgets[row].borderColor = borderColor;
    m_widgets[row].iconColor   = iconColor;
    m_widgets[row].properties.insert("background", buttonColor);
    m_widgets[row].properties.insert("border", borderColor);
    m_widgets[row].properties.insert("iconColor", iconColor);
    emit dataChanged(index(row,0), index(row,0), { ButtonColorRole, BorderColorRole, IconColorRole });
    emit selectedWidgetChanged();
}

QString CanvasViewModel::selectedAppearanceValue(const QString& key) const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return {};

    if (key == QStringLiteral("background") || key == QStringLiteral("buttonColor"))
        return w->buttonColor;
    if (key == QStringLiteral("border") || key == QStringLiteral("borderColor"))
        return w->borderColor;
    if (key == QStringLiteral("font") || key == QStringLiteral("iconColor") || key == QStringLiteral("textColor"))
        return w->iconColor;
    return w->properties.value(key).toString();
}

void CanvasViewModel::updateSelectedAppearanceField(const QString& key, const QString& value)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0 || key.isEmpty())
        return;

    WidgetItem& widget = m_widgets[row];
    QList<int> roles;
    bool changed = false;

    if (key == QStringLiteral("background") || key == QStringLiteral("buttonColor")) {
        if (widget.buttonColor == value)
            return;
        pushUndoState();
        widget.buttonColor = value;
        widget.properties.insert(QStringLiteral("background"), value);
        roles.append(ButtonColorRole);
        changed = true;
    } else if (key == QStringLiteral("border") || key == QStringLiteral("borderColor")) {
        if (widget.borderColor == value)
            return;
        pushUndoState();
        widget.borderColor = value;
        widget.properties.insert(QStringLiteral("border"), value);
        roles.append(BorderColorRole);
        changed = true;
    } else if (key == QStringLiteral("font") || key == QStringLiteral("iconColor") || key == QStringLiteral("textColor")) {
        if (widget.iconColor == value)
            return;
        pushUndoState();
        widget.iconColor = value;
        widget.properties.insert(QStringLiteral("iconColor"), value);
        roles.append(IconColorRole);
        changed = true;
    } else {
        if (widget.properties.value(key).toString() == value)
            return;
        pushUndoState();
        widget.properties.insert(key, value);
        changed = true;
    }

    if (!changed)
        return;

    if (!roles.isEmpty())
        emit dataChanged(index(row, 0), index(row, 0), roles);
    emit selectedWidgetChanged();
}

void CanvasViewModel::updateSelectedAutoFill(bool autoFill)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0) return;
    if (m_widgets[row].autoFill == autoFill) return;
    pushUndoState();
    m_widgets[row].autoFill = autoFill;
    m_widgets[row].properties.insert("autoFill", autoFill);
    emit dataChanged(index(row,0), index(row,0), { AutoFillRole });
    emit selectedWidgetChanged();
}

QVariant CanvasViewModel::selectedPropertyValue(const QString& key) const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return {};

    if (key == "dataSource") return w->dataSource;
    if (key == "controlType") return w->controlType;
    if (key == "iconColor") return w->iconColor;
    if (key == "background" || key == "buttonColor") return w->buttonColor;
    if (key == "border" || key == "borderColor") return w->borderColor;
    if (key == "autoFill") return w->autoFill;
    if (key == "text") return w->title;

    return w->properties.value(key);
}

void CanvasViewModel::updateSelectedProperty(const QString& key, const QVariant& value)
{
    const int row = indexOfWidget(m_selectedWidgetId);
    if (row < 0 || key.isEmpty()) return;

    WidgetItem& w = m_widgets[row];
    if (selectedPropertyValue(key) == value) return;

    pushUndoState();

    if (key == "dataSource") {
        w.dataSource = value.toString();
    } else if (key == "controlType") {
        w.controlType = value.toString();
    } else if (key == "iconColor") {
        w.iconColor = value.toString();
    } else if (key == "background" || key == "buttonColor") {
        w.buttonColor = value.toString();
    } else if (key == "border" || key == "borderColor") {
        w.borderColor = value.toString();
    } else if (key == "autoFill") {
        w.autoFill = value.toBool();
    } else if (key == "text") {
        w.title = value.toString();
    }

    w.properties.insert(key, value);
    syncScreenFromWidgets();

    const QModelIndex mi = index(row, 0);
    emit dataChanged(mi, mi, { TitleRole, DataSourceRole, ControlTypeRole,
                               ButtonColorRole, BorderColorRole, IconColorRole,
                               AutoFillRole });
    emit selectedWidgetChanged();
}

int     CanvasViewModel::selectedWidgetId()     const { return m_selectedWidgetId; }

QString CanvasViewModel::selectedWidgetType()   const { auto* w = selectedWidget(); return w ? w->type        : QString(); }

QString CanvasViewModel::selectedWidgetTitle()  const { auto* w = selectedWidget(); return w ? w->title       : QString(); }

qreal   CanvasViewModel::selectedWidgetX()      const { auto* w = selectedWidget(); return w ? w->x           : 0; }

qreal   CanvasViewModel::selectedWidgetY()      const { auto* w = selectedWidget(); return w ? w->y           : 0; }

qreal   CanvasViewModel::selectedWidgetWidth()  const { auto* w = selectedWidget(); return w ? w->width       : 0; }

qreal   CanvasViewModel::selectedWidgetHeight() const { auto* w = selectedWidget(); return w ? w->height      : 0; }

QString CanvasViewModel::selectedDataSource()   const { auto* w = selectedWidget(); return w ? w->dataSource  : QString(); }

QString CanvasViewModel::selectedControlType()  const { auto* w = selectedWidget(); return w ? w->controlType : QString(); }

QString CanvasViewModel::selectedButtonColor()  const { auto* w = selectedWidget(); return w ? w->buttonColor : QString(); }

QString CanvasViewModel::selectedBorderColor()  const { auto* w = selectedWidget(); return w ? w->borderColor : QString(); }

QString CanvasViewModel::selectedIconColor()    const { auto* w = selectedWidget(); return w ? w->iconColor   : QString(); }

bool    CanvasViewModel::selectedAutoFill()     const { auto* w = selectedWidget(); return w ? w->autoFill    : false; }

QVariantList CanvasViewModel::selectedPropertyDefinitions() const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return {};

    const WidgetTypeDescriptor* desc = WidgetTypeRegistry::instance().descriptor(w->type);
    return desc ? desc->propertyDefinitions : QVariantList{};
}

bool CanvasViewModel::selectedWidgetHasDataSource() const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return false;

    const WidgetTypeDescriptor* desc = WidgetTypeRegistry::instance().descriptor(w->type);
    return desc ? desc->hasDataSource : false;
}

QVariantList CanvasViewModel::selectedAppearanceFields() const
{
    const WidgetItem* w = selectedWidget();
    if (!w) return {};

    const WidgetTypeDescriptor* desc = WidgetTypeRegistry::instance().descriptor(w->type);
    if (!desc)
        return {};

    QVariantList fields;
    for (const QVariant& value : desc->appearanceFields) {
        QVariantMap field = value.toMap();
        const QString key = field.value(QStringLiteral("key")).toString();
        field.insert(QStringLiteral("value"), selectedAppearanceValue(key));
        fields.append(field);
    }
    return fields;
}

qreal CanvasViewModel::canvasWidth() const
{
    const CanvasLayoutModel* layout = activeLayout();
    return layout ? layout->canvasWidth : WidgetEngine::DEFAULT_DESIGN_WIDTH;
}

qreal CanvasViewModel::canvasHeight() const
{
    const CanvasLayoutModel* layout = activeLayout();
    return layout ? layout->canvasHeight : WidgetEngine::DEFAULT_DESIGN_HEIGHT;
}

void CanvasViewModel::setCanvasSize(qreal width, qreal height)
{
    CanvasLayoutModel* layout = activeLayout();
    if (!layout)
        return;

    width = qMax<qreal>(320.0, width);
    height = qMax<qreal>(240.0, height);
    if (qFuzzyCompare(layout->canvasWidth, width) && qFuzzyCompare(layout->canvasHeight, height))
        return;

    pushUndoState();
    layout->canvasWidth = width;
    layout->canvasHeight = height;
    clampWidgetsToLayoutBounds();
    emit canvasSizeChanged();
    emit layoutChanged();
}

QString CanvasViewModel::inspectorMode() const
{
    return EditorController::inspectorMode(hasSelection());
}

void CanvasViewModel::enterPreview()
{
    m_previewManager.enterPreview();
}

void CanvasViewModel::exitPreview()
{
    m_previewManager.exitPreview();
}


#undef EDITABLE_HEIGHT
#undef DESIGN_WIDTH
#undef m_nextId
#undef m_widgets
