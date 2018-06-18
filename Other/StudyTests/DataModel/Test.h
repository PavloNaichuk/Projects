#pragma once

#include "CompositeComponent.h"
#include <memory>
#include <vector>
#include <QString>

class Test : public CompositeComponent
{
public:
    Test();
    Test(QString name);

    bool equals(const Component* comp) const override;

    const QString& name() const;
    void setName(QString name);

    void addChild(std::unique_ptr<Component> component);
    void removeChild(size_t index);
    size_t numChildren() const;
    Component* child(size_t index);
    const Component* child(size_t index) const;

private:
    QString mName;
    std::vector<std::unique_ptr<Component>> mComponents;
};
