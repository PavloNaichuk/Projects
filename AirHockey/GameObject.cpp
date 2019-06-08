#include "pch.h"
#include "GameObject.h"

GameObject::GameObject(GameObjectId gameObjectId)
	: mGameObjectId(gameObjectId)
{
}

void GameObject::AddComponent(ComponentPointer component) 
{
	mComponents.emplace(component->GetId(), std::move(component));
}

