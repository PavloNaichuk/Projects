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
    //areEqual()


    QVERIFY(jsonText == string);
}
void UnitTests::testSecond()
{

}

bool areEqual(const Component* comp1, const Component* comp2){}


QTEST_APPLESS_MAIN(UnitTests)

#include "UnitTests.moc"

