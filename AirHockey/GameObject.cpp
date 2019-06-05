#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(GameObjectId gameObjectId)
	: mGameObjectId(gameObjectId)
{
}

template <class ComponentType>
ComponentType* GameObject::GetComponent(Component::ComponentId componentId)
{
	auto it = mComponents.find(componentId);
	if (it != mComponents.end()) 
		return static_cast<ComponentType*>(it->second.get());

	return nullptr;
}

void GameObject::AddComponent(ComponentPointer component) 
{
	mComponents.emplace(component->GetId(), std::move(component));
}

