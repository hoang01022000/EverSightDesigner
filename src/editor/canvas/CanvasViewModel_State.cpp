#include "CanvasViewModel.h"
#include "EditorController.h"


#define m_widgets       m_widgetStore.widgets()
#define m_nextId        m_widgetStore.nextIdRef()
#define DESIGN_WIDTH    (this->canvasWidth())
#define EDITABLE_HEIGHT (this->canvasHeight() - WidgetEngine::WORKSPACE_STATUS_BAR_HEIGHT)

using namespace eversight;

bool    CanvasViewModel::hasSelection()         const { return selectedWidget() != nullptr; }

int     CanvasViewModel::selectedCount()        const { return m_selectedWidgetIds.size(); }

bool    CanvasViewModel::canUndo()              const { return !m_undoStack.isEmpty(); }

bool    CanvasViewModel::canRedo()              const { return !m_redoStack.isEmpty(); }

bool    CanvasViewModel::previewMode()          const { return m_previewManager.previewMode(); }

CanvasViewModel::StateSnapshot CanvasViewModel::snapshot() const
{
    StateSnapshot state;
    state.widgets = m_widgets;
    state.selectedWidgetIds = m_selectedWidgetIds;
    state.primarySelectedWidgetId = m_selectedWidgetId;
    state.nextId = m_widgetStore.nextId();
    state.tabs = m_tabs;
    state.activeTab = m_activeTab;
    return state;
}

void CanvasViewModel::restoreSnapshot(const StateSnapshot& state)
{
    beginResetModel();
    m_widgets = state.widgets;
    m_selectedWidgetIds = state.selectedWidgetIds;
    m_selectedWidgetId = state.primarySelectedWidgetId;
    m_nextId = state.nextId;
    m_tabs = state.tabs;
    m_activeTab = state.activeTab;
    syncScreenFromWidgets();
    endResetModel();

    emit widgetCountChanged();
    emit selectedWidgetChanged();
    emit canvasSizeChanged();
    emit layoutChanged();
}

void CanvasViewModel::pushUndoState()
{
    m_undoStack.append(snapshot());
    if (m_undoStack.size() > 100)
        m_undoStack.removeFirst();
    m_redoStack.clear();
    emit undoRedoChanged();
}

void CanvasViewModel::syncScreenFromWidgets()
{
    m_screen.canvas.widgets.clear();
    for (const WidgetItem& w : qAsConst(m_widgets))
        m_screen.canvas.widgets.append(eversight::toWidgetModel(w));
}

void CanvasViewModel::emitAllWidgetDataChanged()
{
    if (m_widgets.isEmpty())
        return;

    emit dataChanged(index(0, 0), index(m_widgets.size() - 1, 0),
                     { ParentRegionIdRole, XRole, YRole, WidthRole, HeightRole, SelectedRole });
}

void CanvasViewModel::setSelection(const QList<int>& ids, int primaryId)
{
    const QString oldMode = inspectorMode();
    QList<int> existingIds;
    for (const WidgetItem& widget : qAsConst(m_widgets))
        existingIds.append(widget.id);

    const QList<int> normalized = EditorController::normalizedSelection(ids, existingIds);
    primaryId = EditorController::primarySelection(normalized, primaryId);

    if (m_selectedWidgetIds == normalized && m_selectedWidgetId == primaryId)
        return;

    m_selectedWidgetIds = normalized;
    m_selectedWidgetId = normalized.isEmpty() ? -1 : primaryId;
    if (!normalized.isEmpty()) {
        CanvasLayoutModel* layout = activeLayout();
        if (layout)
            layout->selectedCellIds.clear();
    }
    emitAllWidgetDataChanged();
    emit selectedWidgetChanged();
    emit layoutChanged();
    if (oldMode != inspectorMode())
        emit inspectorModeChanged();
}


#undef EDITABLE_HEIGHT
#undef DESIGN_WIDTH
#undef m_nextId
#undef m_widgets
