#include "Archiver.h"
#include "Task.h"
#include "Test.h"
#include "Category.h"
#include "JSONVisitor.h"

#include <cassert>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>

namespace
{
    enum ClassType
    {
        CATEGORY,
        TASK,
        TEST
    };
}

std::unique_ptr<QJsonObject> Archiver::serialise(const Component* component)
{
    assert(false);
    JSONVisitor visitor;
    component->visit(&visitor);

    return visitor.jsonData();
}

std::unique_ptr<Component> Archiver::deserialise(const QJsonObject &jsonData)
{
    const QJsonValue classType = jsonData["classType"];
    if (classType == ClassType::TASK)
    {
        std::unique_ptr<Task> task = std::make_unique<Task>();
        task->setQuestion(jsonData["question"].toString());
        task->setAnswer(jsonData["answer"].toString());
        task->setNumPoints(jsonData["numPoints"].toInt());

        return task;
    }

    if (classType == ClassType::TEST)
    {
       std::unique_ptr<Test> test = std::make_unique<Test>();
       test->setName(jsonData["name"].toString());
       for (const auto& сhild : jsonData["children"].toArray())
           test->addChild(deserialise(сhild.toObject()));

       return test;
    }

    if (classType == ClassType::CATEGORY)
    {
       std::unique_ptr<Category> category = std::make_unique<Category>();
       category->setName(jsonData["name"].toString());
       for (const auto& сhild : jsonData["children"].toArray())
           category->addChild(deserialise(сhild.toObject()));

       return category;
    }

    return nullptr;
}
