#include "JSONVisitor.h"
#include <memory>
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

void JSONVisitor::visit(const Task* task)
{
    mJsonData = std::make_unique<QJsonObject>();
    (*mJsonData)["classType"] = ClassType::TASK;
    (*mJsonData)["question"] = task->question();
    (*mJsonData)["answer"] = task->answer();
    (*mJsonData)["numPoints"] = task->numPoints();
}

void JSONVisitor::visit(const Test* test)
{
    mJsonData = std::make_unique<QJsonObject>();
    (*mJsonData)["classType"] = ClassType::TEST;
    (*mJsonData)["name"] = test->name();

    QJsonArray children;
    for (decltype(test->numChildren()) childIndex = 0; childIndex < test->numChildren(); ++childIndex)
    {
        JSONVisitor childVisitor;
        test->child(childIndex)->visit(&childVisitor);

        children.push_back(childVisitor.jsonData());
    }
    (*mJsonData)["children"] = children;
}

void JSONVisitor::visit(const Category* category)
{
    mJsonData = std::make_unique<QJsonObject>();
    (*mJsonData)["classType"] = ClassType::CATEGORY;
    (*mJsonData)["name"] = category->name();

    QJsonArray children;
    for (decltype(category->numChildren()) childIndex = 0; childIndex < category->numChildren(); ++childIndex)
    {
        JSONVisitor childVisitor;
        category->child(childIndex)->visit(&childVisitor);
        auto childData = childVisitor.jsonData();
        children.push_back(childData);
    }
    (*mJsonData )["children"] = children;
}

const QJsonObject& JSONVisitor::jsonData()
{
    return *mJsonData;
}
