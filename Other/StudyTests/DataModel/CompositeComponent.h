#pragma once

#include "Component.h"
#include <memory>

class CompositeComponent : public Component
{
public:
    virtual void addChild(std::unique_ptr<Component> component) = 0;
    virtual void removeChild(size_t index) = 0;
    virtual size_t numChildren() const = 0;
    virtual Component* child(size_t index) = 0;
    virtual const Component* child(size_t childIndex) const = 0;
};
