#pragma once

#include <QList>

template <typename Snapshot>
class UndoStack
{
public:
    bool isEmpty() const { return m_items.isEmpty(); }
    int size() const { return m_items.size(); }
    void append(const Snapshot& snapshot) { m_items.append(snapshot); }
    Snapshot takeLast() { return m_items.takeLast(); }
    void removeFirst() { m_items.removeFirst(); }
    void clear() { m_items.clear(); }

private:
    QList<Snapshot> m_items;
};
