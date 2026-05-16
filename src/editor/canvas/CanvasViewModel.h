#pragma once

#include <QAbstractListModel>
#include <QList>
#include <QVariantList>
#include "../../shared/models/WidgetItem.h"
#include "../../shared/models/ScreenModel.h"
#include "../../shared/models/CanvasTabModel.h"
#include "../../shared/models/FixedBarState.h"
#include "PreviewManager.h"
#include "UndoStack.h"
#include "WidgetStore.h"

class CanvasViewModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(int     selectedWidgetId     READ selectedWidgetId     NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedWidgetType   READ selectedWidgetType   NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedWidgetTitle  READ selectedWidgetTitle  NOTIFY selectedWidgetChanged)
    Q_PROPERTY(qreal   selectedWidgetX      READ selectedWidgetX      NOTIFY selectedWidgetChanged)
    Q_PROPERTY(qreal   selectedWidgetY      READ selectedWidgetY      NOTIFY selectedWidgetChanged)
    Q_PROPERTY(qreal   selectedWidgetWidth  READ selectedWidgetWidth  NOTIFY selectedWidgetChanged)
    Q_PROPERTY(qreal   selectedWidgetHeight READ selectedWidgetHeight NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedDataSource   READ selectedDataSource   NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedControlType  READ selectedControlType  NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedButtonColor  READ selectedButtonColor  NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedBorderColor  READ selectedBorderColor  NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QString selectedIconColor    READ selectedIconColor    NOTIFY selectedWidgetChanged)
    Q_PROPERTY(bool    selectedAutoFill     READ selectedAutoFill     NOTIFY selectedWidgetChanged)
    Q_PROPERTY(bool    hasSelection         READ hasSelection         NOTIFY selectedWidgetChanged)
    Q_PROPERTY(int     selectedCount        READ selectedCount        NOTIFY selectedWidgetChanged)
    Q_PROPERTY(int     widgetCount          READ widgetCount          NOTIFY widgetCountChanged)
    Q_PROPERTY(bool    canUndo              READ canUndo              NOTIFY undoRedoChanged)
    Q_PROPERTY(bool    canRedo              READ canRedo              NOTIFY undoRedoChanged)
    Q_PROPERTY(QVariantList selectedPropertyDefinitions READ selectedPropertyDefinitions NOTIFY selectedWidgetChanged)
    Q_PROPERTY(bool selectedWidgetHasDataSource READ selectedWidgetHasDataSource NOTIFY selectedWidgetChanged)
    Q_PROPERTY(QVariantList selectedAppearanceFields READ selectedAppearanceFields NOTIFY selectedWidgetChanged)
    Q_PROPERTY(qreal canvasWidth READ canvasWidth NOTIFY canvasSizeChanged)
    Q_PROPERTY(qreal canvasHeight READ canvasHeight NOTIFY canvasSizeChanged)
    Q_PROPERTY(int     currentBasicLayout   READ currentBasicLayout   NOTIFY layoutChanged)
    Q_PROPERTY(int     currentSplitTemplate READ currentSplitTemplate NOTIFY layoutChanged)
    Q_PROPERTY(int     currentCustomRows READ currentCustomRows NOTIFY layoutChanged)
    Q_PROPERTY(int     currentCustomColumns READ currentCustomColumns NOTIFY layoutChanged)
    Q_PROPERTY(QVariantList layoutCells READ layoutCells NOTIFY layoutChanged)
    Q_PROPERTY(QVariantList layoutResizeHandles READ layoutResizeHandles NOTIFY layoutChanged)
    Q_PROPERTY(int selectedLayoutCellId READ selectedLayoutCellId NOTIFY layoutChanged)
    Q_PROPERTY(int selectedLayoutCellCount READ selectedLayoutCellCount NOTIFY layoutChanged)
    Q_PROPERTY(bool hasSelectedLayoutCell READ hasSelectedLayoutCell NOTIFY layoutChanged)
    Q_PROPERTY(QString inspectorMode READ inspectorMode NOTIFY inspectorModeChanged)
    Q_PROPERTY(QString selectedContainerName READ selectedContainerName NOTIFY layoutChanged)
    Q_PROPERTY(qreal selectedContainerWidth READ selectedContainerWidth NOTIFY layoutChanged)
    Q_PROPERTY(qreal selectedContainerHeight READ selectedContainerHeight NOTIFY layoutChanged)
    Q_PROPERTY(bool    currentShowGrid      READ currentShowGrid      NOTIFY layoutChanged)
    Q_PROPERTY(QString currentGridLineColor READ currentGridLineColor NOTIFY layoutChanged)
    Q_PROPERTY(QString currentBackgroundColor READ currentBackgroundColor NOTIFY layoutChanged)
    Q_PROPERTY(bool topFixedVisible READ isTopFixedVisible NOTIFY fixedBarChanged)
    Q_PROPERTY(bool bottomFixedVisible READ isBottomFixedVisible NOTIFY fixedBarChanged)
    Q_PROPERTY(bool leftFixedVisible READ isLeftFixedVisible NOTIFY fixedBarChanged)
    Q_PROPERTY(bool rightFixedVisible READ isRightFixedVisible NOTIFY fixedBarChanged)
    Q_PROPERTY(bool previewMode READ previewMode NOTIFY previewModeChanged)
    Q_PROPERTY(int activeTabIndex READ activeTabIndex NOTIFY activeTabChanged)
    Q_PROPERTY(int tabCount READ tabCount NOTIFY tabsChanged)

public:
    explicit CanvasViewModel(QObject* parent = nullptr);

    enum Roles {
        IdRole = Qt::UserRole + 1,
        ParentRegionIdRole, TypeRole, XRole, YRole, WidthRole, HeightRole,
        TitleRole, SelectedRole, ComponentSourceRole, DataSourceRole, ControlTypeRole,
        ButtonColorRole, BorderColorRole, IconColorRole, AutoFillRole
    };

    int      rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void addWidget(const QString& type);
    Q_INVOKABLE void addWidgetAt(const QString& type, qreal x, qreal y);
    Q_INVOKABLE void selectWidget(int id, bool additive = false);
    Q_INVOKABLE void clearSelection();
    Q_INVOKABLE void removeSelectedWidget();
    Q_INVOKABLE void duplicateSelectedWidget();
    Q_INVOKABLE void moveSelectedForward();
    Q_INVOKABLE void moveSelectedBackward();
    Q_INVOKABLE void alignSelected(const QString& mode, qreal canvasWidth, qreal canvasHeight);
    Q_INVOKABLE void updateWidgetGeometry(int id, qreal x, qreal y, qreal width, qreal height);
    Q_INVOKABLE void updateSelectedTitle(const QString& title);
    Q_INVOKABLE void updateSelectedData(const QString& dataSource, const QString& controlType);
    Q_INVOKABLE void updateSelectedAppearance(const QString& buttonColor, const QString& borderColor, const QString& iconColor);
    Q_INVOKABLE QString selectedAppearanceValue(const QString& key) const;
    Q_INVOKABLE void updateSelectedAppearanceField(const QString& key, const QString& value);
    Q_INVOKABLE void updateSelectedAutoFill(bool autoFill);
    Q_INVOKABLE QVariant selectedPropertyValue(const QString& key) const;
    Q_INVOKABLE void updateSelectedProperty(const QString& key, const QVariant& value);
    Q_INVOKABLE void bringSelectedToFront();
    Q_INVOKABLE void bringSelectedForward();
    Q_INVOKABLE void sendSelectedToBack();
    Q_INVOKABLE void sendSelectedBackward();
    Q_INVOKABLE void distributeSelectedHorizontal();
    Q_INVOKABLE void distributeSelectedVertical();
    Q_INVOKABLE void undo();
    Q_INVOKABLE void redo();
    Q_INVOKABLE void fitToWindow();
    Q_INVOKABLE int fittedZoomPercent(qreal viewportWidth, qreal viewportHeight,
                                      qreal designWidth, qreal designHeight,
                                      qreal horizontalPadding, qreal verticalPadding) const;
    Q_INVOKABLE void clear();
    Q_INVOKABLE bool saveToFile(const QString& filePath) const;
    Q_INVOKABLE bool loadFromFile(const QString& filePath);
    Q_INVOKABLE void setCanvasSize(qreal width, qreal height);
    Q_INVOKABLE void enterPreview();
    Q_INVOKABLE void exitPreview();

    // Tab and fixed bar management (MVVM API)
    Q_INVOKABLE void addTab();
    Q_INVOKABLE void removeTab(int index);
    Q_INVOKABLE void renameTab(int index, const QString& title);
    Q_INVOKABLE void setActiveTab(int index);
    Q_INVOKABLE int  activeTabIndex() const;
    Q_INVOKABLE int  tabCount() const;

    Q_INVOKABLE void setFixedBar(const QString& bar, bool visible);
    Q_INVOKABLE bool isTopFixedVisible() const;
    Q_INVOKABLE bool isBottomFixedVisible() const;
    Q_INVOKABLE bool isLeftFixedVisible() const;
    Q_INVOKABLE bool isRightFixedVisible() const;
    Q_INVOKABLE void setBasicLayout(int basicLayout);
    Q_INVOKABLE void setSplitTemplate(int tmpl);
    Q_INVOKABLE void setCustomLayout(int rows, int columns);
    Q_INVOKABLE void splitSelectedLayoutCells(int rows, int columns);
    Q_INVOKABLE void clearLayoutToRootContainer();
    Q_INVOKABLE void splitSelectedRegionHorizontal();
    Q_INVOKABLE void splitSelectedRegionVertical();
    Q_INVOKABLE bool splitCell(int cellId, const QString& orientation, qreal ratio);
    Q_INVOKABLE bool splitCellAt(const QString& orientation, qreal normalizedX, qreal normalizedY);
    Q_INVOKABLE void selectLayoutCell(int cellId, bool additive = false);
    Q_INVOKABLE void clearLayoutCellSelection();
    Q_INVOKABLE void assignSelectedWidgetToLayoutCell(int cellId);
    Q_INVOKABLE void addWidgetToLayoutCell(const QString& type, int cellId);
    Q_INVOKABLE void addWidgetToLayoutCellAt(const QString& type, int cellId, qreal x, qreal y);
    Q_INVOKABLE void clearWidgetsInSelectedLayoutCell();
    Q_INVOKABLE void mergeSelectedLayoutCells();
    Q_INVOKABLE bool mergeSelectedContainersHorizontal();
    Q_INVOKABLE bool mergeSelectedContainersVertical();
    Q_INVOKABLE bool mergeCells(const QVariantList& cellIds);
    Q_INVOKABLE void mergeLayoutSiblings(int firstCellId, int secondCellId);
    Q_INVOKABLE void unmergeSelectedLayoutCell();
    Q_INVOKABLE void beginResizeDivider(int parentCellId);
    Q_INVOKABLE bool resizeDivider(int parentCellId, qreal ratio, qreal minFirstRatio = 0.0, qreal minSecondRatio = 0.0);
    Q_INVOKABLE void endResizeDivider();
    Q_INVOKABLE bool setExactRatio(int parentCellId, qreal ratio);
    Q_INVOKABLE void resizeLayoutCells(const QString& firstCellId, const QString& secondCellId,
                                       const QString& orientation, qreal deltaRatio);
    Q_INVOKABLE int  currentBasicLayout() const;
    Q_INVOKABLE int  currentSplitTemplate() const;
    Q_INVOKABLE int  currentCustomRows() const;
    Q_INVOKABLE int  currentCustomColumns() const;
    Q_INVOKABLE bool currentShowGrid() const;
    Q_INVOKABLE QString currentGridLineColor() const;
    Q_INVOKABLE QString currentBackgroundColor() const;

    int     selectedWidgetId()     const;
    QString selectedWidgetType()   const;
    QString selectedWidgetTitle()  const;
    qreal   selectedWidgetX()      const;
    qreal   selectedWidgetY()      const;
    qreal   selectedWidgetWidth()  const;
    qreal   selectedWidgetHeight() const;
    QString selectedDataSource()   const;
    QString selectedControlType()  const;
    QString selectedButtonColor()  const;
    QString selectedBorderColor()  const;
    QString selectedIconColor()    const;
    bool    selectedAutoFill()     const;
    bool    hasSelection()         const;
    int     selectedCount()        const;
    int     widgetCount()          const;
    bool    canUndo()              const;
    bool    canRedo()              const;
    bool    previewMode()          const;
    QVariantList selectedPropertyDefinitions() const;
    bool selectedWidgetHasDataSource() const;
    QVariantList selectedAppearanceFields() const;
    qreal canvasWidth() const;
    qreal canvasHeight() const;
    QVariantList layoutCells() const;
    QVariantList layoutResizeHandles() const;
    int selectedLayoutCellId() const;
    int selectedLayoutCellCount() const;
    bool hasSelectedLayoutCell() const;
    QString inspectorMode() const;
    QString selectedContainerName() const;
    qreal selectedContainerWidth() const;
    qreal selectedContainerHeight() const;

signals:
    void selectedWidgetChanged();
    void widgetCountChanged();
    void undoRedoChanged();
    void fixedBarChanged();
    void tabsChanged();
    void activeTabChanged();
    void layoutChanged();
    void inspectorModeChanged();
    void canvasSizeChanged();
    void previewModeChanged();

private:
    struct StateSnapshot
    {
        QList<WidgetItem> widgets;
        QList<int> selectedWidgetIds;
        int primarySelectedWidgetId = -1;
        int nextId = 1;
        QList<eversight::CanvasTabModel> tabs;
        int activeTab = 0;
    };

    WidgetItem        createWidget(const QString& type) const;
    int               indexOfWidget(int id) const;
    const WidgetItem* selectedWidget() const;
    QList<int>        selectedRows() const;
    StateSnapshot     snapshot() const;
    void              restoreSnapshot(const StateSnapshot& state);
    void              pushUndoState();
    void              syncScreenFromWidgets();
    void              emitAllWidgetDataChanged();
    void              setSelection(const QList<int>& ids, int primaryId);
    eversight::CanvasLayoutModel* activeLayout();
    const eversight::CanvasLayoutModel* activeLayout() const;
    eversight::LayoutNode* findLayoutNode(int id);
    const eversight::LayoutNode* findLayoutNode(int id) const;
    eversight::LayoutNode* findLayoutParent(int childId);
    const eversight::LayoutNode* findLayoutParent(int childId) const;
    void splitLayoutNode(eversight::LayoutNode& node, int rows, int columns);
    bool splitLayoutNodeAtRatio(eversight::LayoutNode& node, const QString& orientation, qreal ratio);
    void setLayoutNodeRect(eversight::LayoutNode& node, qreal x, qreal y, qreal width, qreal height);
    int cellAt(qreal normalizedX, qreal normalizedY) const;
    bool mergeSelectedContainersByOrientation(const QString& orientation);
    void setLayoutTemplate(eversight::CanvasLayoutModel& layout, int tmpl);
    int nextLayoutNodeId(eversight::CanvasLayoutModel& layout);
    void assignWidgetToLayoutCell(int widgetId, int cellId);
    void updateWidgetGeometryFromLayoutCell(int widgetId, const eversight::LayoutNode& cell);
    void clampWidgetsToLayoutBounds();
    int defaultLeafRegionId() const;
    void reassignWidgetsToRegion(const QList<int>& oldRegionIds, int newRegionId);

    WidgetStore       m_widgetStore;
    int               m_selectedWidgetId = -1;
    QList<int>        m_selectedWidgetIds;
    UndoStack<StateSnapshot> m_undoStack;
    UndoStack<StateSnapshot> m_redoStack;
    eversight::ScreenModel m_screen;
    QList<eversight::CanvasTabModel> m_tabs;
    int m_activeTab = 0;
    eversight::FixedBarState m_fixedBars;
    PreviewManager m_previewManager;
    int m_resizingDividerParentCellId = -1;
};
