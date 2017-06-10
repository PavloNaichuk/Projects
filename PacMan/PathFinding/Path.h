#pragma once

#include <vector>

struct GraphNode;

class Path
{
public:
	Path(const std::vector<const GraphNode*>& nodes);
	Path(std::vector<const GraphNode*>&& nodes);

	const GraphNode* GetCurrentNode() const;
	void SetNextNode();
	bool ReachedEnd() const;
	void Reset();

private:
	std::vector<const GraphNode*> m_Nodes;
	unsigned m_CurrNodeIndex;
};