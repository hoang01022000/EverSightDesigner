#include "FixedBarViewModel.h"

namespace eversight {

FixedBarViewModel::FixedBarViewModel(QObject* parent)
    : QObject(parent)
{
}

bool FixedBarViewModel::topVisible() const { return m_topVisible; }
bool FixedBarViewModel::bottomVisible() const { return m_bottomVisible; }
bool FixedBarViewModel::leftVisible() const { return m_leftVisible; }
bool FixedBarViewModel::rightVisible() const { return m_rightVisible; }

int FixedBarViewModel::topHeight() const { return m_topHeight; }
int FixedBarViewModel::bottomHeight() const { return m_bottomHeight; }
int FixedBarViewModel::leftWidth() const { return m_leftWidth; }
int FixedBarViewModel::rightWidth() const { return m_rightWidth; }

void FixedBarViewModel::setTopVisible(bool v)
{
    if (m_topVisible == v) return;
    m_topVisible = v;
    emit topVisibleChanged();
}

void FixedBarViewModel::setBottomVisible(bool v)
{
    if (m_bottomVisible == v) return;
    m_bottomVisible = v;
    emit bottomVisibleChanged();
}

void FixedBarViewModel::setLeftVisible(bool v)
{
    if (m_leftVisible == v) return;
    m_leftVisible = v;
    emit leftVisibleChanged();
}

void FixedBarViewModel::setRightVisible(bool v)
{
    if (m_rightVisible == v) return;
    m_rightVisible = v;
    emit rightVisibleChanged();
}

void FixedBarViewModel::setTopHeight(int h)
{
    if (m_topHeight == h) return;
    m_topHeight = h;
    emit topHeightChanged();
}

void FixedBarViewModel::setBottomHeight(int h)
{
    if (m_bottomHeight == h) return;
    m_bottomHeight = h;
    emit bottomHeightChanged();
}

void FixedBarViewModel::setLeftWidth(int w)
{
    if (m_leftWidth == w) return;
    m_leftWidth = w;
    emit leftWidthChanged();
}

void FixedBarViewModel::setRightWidth(int w)
{
    if (m_rightWidth == w) return;
    m_rightWidth = w;
    emit rightWidthChanged();
}

} // namespace eversight
