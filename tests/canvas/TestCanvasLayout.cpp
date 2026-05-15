#include "../../src/editor/canvas/CanvasViewModel.h"

#include <QTest>
#include <QVariantMap>

class TestCanvasLayout : public QObject
{
    Q_OBJECT

private slots:
    void splitCell();
    void resizeDivider();
    void mergeCells();
    void setExactRatio();
};

// Verifies splitting one leaf creates two proportional child cells.
void TestCanvasLayout::splitCell()
{
    CanvasViewModel model;

    QVERIFY(model.splitCell(1, QStringLiteral("vertical"), 0.25));
    const QVariantList cells = model.layoutCells();
    QCOMPARE(cells.size(), 2);

    const QVariantMap first = cells.at(0).toMap();
    const QVariantMap second = cells.at(1).toMap();
    QCOMPARE(first.value("width").toDouble(), 0.25);
    QCOMPARE(second.value("x").toDouble(), 0.25);
    QCOMPARE(second.value("width").toDouble(), 0.75);
}

// Verifies divider resize updates sibling ratios while preserving total size.
void TestCanvasLayout::resizeDivider()
{
    CanvasViewModel model;

    QVERIFY(model.splitCell(1, QStringLiteral("vertical"), 0.25));
    QVERIFY(model.resizeDivider(1, 0.40));

    const QVariantList cells = model.layoutCells();
    const QVariantMap first = cells.at(0).toMap();
    const QVariantMap second = cells.at(1).toMap();
    QCOMPARE(first.value("width").toDouble(), 0.40);
    QCOMPARE(second.value("x").toDouble(), 0.40);
    QCOMPARE(second.value("width").toDouble(), 0.60);
}

// Verifies adjacent sibling leaves can merge back into one leaf cell.
void TestCanvasLayout::mergeCells()
{
    CanvasViewModel model;

    QVERIFY(model.splitCell(1, QStringLiteral("vertical"), 0.25));
    QVariantList ids;
    ids << 2 << 3;
    QVERIFY(model.mergeCells(ids));

    const QVariantList cells = model.layoutCells();
    QCOMPARE(cells.size(), 1);
    const QVariantMap merged = cells.first().toMap();
    QCOMPARE(merged.value("id").toInt(), 1);
    QCOMPARE(merged.value("width").toDouble(), 1.0);
}

// Verifies exact ratio editing recalculates sibling ratios.
void TestCanvasLayout::setExactRatio()
{
    CanvasViewModel model;

    QVERIFY(model.splitCell(1, QStringLiteral("horizontal"), 0.50));
    QVERIFY(model.setExactRatio(1, 0.30));

    const QVariantList cells = model.layoutCells();
    const QVariantMap first = cells.at(0).toMap();
    const QVariantMap second = cells.at(1).toMap();
    QCOMPARE(first.value("height").toDouble(), 0.30);
    QCOMPARE(second.value("y").toDouble(), 0.30);
    QCOMPARE(second.value("height").toDouble(), 0.70);
}

QTEST_APPLESS_MAIN(TestCanvasLayout)
#include "TestCanvasLayout.moc"
