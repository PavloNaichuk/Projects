#include "Category.h"

Category::Category()
{
}

Category::Category(QString name)
    : mName(std::move(name))
{
}

const QString& Category::name() const
{
    return mName;
}

void Category::setName(QString name)
{
    mName = std::move(name);
}

void Category::addChild(std::unique_ptr<Component> component)
{
     mComponents.push_back(std::move(component));
}

void Category::removeChild(size_t index)
{
     mComponents.erase(mComponents.begin() + index);
}

size_t Category::numChildren() const
{
    return mComponents.size();
}

Component* Category::child(size_t index)
{
    return mComponents[index].get();
}

const Component* Category::child(size_t index) const
{
    return mComponents[index].get();
}
