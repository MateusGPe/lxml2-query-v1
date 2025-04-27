#include "UnarySelector.h"

/*static inline void GetChildren(xmlNodePtr node, std::vector<xmlNodePtr> &stack)
{
	for (xmlNodePtr child = node->children; child != nullptr; child = child->next)
	{
		if (child->type == XML_ELEMENT_NODE)
		{
			stack.push_back(child);
		}
	}
}

bool UnarySelector::hasDescendantMatch(xmlNodePtr node, SelectorPtr selector) const
{
	std::vector<xmlNodePtr> stack;
	stack.reserve(128);
	GetChildren(node, stack);

	while (!stack.empty())
	{
		xmlNodePtr current = stack.back();
		stack.pop_back();

		if (selector->matches(current))
		{
			return true;
		}

		if (current->type == XML_ELEMENT_NODE)
		{
			GetChildren(current, stack);
		}
	}
	return false;
}

bool UnarySelector::hasDescendantMatch(xmlNodePtr node, SelectorPtr selector) const
{
	for (xmlNodePtr child = node->children; child != nullptr; child = child->next)
	{
		if (child->type != XML_ELEMENT_NODE)
		{
			continue;
		}
		if (selector->matches(child))
		{
			return true;
		}
		if (hasDescendantMatch(child, selector))
		{
			return true;
		}
	}
	return false;
}
*/

static inline bool hasDescendantMatch(xmlNodePtr node, SelectorPtr selector)
{
	if (node == nullptr || node->type != XML_ELEMENT_NODE)
	{
		return false;
	}

	std::vector<xmlNodePtr> stack;
	stack.reserve(256);
	stack.push_back(node);

	while (!stack.empty())
	{
		xmlNodePtr current = stack.back();
		stack.pop_back();

		for (xmlNodePtr child = current->children; child != nullptr; child = child->next)
		{
			if (child->type != XML_ELEMENT_NODE)
			{
				continue;
			}

			if (selector->matches(child))
			{
				return true;
			}

			stack.push_back(child);
		}
	}

	return false;
}

static inline bool hasChildMatch(xmlNodePtr node, SelectorPtr selector)
{
	for (xmlNodePtr child = node->children; child != nullptr; child = child->next)
	{
		if (child->type != XML_ELEMENT_NODE)
		{
			continue;
		}
		if (selector->matches(child))
		{
			return true;
		}
	}
	return false;
}

bool UnarySelector::matches(xmlNodePtr node) const
{
	switch (operator_)
	{
	case Operator::Not:
		return !selector_->matches(node);
	case Operator::HasDescendant:
		if (node->type != XML_ELEMENT_NODE || node->children == nullptr)
		{
			return false;
		}
		return hasDescendantMatch(node, selector_);
	case Operator::HasChild:
		if (node->type != XML_ELEMENT_NODE)
		{
			return false;
		}
		return hasChildMatch(node, selector_);
	default:
		return false;
	}
}

std::string UnarySelector::toString() const
{
	switch (operator_)
	{
	case Operator::Not:
		return ":not(" + selector_->toString() + ")";
	case Operator::HasDescendant:
		return ":has(" + selector_->toString() + ")";
	case Operator::HasChild:
		return ":has-child(" + selector_->toString() + ")";
	default:
		break;
	}
	return std::string();
}
