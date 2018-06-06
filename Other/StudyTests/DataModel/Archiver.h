#pragma once

#include <memory>


class Component;
class QJsonObject;

class Archiver
{
public:
    static std::unique_ptr<QJsonObject> serialise(const Component* component);
    static std::unique_ptr<Component> deserialise(const QJsonObject& jsonData);
};
