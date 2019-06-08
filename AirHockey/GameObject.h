#pragma once

#include "Component.h"
#include <memory>
#include <map>

class GameObject
{
public:
	using GameObjectId = unsigned;
	using ComponentPointer = std::unique_ptr<Component>;

	GameObject(GameObjectId gameObjectId);

	GameObjectId GetId() const { return mGameObjectId; }

	template <class ComponentType>
	ComponentType* GetComponent(Component::ComponentId componentId) 
	{
		auto it = mComponents.find(componentId);
		if (it != mComponents.end())
			return static_cast<ComponentType*>(it->second.get());

		return nullptr;
	}

	template <class ComponentType>
	const ComponentType* GetComponent(Component::ComponentId componentId) const 
	{
		auto it = mComponents.find(componentId);
		if (it != mComponents.end())
			return static_cast<ComponentType*>(it->second.get());

		return nullptr;
	}

	void AddComponent(ComponentPointer component);

private:
	using ComponentMap = std::map<Component::ComponentId, ComponentPointer>;

	GameObjectId mGameObjectId;
	ComponentMap mComponents;
};
