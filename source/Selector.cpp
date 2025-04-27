#include "Selector.h"
#include "Node.h"

Selector::NodeSet Selector::Filter(NodeSet nodes) const
{
	NodeSet result;
	for (xmlNodePtr node : nodes)
	{
		if (matches(node))
		{
			result.insert(node);
		}
	}
	return result;
}

Selector::NodeSet Selector::MatchAll(xmlNodePtr node) const
{
	NodeSet result;

	if (node == nullptr)
		return result;

	matchAllInto(node, result);
	return result;
}
/*
void Selector::matchAllInto(xmlNodePtr node, NodeSet &nodes) const
{
	if (matches(node))
	{
		nodes.insert(node);
	}

	if (node->type != XML_ELEMENT_NODE)
	{
		return;
	}

	for (xmlNodePtr child = node->children; child != nullptr; child = child->next)
	{
		matchAllInto(child, nodes);
	}
}*/
void Selector::matchAllInto(xmlNodePtr node, NodeSet &nodes) const
{
	if (node == nullptr)
	{
		return;
	}

	std::vector<xmlNodePtr> stack;
	stack.reserve(256);
	stack.push_back(node);

	while (!stack.empty())
	{
		xmlNodePtr current = stack.back();
		stack.pop_back();

		if (matches(current))
		{
			nodes.insert(current);
		}

		if (current->type == XML_ELEMENT_NODE)
		{
			for (xmlNodePtr child = current->children; child != nullptr; child = child->next)
			{
				if (child->type == XML_ELEMENT_NODE)
				{
					stack.push_back(child);
				}
			}
		}
	}
}