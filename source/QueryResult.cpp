#include "QueryResult.h"
#include "Parser.h"
#include "Node.h"

QueryResult::QueryResult(xmlNodePtr apNode)
{
	if (apNode != nullptr)
	{
		nodes_.insert(apNode);
	}
}

QueryResult::iterator QueryResult::begin() { return QueryResult::iterator(nodes_.begin()); }
QueryResult::iterator QueryResult::end() { return QueryResult::iterator(nodes_.end()); }

std::optional<Node> QueryResult::operator[](std::size_t index) const
{
	if (index >= nodes_.size())
	{
		return std::nullopt;
	}

	auto it = nodes_.cbegin();
	std::advance(it, index);
	return Node(*it);
}

QueryResult QueryResult::Find(std::string_view selector) const
{
	return Find(SelectorParser::Create(selector));
}

QueryResult QueryResult::Find(const SelectorPtr &selector) const
{
	NodeSet ret;
	for (xmlNodePtr pNode : nodes_)
	{
		NodeSet matched = selector->MatchAll(pNode);
		ret.merge(matched);
	}
	return QueryResult(ret);
}

QueryResult QueryResult::Find(xmlNodePtr node, std::string_view selector)
{
	return Find(node, SelectorParser::Create(selector));
}

QueryResult QueryResult::Find(xmlNodePtr node, const SelectorPtr &selector)
{
	return QueryResult(selector->MatchAll(node));
}

std::optional<Node> QueryResult::at(std::size_t i) const
{
	return (*this)[i];
}

std::size_t QueryResult::size() const
{
	return nodes_.size();
}

std::vector<Node> QueryResult::toVector() const
{
	std::vector<Node> nodes;
	nodes.reserve(nodes_.size());
	std::transform(nodes_.cbegin(), nodes_.cend(), std::back_inserter(nodes), [](xmlNodePtr node)
				   { return Node(node); });
	return nodes;
}