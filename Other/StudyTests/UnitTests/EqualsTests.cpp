#include <QString>
#include <QtTest/QtTest>
#include "Category.h"
#include "Test.h"
#include "Task.h"
#include "Archiver.h"

class EqualsTests : public QObject
{
    Q_OBJECT

public:
    EqualsTests();

private Q_SLOTS:
    void equalTestTask();
    void equalTestTest();
    void equalTestCategory();
    void noEqualTestTask();
    void noEqualTestTest();
    void noEqualTestCategory();
};

EqualsTests::EqualsTests()
{
}

void EqualsTests::equalTestTask()
{
    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(3);


    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 1");
    task2->setAnswer("Відповідь 1");
    task2->setNumPoints(3);


    QVERIFY(task1->equals(task2.get()));
}

void EqualsTests::equalTestTest()
{
     auto test1 = std::make_unique<Test>();
     test1->setName("Ім'я");

     auto task1 = std::make_unique<Task>();
     task1->setQuestion("Запитання 1");
     task1->setAnswer("Відповідь 1");
     task1->setNumPoints(2);

     auto task2 = std::make_unique<Task>();
     task2->setQuestion("Запитання 2");
     task2->setAnswer("Відповідь 2");
     task2->setNumPoints(4);

     test1->addChild(std::move(task1));
     test1->addChild(std::move(task2));


     auto test2 = std::make_unique<Test>();
     test2->setName("Ім'я");

     auto task3 = std::make_unique<Task>();
     task3->setQuestion("Запитання 1");
     task3->setAnswer("Відповідь 1");
     task3->setNumPoints(2);

     auto task4 = std::make_unique<Task>();
     task4->setQuestion("Запитання 2");
     task4->setAnswer("Відповідь 2");
     task4->setNumPoints(4);

     test2->addChild(std::move(task3));
     test2->addChild(std::move(task4));


     QVERIFY(test1->equals(test2.get()));
}

void EqualsTests::equalTestCategory()
{
    auto category1 = std::make_unique<Category>();
    category1->setName("Ім'я");

    auto test1 = std::make_unique<Test>();
    test1->setName("Ім'я");

    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(2);

    test1->addChild(std::move(task1));

    category1->addChild(std::move(test1));

    auto category2 = std::make_unique<Category>();
    category2->setName("Ім'я");

    auto test2 = std::make_unique<Test>();
    test2->setName("Ім'я");

    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 1");
    task2->setAnswer("Відповідь 1");
    task2->setNumPoints(2);

    test2->addChild(std::move(task2));

    category2->addChild(std::move(test2));

    QVERIFY(category1->equals(category2.get()));
}

void EqualsTests::noEqualTestTask()
{
    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(3);


    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 2");
    task2->setAnswer("Відповідь 2");
    task2->setNumPoints(3);


    QVERIFY(!task1->equals(task2.get()));
}

void EqualsTests::noEqualTestTest()
{
     auto test1 = std::make_unique<Test>();
     test1->setName("Ім'я");

     auto task1 = std::make_unique<Task>();
     task1->setQuestion("Запитання 1");
     task1->setAnswer("Відповідь 1");
     task1->setNumPoints(2);

     auto task2 = std::make_unique<Task>();
     task2->setQuestion("Запитання 2");
     task2->setAnswer("Відповідь 2");
     task2->setNumPoints(4);

     test1->addChild(std::move(task1));
     test1->addChild(std::move(task2));


     auto test2 = std::make_unique<Test>();
     test2->setName("Ім'я");

     auto task3 = std::make_unique<Task>();
     task3->setQuestion("Запитання 1");
     task3->setAnswer("Відповідь 2");
     task3->setNumPoints(7);

     auto task4 = std::make_unique<Task>();
     task4->setQuestion("Запитання 2");
     task4->setAnswer("Відповідь 2");
     task4->setNumPoints(4);

     test2->addChild(std::move(task3));
     test2->addChild(std::move(task4));


     QVERIFY(!test1->equals(test2.get()));
}

void EqualsTests::noEqualTestCategory()
{
    auto category1 = std::make_unique<Category>();
    category1->setName("Ім'я");

    auto test1 = std::make_unique<Test>();
    test1->setName("Ім'я");

    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(2);

    test1->addChild(std::move(task1));

    category1->addChild(std::move(test1));

    auto category2 = std::make_unique<Category>();
    category2->setName("Ім'я");

    auto test2 = std::make_unique<Test>();
    test2->setName("Ім'я");

    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 1");
    task2->setAnswer("Відповідь 2");
    task2->setNumPoints(5);

    test2->addChild(std::move(task2));

    category2->addChild(std::move(test2));

    QVERIFY(!category1->equals(category2.get()));
}

#include "EqualsTests.moc"

