#include "Test.h"

Test::Test(QString name)
    : mName(std::move(name))
{
}

const QString& Test::name() const
{
    return mName;
}

void Test::setName(QString name)
{
    mName = std::move(name);
}

void Test::addChild(std::unique_ptr<Component> component)
{
     mComponents.push_back(std::move(component));
}

void Test::removeChild(size_t index)
{
     mComponents.erase(mComponents.begin() + index);
}

size_t Test::numChildren() const
{
    return mComponents.size();
}

Component* Test::child(size_t index)
{
    return mComponents[index].get();
}

const Component* Test::child(size_t index) const
{
    return mComponents[index].get();
}
