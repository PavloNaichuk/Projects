#pragma once

class Task;
class Test;
class Category;

class Visitor
{
public:
    virtual ~Visitor();

    virtual void visit(const Task* task)  = 0;
    virtual void visit(const Test* test)  = 0;
    virtual void visit(const Category* category) = 0;
};
