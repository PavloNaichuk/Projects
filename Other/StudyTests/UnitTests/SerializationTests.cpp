#include <QString>
#include <QtTest/QtTest>
#include "Category.h"
#include "Test.h"
#include "Task.h"
#include "Archiver.h"

bool areEqual(const Component* comp1, const Component* comp2);

class SerializationTests : public QObject
{
    Q_OBJECT

public:
    SerializationTests();

private Q_SLOTS:
    void testTask();
    void testTest();
    void testCategory1();
    void testCategory2();
    void testCategory3();
    void testCategory4();
};

SerializationTests::SerializationTests()
{
}

void SerializationTests::testTask()
{
    Task task1;
    task1.setQuestion("Запитання 1");
    task1.setAnswer("Відповідь 1");
    task1.setNumPoints(3);

    auto jsonObject = Archiver::serialise(&task1);
    auto task2 = Archiver::deserialise(*jsonObject);

    QVERIFY(areEqual(&task1, task2.get()));
}

void SerializationTests::testTest()
{
     Test test1;
     test1.setName("Ім'я");

     auto task1 = std::make_unique<Task>();
     task1->setQuestion("Запитання 1");
     task1->setAnswer("Відповідь 1");
     task1->setNumPoints(2);

     auto task2 = std::make_unique<Task>();
     task2->setQuestion("Запитання 2");
     task2->setAnswer("Відповідь 2");
     task2->setNumPoints(4);

     test1.addChild(std::move(task1));
     test1.addChild(std::move(task2));

     auto jsonObject = Archiver::serialise(&test1);
     auto test2 = Archiver::deserialise(*jsonObject);

     QVERIFY(areEqual(&test1, test2.get()));
}

void SerializationTests::testCategory1()
{
    Category category1;
    category1.setName("Ім'я");

    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(2);

    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 2");
    task2->setAnswer("Відповідь 2");
    task2->setNumPoints(4);

    category1.addChild(std::move(task1));
    category1.addChild(std::move(task2));

    auto jsonObject = Archiver::serialise(&category1);
    auto category2 = Archiver::deserialise(*jsonObject);

    QVERIFY(areEqual(&category1, category2.get()));
}

void SerializationTests::testCategory2()
{
    Category category1;
    category1.setName("Ім'я");

    auto test1 = std::make_unique<Test>();
    test1->setName("Тест 1");

    auto test2 = std::make_unique<Test>();
    test2->setName("Тест 2");

    category1.addChild(std::move(test1));
    category1.addChild(std::move(test2));

    auto jsonObject = Archiver::serialise(&category1);
    auto category2 = Archiver::deserialise(*jsonObject);

    QVERIFY(areEqual(&category1, category2.get()));
}

void SerializationTests::testCategory3()
{
    Category category1;
    category1.setName("Ім'я");

    auto test1 = std::make_unique<Test>();
    test1->setName("Тест 1");

    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(2);

    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 2");
    task2->setAnswer("Відповідь 2");
    task2->setNumPoints(4);

    auto task3 = std::make_unique<Task>();
    task3->setQuestion("Запитання 3");
    task3->setAnswer("Відповідь 3");
    task3->setNumPoints(7);

    auto task4 = std::make_unique<Task>();
    task4->setQuestion("Запитання 4");
    task4->setAnswer("Відповідь 4");
    task4->setNumPoints(9);

    auto task5 = std::make_unique<Task>();
    task5->setQuestion("Запитання 5");
    task5->setAnswer("Відповідь 5");
    task5->setNumPoints(1);

    auto task6 = std::make_unique<Task>();
    task6->setQuestion("Запитання 6");
    task6->setAnswer("Відповідь 6");
    task6->setNumPoints(2);

    test1->addChild(std::move(task1));
    test1->addChild(std::move(task2));
    test1->addChild(std::move(task3));
    test1->addChild(std::move(task4));
    test1->addChild(std::move(task5));
    test1->addChild(std::move(task6));

    auto test2 = std::make_unique<Test>();
    test2->setName("Тест 2");

    auto task7 = std::make_unique<Task>();
    task7->setQuestion("Запитання 7");
    task7->setAnswer("Відповідь 7");
    task7->setNumPoints(2);

    auto task8 = std::make_unique<Task>();
    task8->setQuestion("Запитання 8");
    task8->setAnswer("Відповідь 8");
    task8->setNumPoints(4);

    auto task9 = std::make_unique<Task>();
    task9->setQuestion("Запитання 9");
    task9->setAnswer("Відповідь 9");
    task9->setNumPoints(3);

    test2->addChild(std::move(task7));
    test2->addChild(std::move(task8));
    test2->addChild(std::move(task9));

    auto category2 = std::make_unique<Category>();
    category2->setName("Ім'я");

    category1.addChild(std::move(test1));
    category1.addChild(std::move(test2));
    category1.addChild(std::move(category2));

    auto jsonObject = Archiver::serialise(&category1);
    auto category3 = Archiver::deserialise(*jsonObject);

    QVERIFY(areEqual(&category1, category3.get()));
}

void SerializationTests::testCategory4()
{
    Category category1;
    category1.setName("Ім'я");

    auto test1 = std::make_unique<Test>();
    test1->setName("Тест 1");

    auto task1 = std::make_unique<Task>();
    task1->setQuestion("Запитання 1");
    task1->setAnswer("Відповідь 1");
    task1->setNumPoints(2);

    auto task2 = std::make_unique<Task>();
    task2->setQuestion("Запитання 2");
    task2->setAnswer("Відповідь 2");
    task2->setNumPoints(4);

    auto task3 = std::make_unique<Task>();
    task3->setQuestion("Запитання 3");
    task3->setAnswer("Відповідь 3");
    task3->setNumPoints(7);


    test1->addChild(std::move(task1));
    test1->addChild(std::move(task2));
    test1->addChild(std::move(task3));


    auto test2 = std::make_unique<Test>();
    test2->setName("Тест 2");

    auto task4 = std::make_unique<Task>();
    task4->setQuestion("Запитання 4");
    task4->setAnswer("Відповідь 4");
    task4->setNumPoints(9);

    auto task5 = std::make_unique<Task>();
    task5->setQuestion("Запитання 5");
    task5->setAnswer("Відповідь 5");
    task5->setNumPoints(1);

    auto task6 = std::make_unique<Task>();
    task6->setQuestion("Запитання 6");
    task6->setAnswer("Відповідь 6");
    task6->setNumPoints(2);

    test2->addChild(std::move(task4));
    test2->addChild(std::move(task5));
    test2->addChild(std::move(task6));


    auto category2 = std::make_unique<Category>();
    category2->setName("Ім'я 1");

    auto category3 = std::make_unique<Category>();
    category2->setName("Ім'я 2");

    auto category4 = std::make_unique<Category>();
    category2->setName("Ім'я 3");

    auto category5 = std::make_unique<Category>();
    category2->setName("Ім'я 4");

    auto category6 = std::make_unique<Category>();
    category2->setName("Ім'я 5");

    category2->addChild(std::move(test1));
    category2->addChild(std::move(test2));

    category3->addChild(std::move(category4));
    category3->addChild(std::move(category5));
    category3->addChild(std::move(category6));

    category1.addChild(std::move(category2));
    category1.addChild(std::move(category3));

    auto jsonObject = Archiver::serialise(&category1);
    auto category7 = Archiver::deserialise(*jsonObject);

    QVERIFY(areEqual(&category1, category7.get()));
}

bool areEqual(const Component* comp1, const Component* comp2)
{
    const Task* task1 = dynamic_cast<const Task*>(comp1);
    const Task* task2 = dynamic_cast<const Task*>(comp2);
    if ((task1 != nullptr) && (task2 != nullptr))
    {
        return ((task1->question() == task2->question()) &&
                (task1->answer() == task2->answer()) &&
                (task1->numPoints() == task2->numPoints()));
    }

    const Test* test1 = dynamic_cast<const Test*>(comp1);
    const Test* test2 = dynamic_cast<const Test*>(comp2);
    if ((test1 != nullptr) && (test2 != nullptr))
    {
        if (test1->name() != test2->name())
            return false;
        if (test1->numChildren() != test2->numChildren())
            return false;
        for (decltype(test1->numChildren()) childIndex = 0; childIndex < test1->numChildren(); ++childIndex)
        {
            if (!areEqual(test1->child(childIndex), test2->child(childIndex)))
                return false;
        }
        return true;
    }

    const Category* category1 = dynamic_cast<const Category*>(comp1);
    const Category* category2 = dynamic_cast<const Category*>(comp2);
    if ((category1 != nullptr) && (category2 != nullptr))
    {
        if (category1->name() != category2->name())
            return false;
        if (category1->numChildren() != category2->numChildren())
            return false;
        for (decltype(category1->numChildren()) childIndex = 0; childIndex < category1->numChildren(); ++childIndex)
        {
            if (!areEqual(category1->child(childIndex), category2->child(childIndex)))
                return false;
        }
        return true;
    }
    return false;
}


QTEST_APPLESS_MAIN(SerializationTests)

#include "SerializationTests.moc"

