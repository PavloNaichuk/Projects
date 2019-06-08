#pragma once

#include "Component.h"
#include <memory>
#include <map>

class GameObject
{
public:
	using GameObjectId = unsigned;

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

	void AddComponent(UniqueComponent component);

private:
	using ComponentMap = std::map<Component::ComponentId, UniqueComponent>;

	GameObjectId mGameObjectId;
	ComponentMap mComponents;
};

using GameObjectUniquePointer = std::unique_ptr<GameObject>;
