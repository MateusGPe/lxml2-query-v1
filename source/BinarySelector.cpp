#include "BinarySelector.h"

static inline bool descendant(SelectorPtr lselector_, SelectorPtr rselector_, xmlNodePtr node)
{
	if (!rselector_->matches(node) || node->type != XML_ELEMENT_NODE)
	{
		return false;
	}

	for (xmlNodePtr parent = node->parent; parent != nullptr; parent = parent->parent)
	{
		if (lselector_->matches(parent))
		{
			return true;
		}
	}
	return false;
}

static inline bool adjacent(SelectorPtr lselector_, SelectorPtr rselector_, xmlNodePtr node, bool adjacent_)
{
	if (!rselector_->matches(node) || node->type != XML_ELEMENT_NODE)
	{
		return false;
	}
	for (xmlNodePtr sibling = node->prev; sibling; sibling = sibling->prev)
	{
		if (sibling->type != XML_ELEMENT_NODE)
		{
			continue;
		}
		if (adjacent_)
		{
			return lselector_->matches(sibling);
		}
		if (lselector_->matches(sibling))
		{
			return true;
		}
	}
	return false;
}

bool BinarySelector::matches(xmlNodePtr node) const
{
	if (!node)
	{
		return false;
	}
	switch (operator_)
	{
	case Operator::Union:
	{
		if (lselector_->matches(node))
		{
			return true;
		}
		return rselector_->matches(node);
	}
	case Operator::Intersection:
		if (!lselector_->matches(node))
		{
			return false;
		}
		return rselector_->matches(node);
	case Operator::Child:
		if (!rselector_->matches(node) || node->parent == nullptr)
		{
			return false;
		}
		return lselector_->matches(node->parent) && node->parent->type == XML_ELEMENT_NODE;
	case Operator::Descendant:
		return descendant(lselector_, rselector_, node);
	case Operator::Adjacent:
		return adjacent(lselector_, rselector_, node, adjacent_);
	default:
		return false;
	}

	return false;
}

std::string BinarySelector::toString() const
{
	switch (operator_)
	{
	case Operator::Union:
		return lselector_->toString() + ", " + rselector_->toString();
	case Operator::Intersection:
	{
		return lselector_->toString() + " " + rselector_->toString();
	}
	case Operator::Descendant:
	{
		return lselector_->toString() + " " + rselector_->toString();
	}
	case Operator::Child:
		return lselector_->toString() + " > " + rselector_->toString();
	case Operator::Adjacent:
		return lselector_->toString() + (adjacent_ ? " + " : " ~ ") + rselector_->toString();
	default:
		break;
	}
	return std::string();
}
