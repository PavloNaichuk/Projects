#include "PathFinder.h"
#include "Core/Config.h"
#include "Core/LevelMap.h"
#include "Core/Utilities.h"
#include "Core/GameUnit.h"

#include <vector>
#include <queue>
#include <numeric>
#include <cmath>
#include <algorithm>

namespace
{
    const int kMaxDistance = std::numeric_limits<int>::max();
    struct Node
    {
        Node()
            : mpParent(nullptr)
            , mDist(kMaxDistance)
            , mpMapTile(nullptr)
        {}
        Node* mpParent;
        int mDist;
        MapTile* mpMapTile;
    };

    Node* extractNodeWithMinDist(std::vector<Node*>& nodes);

    int fetchNeighbours(Node mapNodes[Config::TILE_NUM_ROWS][Config::TILE_NUM_COLS],
        const Node& parentNode, Node** ppFetchedNeighbours);

    bool dijkstraSearch(Node mapNodes[Config::TILE_NUM_ROWS][Config::TILE_NUM_COLS],
        Node& startNode, Node& endNode, int gameUnitTileSize);

    bool breadthFirstSearch(Node mapNodes[Config::TILE_NUM_ROWS][Config::TILE_NUM_COLS],
        Node& startNode, Node& endNode, int gameUnitTileSize);

    Path buildPath(Node& endNode);
}

PathFinder::PathFinder(LevelMap& levelMap)
    : mLevelMap(levelMap)
{
}

Path PathFinder::findPath(const GameUnit& source, const GameUnit& target)
{
    const QRectF& sourceBounds = source.bounds();

    int startRow, startCol;
    calcMapTileRowAndCol(sourceBounds.topLeft(), startRow, startCol);

    const QRectF& targetBounds = target.bounds();

    int endRow, endCol;
    calcMapTileRowAndCol(targetBounds.topLeft(), endRow, endCol);

    Q_ASSERT(sourceBounds.width() == sourceBounds.height());
    int gameUnitTileSize = std::ceilf(sourceBounds.width() / float(Config::TILE_SCREEN_SIZE));

    return findShortestPath(gameUnitTileSize, startRow, startCol, endRow, endCol);
}

Path PathFinder::findShortestPath(int gameUnitTileSize, int startTileRow, int startTileCol, int endTileRow, int endTileCol)
{
    Node mapNodes[Config::TILE_NUM_ROWS][Config::TILE_NUM_COLS];

    for (int row = 0; row < Config::TILE_NUM_ROWS; ++row)
        for (int col = 0; col < Config::TILE_NUM_COLS; ++col)
            mapNodes[row][col].mpMapTile = &mLevelMap.mapTileAt(row, col);

    Node& startNode = mapNodes[startTileRow][startTileCol];
    Node& endNode = mapNodes[endTileRow][endTileCol];

    bool pathFound = breadthFirstSearch(mapNodes, startNode, endNode, gameUnitTileSize);
    if (pathFound)
        return buildPath(endNode);

    return {};
}

namespace
{
    int fetchNeighbours(Node mapNodes[Config::TILE_NUM_ROWS][Config::TILE_NUM_COLS],
        const Node& currNode, Node** ppFetchedNeighbours)
    {
        int numNeighbours = 0;
        const MapTile* pCurrTile = currNode.mpMapTile;

        if (pCurrTile->mRow > 0)
        {
            Node& neighbour = mapNodes[pCurrTile->mRow - 1][pCurrTile->mCol];
            ppFetchedNeighbours[numNeighbours++] = &neighbour;
        }
        if (pCurrTile->mRow < Config::TILE_NUM_ROWS)
        {
            Node& neighbour = mapNodes[pCurrTile->mRow + 1][pCurrTile->mCol];
            ppFetchedNeighbours[numNeighbours++] = &neighbour;
        }
        if (pCurrTile->mCol > 0)
        {
            Node& neighbour = mapNodes[pCurrTile->mRow][pCurrTile->mCol - 1];
            ppFetchedNeighbours[numNeighbours++] = &neighbour;
        }
        if (pCurrTile->mCol < Config::TILE_NUM_COLS)
        {
            Node& neighbour = mapNodes[pCurrTile->mRow][pCurrTile->mCol + 1];
            ppFetchedNeighbours[numNeighbours++] = &neighbour;
        }

        return numNeighbours;
    }

    bool dijkstraSearch(Node mapNodes[Config::TILE_NUM_ROWS][Config::TILE_NUM_COLS],
        Node& startNode, Node& endNode, int gameUnitTileSize)
    {
        std::vector<Node*> nodes;
        nodes.reserve(Config::TILE_NUM_ROWS * Config::TILE_NUM_COLS);

        for (int row = 0; row < Config::TILE_NUM_ROWS; ++row)
            for (int col = 0; col < Config::TILE_NUM_COLS; ++col)
                nodes.emplace_back(&mapNodes[row][col]);

        startNode.mpParent = nullptr;
        startNode.mDist = 0;

        const int maxNumNeighbours = 4;
        Node* neighbours[maxNumNeighbours] = {nullptr};

        while (!nodes.empty())
        {
            Node* pCurrNode = extractNodeWithMinDist(nodes);
            if (pCurrNode == &endNode)
                return true;

            const int numNeighbours = fetchNeighbours(mapNodes, *pCurrNode, neighbours);
            for (int i = 0; i < numNeighbours; ++i)
            {
                Node* pNeighbour = neighbours[i];

                if (gameUnitTileSize > pNeighbour->mpMapTile->mClearance);
                else
                {
                    int dist = pCurrNode->mDist + 1 + pNeighbour->mpMapTile->mCost;
                    if (dist < pNeighbour->mDist)
                    {
                        pNeighbour->mDist = dist;
                        pNeighbour->mpParent = pCurrNode;
                    }
                }
            }
        }
        return false;
    }

    bool breadthFirstSearch(Node mapNodes[Config::TILE_NUM_ROWS][Config::TILE_NUM_COLS],
        Node& startNode, Node& endNode, int gameUnitTileSize)
    {
        startNode.mpParent = nullptr;
        startNode.mDist = 0;

        const int maxNumNeighbours = 4;
        Node* neighbours[maxNumNeighbours] = {nullptr};

        std::queue<Node*> nodeQueue;
        nodeQueue.push(&startNode);

        for (; !nodeQueue.empty(); nodeQueue.pop())
        {
            Node* pCurrentNode = nodeQueue.front();
            if (pCurrentNode == &endNode)
                return true;

            const int numNeighbours = fetchNeighbours(mapNodes, *pCurrentNode, neighbours);
            for (int i = 0; i < numNeighbours; ++i)
            {
                Node* pNeighbour = neighbours[i];
                if (pNeighbour->mDist == kMaxDistance)
                {
                    pNeighbour->mpParent = pCurrentNode;
                    pNeighbour->mDist = pCurrentNode->mDist + 1;

                    if (gameUnitTileSize > pNeighbour->mpMapTile->mClearance);
                    else
                        nodeQueue.push(pNeighbour);
                }
            }
        }
        return false;
    }

    Path buildPath(Node& endNode)
    {
        Path path;

        Node* pPrevNode = &endNode;
        path.addMapTile(pPrevNode->mpMapTile);

        Node* pCurrNode = pPrevNode->mpParent;
        if (pCurrNode == nullptr)
            return path;

        int prevRowDir = pCurrNode->mpMapTile->mRow - pPrevNode->mpMapTile->mRow;
        int prevColDir = pCurrNode->mpMapTile->mCol - pPrevNode->mpMapTile->mCol;

        pPrevNode = pCurrNode;
        pCurrNode = pPrevNode->mpParent;

        while (pCurrNode != nullptr)
        {
            const int currRowDir = pCurrNode->mpMapTile->mRow - pPrevNode->mpMapTile->mRow;
            const int currColDir = pCurrNode->mpMapTile->mCol - pPrevNode->mpMapTile->mCol;

            if ((prevRowDir != currRowDir) || (prevColDir != currColDir))
            {
                prevRowDir = currRowDir;
                prevColDir = currColDir;

                path.addMapTile(pPrevNode->mpMapTile);
            }

            pPrevNode = pCurrNode;
            pCurrNode = pPrevNode->mpParent;
        }
        path.addMapTile(pPrevNode->mpMapTile);

        path.reverse();
        return path;
    }

    Node* extractNodeWithMinDist(std::vector<Node*>& nodes)
    {
        auto distComp = [&nodes](Node* pNode1, Node* pNode2)
        {
            return (pNode1->mDist < pNode2->mDist);
        };

        auto it = std::min_element(nodes.begin(), nodes.end(), distComp);
        if (it == nodes.end())
            return nullptr;

        Node* pNode = *it;
        nodes.erase(it);

        return pNode;
    }
}
