#pragma once

#include "../../shared/models/WidgetItem.h"

#include <QList>

class WidgetStore
{
public:
    QList<WidgetItem>& widgets() { return m_widgets; }
    const QList<WidgetItem>& widgets() const { return m_widgets; }

    int& nextIdRef() { return m_nextId; }
    int nextId() const { return m_nextId; }
    void setNextId(int nextId) { m_nextId = nextId; }

private:
    QList<WidgetItem> m_widgets;
    int m_nextId = 1;
};
