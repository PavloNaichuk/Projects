#include <QString>
#include <QtTest/QtTest>
#include "Test.h"
#include "Task.h"

class TestTests : public QObject
{
    Q_OBJECT

public:
    TestTests();

private Q_SLOTS:
    void testTest();
};

TestTests::TestTests()
{
}

void TestTests::testTest()
{
    Test test;
    test.setName("Iм'я");
    QVERIFY(test.name() == "Iм'я");

    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(2);

    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 2");
    task2->setAnswer("Відповідь 2");
    task2->setNumPoints(4);

    test.addChild(std::move(task1));
    QVERIFY(test.numChildren() == 1);
    test.addChild(std::move(task2));
    QVERIFY(test.numChildren() == 2);

    // test.child(0) == task1;
    // test.child(1) == task2;

    test.removeChild(0);
    QVERIFY(test.numChildren() == 1);

    test.removeChild(0);
    QVERIFY(test.numChildren() == 0);

    QVERIFY(test.type() == TEST);
}

#include "TestTests.moc"
