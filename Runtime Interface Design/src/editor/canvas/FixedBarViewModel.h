#pragma once

#include <QObject>

namespace eversight {

class FixedBarViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool topVisible READ topVisible WRITE setTopVisible NOTIFY topVisibleChanged)
    Q_PROPERTY(bool bottomVisible READ bottomVisible WRITE setBottomVisible NOTIFY bottomVisibleChanged)
    Q_PROPERTY(bool leftVisible READ leftVisible WRITE setLeftVisible NOTIFY leftVisibleChanged)
    Q_PROPERTY(bool rightVisible READ rightVisible WRITE setRightVisible NOTIFY rightVisibleChanged)

    Q_PROPERTY(int topHeight READ topHeight WRITE setTopHeight NOTIFY topHeightChanged)
    Q_PROPERTY(int bottomHeight READ bottomHeight WRITE setBottomHeight NOTIFY bottomHeightChanged)
    Q_PROPERTY(int leftWidth READ leftWidth WRITE setLeftWidth NOTIFY leftWidthChanged)
    Q_PROPERTY(int rightWidth READ rightWidth WRITE setRightWidth NOTIFY rightWidthChanged)

public:
    explicit FixedBarViewModel(QObject* parent = nullptr);

    bool topVisible() const;
    bool bottomVisible() const;
    bool leftVisible() const;
    bool rightVisible() const;

    int topHeight() const;
    int bottomHeight() const;
    int leftWidth() const;
    int rightWidth() const;

public slots:
    void setTopVisible(bool v);
    void setBottomVisible(bool v);
    void setLeftVisible(bool v);
    void setRightVisible(bool v);

    void setTopHeight(int h);
    void setBottomHeight(int h);
    void setLeftWidth(int w);
    void setRightWidth(int w);

signals:
    void topVisibleChanged();
    void bottomVisibleChanged();
    void leftVisibleChanged();
    void rightVisibleChanged();

    void topHeightChanged();
    void bottomHeightChanged();
    void leftWidthChanged();
    void rightWidthChanged();

private:
    bool m_topVisible = true;
    bool m_bottomVisible = true;
    bool m_leftVisible = true;
    bool m_rightVisible = true;

    int m_topHeight = 56;
    int m_bottomHeight = 56;
    int m_leftWidth = 56;
    int m_rightWidth = 56;
};

} // namespace eversight
