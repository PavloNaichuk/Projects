#include "Archiver.h"
#include "Task.h"
#include "Test.h"
#include "Category.h"
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
    const Task* task = dynamic_cast<const Task*>(component);
    if (task != nullptr)
    {
        std::unique_ptr<QJsonObject> jsonObject = std::make_unique<QJsonObject>();
        (*jsonObject)["classType"] = ClassType::TASK;
        (*jsonObject)["question"] = task->question();
        (*jsonObject)["answer"] = task->answer();
        (*jsonObject)["numPoints"] = task->numPoints();

        return jsonObject;
    }

    const Test* test = dynamic_cast<const Test*>(component);
    if (test != nullptr)
    {
        std::unique_ptr<QJsonObject> jsonObject = std::make_unique<QJsonObject>();
        (*jsonObject)["classType"] = ClassType::TEST;
        (*jsonObject)["name"] = test->name();

        QJsonArray children;
        for (decltype(test->numChildren()) childIndex = 0; childIndex < test->numChildren(); ++childIndex)
        {
            std::unique_ptr<QJsonObject> childJsonObject = serialise(test->child(childIndex));
            children.push_back(*childJsonObject);
        }
        (*jsonObject)["children"] = children;

        return jsonObject;
    }

    const Category* category = dynamic_cast<const Category*>(component);
    if (category != nullptr)
    {
        std::unique_ptr<QJsonObject> jsonObject = std::make_unique<QJsonObject>();
        (*jsonObject)["classType"] = ClassType::CATEGORY;
        (*jsonObject)["name"] = category->name();

        QJsonArray children;
        for (decltype(category->numChildren()) childIndex = 0; childIndex < category->numChildren(); ++childIndex)
        {
            std::unique_ptr<QJsonObject> childJsonObject = serialise(category->child(childIndex));
            children.push_back(*childJsonObject);
        }
        (*jsonObject )["children"] = children;

        return jsonObject;
    }

    assert(false);
    return nullptr;
}

std::unique_ptr<Component> Archiver::deserialise(const QJsonObject& jsonData)
{
    const int classType = jsonData["classType"];
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
