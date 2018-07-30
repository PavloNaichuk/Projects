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

bool Category::equals(const Component* comp) const
{
    const Category* category = dynamic_cast<const Category*>(comp);
    if (category == nullptr)
        return false;

    if (name() != category->name())
        return false;

    if (numChildren() != category->numChildren())
        return false;

    for (decltype(numChildren()) childIndex = 0; childIndex < numChildren(); ++childIndex)
    {
        if (!child(childIndex)->equals(category->child(childIndex)))
            return false;
    }
    return true;
}

void Category::visit(Visitor* visitor) const
{
     visitor->visit(this);
}

Type Category::type() const
{
    return CATEGORY;
}
