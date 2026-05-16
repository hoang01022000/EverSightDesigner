#include "CanvasViewModel.h"

#define m_widgets       m_widgetStore.widgets()
#define m_nextId        m_widgetStore.nextIdRef()
#define DESIGN_WIDTH    (this->canvasWidth())
#define EDITABLE_HEIGHT (this->canvasHeight() - WidgetEngine::WORKSPACE_STATUS_BAR_HEIGHT)

using namespace eversight;

CanvasViewModel::CanvasViewModel(QObject* parent)
    : QAbstractListModel(parent)
{
    connect(&m_previewManager, &PreviewManager::previewModeChanged, this, [this]() {
        emit previewModeChanged();
        emitAllWidgetDataChanged();
        emit layoutChanged();
    });

    CanvasTabModel tab;
    tab.id = 1;
    tab.title = QStringLiteral("Tab 1");
    m_tabs.append(tab);
}

void CanvasViewModel::addTab()
{
    CanvasTabModel t;
    t.id = m_tabs.size() + 1;
    t.title = QString("Tab %1").arg(t.id);
    beginResetModel();
    m_tabs.append(t);
    endResetModel();
    emit tabsChanged();
    emit layoutChanged();
}

void CanvasViewModel::removeTab(int index)
{
    if (index < 0 || index >= m_tabs.size()) return;
    beginResetModel();
    m_tabs.removeAt(index);
    if (m_activeTab >= m_tabs.size()) m_activeTab = qMax(0, m_tabs.size() - 1);
    endResetModel();
    emit tabsChanged();
    emit activeTabChanged();
}

void CanvasViewModel::renameTab(int index, const QString& title)
{
    if (index < 0 || index >= m_tabs.size()) return;
    m_tabs[index].title = title;
    emit tabsChanged();
}

void CanvasViewModel::setActiveTab(int index)
{
    if (index < 0 || index >= m_tabs.size()) return;
    if (m_activeTab == index) return;
    m_activeTab = index;
    emit activeTabChanged();
    emit canvasSizeChanged();
}

int CanvasViewModel::activeTabIndex() const { return m_activeTab; }

int CanvasViewModel::tabCount() const { return m_tabs.size(); }

void CanvasViewModel::setFixedBar(const QString& bar, bool visible)
{
    const QString b = bar.toLower();
    if (b == "top") m_fixedBars.topVisible = visible;
    else if (b == "bottom") m_fixedBars.bottomVisible = visible;
    else if (b == "left") m_fixedBars.leftVisible = visible;
    else if (b == "right") m_fixedBars.rightVisible = visible;
    emit fixedBarChanged();
}

bool CanvasViewModel::isTopFixedVisible() const { return m_fixedBars.topVisible; }

bool CanvasViewModel::isBottomFixedVisible() const { return m_fixedBars.bottomVisible; }

bool CanvasViewModel::isLeftFixedVisible() const { return m_fixedBars.leftVisible; }

bool CanvasViewModel::isRightFixedVisible() const { return m_fixedBars.rightVisible; }


#undef EDITABLE_HEIGHT
#undef DESIGN_WIDTH
#undef m_nextId
#undef m_widgets
