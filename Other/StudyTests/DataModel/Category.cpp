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
    const Category* category1 = this;
    const Category* category2 = dynamic_cast<const Category*>(comp);
    if (category2 != nullptr)
    {
        if (category1->name() != category2->name())
            return false;
        if (category1->numChildren() != category2->numChildren())
            return false;
        for (decltype(category1->numChildren()) childIndex = 0; childIndex < category1->numChildren(); ++childIndex)
        {
            if (!category1->child(childIndex)->equals(category2->child(childIndex)))
                      return false;
        }
        return true;
    }
    return false;
}
