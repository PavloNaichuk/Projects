#include "Test.h"

Test::Test()
{
}

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

bool Test::equals(const Component* comp) const
{
    const Test* test1 = this;
    const Test* test2 = dynamic_cast<const Test*>(comp);
    if (test2 != nullptr)
    {
        if (test1->name() != test2->name())
            return false;
        if (test1->numChildren() != test2->numChildren())
            return false;
        for (decltype(test1->numChildren()) childIndex = 0; childIndex < test1->numChildren(); ++childIndex)
        {
            if (!test1->child(childIndex)->equals(test2->child(childIndex)))
                      return false;
        }
        return true;
    }
    return false;
}
