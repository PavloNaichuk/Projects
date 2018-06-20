#pragma once

class Visitor;

class Component
{
public:
    virtual ~Component();
    virtual bool equals(const Component* comp) const = 0;
    virtual void visit(Visitor* visitor) const = 0;
};
