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
    void testTest1();
    void testTest2();
};

TestTests::TestTests()
{
}

void TestTests::testTest1()
{
    Test test;
    test.setName("Iм'я");

    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(2);

    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 2");
    task2->setAnswer("Відповідь 2");
    task2->setNumPoints(4);

    test.addChild(std::move(task1));
    test.addChild(std::move(task2));

    size_t number = test.numChildren();

    test.removeChild(1);

    QVERIFY(test.name() == "Iм'я");
    QVERIFY(number == 2);
    QVERIFY(number != test.numChildren());
    QVERIFY(test.numChildren() == 1);
}

void TestTests::testTest2()
{
    Test test;
    test.setName("Iм'я");

    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(2);

    test.addChild(std::move(task1));
    size_t number1 = test.numChildren();
    test.removeChild(0);

    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 2");
    task2->setAnswer("Відповідь 2");
    task2->setNumPoints(4);

    test.addChild(std::move(task1));
    size_t number2 = test.numChildren();

    QVERIFY(test.name() == "Iм'я");
    QVERIFY(number1 == number2);
    QVERIFY(test.numChildren() == 1);
}

#include "TestTests.moc"
