#include <QString>
#include <QtTest/QtTest>
#include "Test.h"
#include "Task.h"
#include "Category.h"

class CategoryTests : public QObject
{
    Q_OBJECT

public:
    CategoryTests();

private Q_SLOTS:
    void testCategory();
};

CategoryTests::CategoryTests()
{
}

void CategoryTests::testCategory()
{
    Category category;
    QVERIFY(category.numChildren() == 0);

    category.setName("Iм'я");
    QVERIFY(category.name() == "Iм'я");

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

    test1->addChild(std::move(task1));
    test1->addChild(std::move(task2));

    auto task3 = std::make_unique<Task>();
    task3->setQuestion("Запитання 3");
    task3->setAnswer("Відповідь 3");
    task3->setNumPoints(3);

    auto task4 = std::make_unique<Task>();
    task4->setQuestion("Запитання 4");
    task4->setAnswer("Відповідь 4");
    task4->setNumPoints(9);

    auto test2 = std::make_unique<Test>();
    test2->setName("Тест 2");

    test2->addChild(std::move(task3));
    test2->addChild(std::move(task4));

    category.addChild(std::move(test1));
    QVERIFY(category.numChildren() == 1);
    category.addChild(std::move(test2));
    QVERIFY(category.numChildren() == 2);

    category.removeChild(0);
    QVERIFY(category.numChildren() == 1);
    category.removeChild(0);
    QVERIFY(category.numChildren() == 0);
}

#include "CategoryTests.moc"
