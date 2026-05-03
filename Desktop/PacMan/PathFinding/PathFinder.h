#pragma once

#include <memory>

class Path;
class LevelMap;
class NavigationGraph;
struct Vector2f;

class PathFinder
{
public:
	PathFinder(std::shared_ptr<LevelMap> levelMap);
	PathFinder(const PathFinder&) = delete;
	PathFinder& operator= (const PathFinder&) = delete;

	std::shared_ptr<Path> FindPath(const Vector2f& startPoint, const Vector2f& endPoint);

private:
	void BuildNavigationGraph(std::shared_ptr<LevelMap> levelMap);
	std::shared_ptr<Path> BreadthFirstSearch(int startTileRow, int startTileCol, int endTileRow, int endTileCol);
		
private:
	std::shared_ptr<NavigationGraph> m_NavGraph;
};