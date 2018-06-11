#include <QString>
#include <QtTest/QtTest>
#include "Category.h"
#include "Test.h"
#include "Task.h"
#include "Archiver.h"

class UnitTests : public QObject
{
    Q_OBJECT

public:
    UnitTests();

private Q_SLOTS:
    void testOne();
    void testSecond();
};

UnitTests::UnitTests()
{
}

void UnitTests::testOne()
{
    Task task1;
    task1.setQuestion("Запитання 1");
    task1.setAnswer("Відповідь 1");
    task1.setNumPoints(3);


    auto jsonObject = Archiver::serialise(&task1);
    //std::unique_ptr<Task> task2 = Archiver::deserialise(jsonObject);
    //areEqual(task1, task2);


    //QVERIFY(jsonText == string);
}
void UnitTests::testSecond()
{

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


QTEST_APPLESS_MAIN(UnitTests)

#include "UnitTests.moc"

