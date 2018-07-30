#pragma once

#include "CompositeComponent.h"
#include "Visitor.h"
#include <memory>
#include <vector>

class Test : public CompositeComponent
{
public:
    Test();
    Test(QString name);

    bool equals(const Component* comp) const override;
    void visit(Visitor* visitor) const override;
    Type type() const override;
    const QString& name() const override;
    void setName(QString name) override;

    void addChild(std::unique_ptr<Component> component);
    void removeChild(size_t index);
    size_t numChildren() const;
    Component* child(size_t index);
    const Component* child(size_t index) const;

private:
    QString mName;
    std::vector<std::unique_ptr<Component>> mComponents;
};
