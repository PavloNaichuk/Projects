#pragma once
#include "Visitor.h"
#include "Task.h"
#include "Test.h"
#include "Category.h"

#include <QJsonObject>

class JSONVisitor : public Visitor
{
public:
    void visit(const Task* task)  override;
    void visit(const Test* test) override;
    void visit(const Category* category) override;
    const QJsonObject& jsonData();

private:
    std::unique_ptr<QJsonObject> mJsonData;
};

