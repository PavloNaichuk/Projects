#include "Archiver.h"
#include "Task.h"
#include "Test.h"
#include "Category.h"
#include <cassert>
#include <QJsonObject>
#include <QJsonArray>

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
    if (json.contains("ClassType::TEST")!= nullptr)
    {
        mQuestion =  (*jsonObject)["question"];
        (*jsonObject)["answer"] = task->answer();
        (*jsonObject)["numPoints"] = task->numPoints();

    }

    return nullptr;
}
