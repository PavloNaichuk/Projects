#pragma once

#include "Component.h"
#include "Common.h"

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

enum GameObjectId
{
	PLAYER_STRIKER_ID,
	PLAYER_GOAL_ID,
	ENEMY_STRIKER_ID,
	ENEMY_GOAL_ID,
	PUCK_ID,
	NUM_GAME_OBJECTS
};

enum UIObjectId
{
	SCORE_UI_ID,
	TIMER_UI_ID,
	NUM_UI_OBJECTS
};

using UniqueGameObject = std::unique_ptr<GameObject>;
using GameObjectList = std::vector<UniqueGameObject>;
