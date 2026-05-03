#include "Path.h"
#include <cassert>

Path::Path(const std::vector<const GraphNode*>& nodes)
	: m_Nodes(nodes)
	, m_CurrNodeIndex(0)
{
}

Path::Path(std::vector<const GraphNode*>&& nodes)
	: m_Nodes(std::move(nodes))
	, m_CurrNodeIndex(0)
{
}

const GraphNode* Path::GetCurrentNode() const
{
	return m_Nodes[m_CurrNodeIndex];
}

void Path::SetNextNode()
{
	assert(!ReachedEnd());
	++m_CurrNodeIndex;
}

bool Path::ReachedEnd() const
{
	return (m_CurrNodeIndex == m_Nodes.size());
}

void Path::Reset()
{
	m_CurrNodeIndex = 0;
}
