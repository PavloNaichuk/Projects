#pragma once
#include <QString>

enum Type
{
    CATEGORY,
    TASK,
    TEST
};

class Visitor;

class Component
{
public:
    virtual ~Component();
    virtual bool equals(const Component* comp) const = 0;
    virtual void visit(Visitor* visitor) const = 0;
    virtual Type type() const = 0;
    virtual const QString& name() const = 0;
    virtual void setName(QString name) = 0;
};
