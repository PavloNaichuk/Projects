#include <QString>
#include <QtTest/QtTest>
#include "Task.h"

class TaskTests : public QObject
{
    Q_OBJECT

public:
    TaskTests();

private Q_SLOTS:
    void testTask();
};

TaskTests::TaskTests()
{
}

void TaskTests::testTask()
{
    Task task;
    task.setName("Завдання 1");

    QVERIFY(task.name() == "Завдання 1");
    task.setQuestion("Запитання");
    task.setAnswer("Відповідь");
    task.setNumPoints(3);

    QVERIFY(task.name() == "Завдання 1");
    QVERIFY(task.question() == "Запитання");
    QVERIFY(task.answer() == "Відповідь");
    QVERIFY(task.numPoints() == 3);

    QVERIFY(task.type() == TASK);
}

#include "TaskTests.moc"
