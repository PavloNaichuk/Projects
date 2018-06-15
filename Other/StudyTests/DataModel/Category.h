#pragma once

#include "CompositeComponent.h"
#include <vector>
#include <QString>

class Category : public CompositeComponent
{
public:
    Category();
    Category(QString name);

    const QString& name() const;
    void setName(QString name);

    void addChild(std::unique_ptr<Component> component);
    void removeChild(size_t index);
    size_t numChildren() const;
    Component* child(size_t index);
    const Component* child(size_t index) const;
    bool equals(const Component* comp) const;

private:
    QString mName;
    std::vector<std::unique_ptr<Component>> mComponents;
};
