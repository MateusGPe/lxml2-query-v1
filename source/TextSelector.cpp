#include "TextSelector.h"
#include "Node.h"

bool TextSelector::matches(xmlNodePtr node) const
{
	if (!node)
		return false;
	std::string text;
	switch (operator_)
	{
	case Operator::Contains:
		text = normalizeHtmlText(NodeText(node));
		break;
	case Operator::OwnContains:
		text = normalizeHtmlText(NodeOwnText(node));
		break;
	case Operator::Matches:
	{
		text = NodeText(node);
		return std::regex_search(text, std::regex(value_));
	}
	case Operator::MatchesOwn:
	{
		text = NodeOwnText(node);
		return std::regex_search(text, std::regex(value_));
	}
	default:
		return false;
	}

	text = toLower(text);
	return text.find(value_) != std::string::npos;
}

std::string TextSelector::toString() const
{
	switch (operator_)
	{
	case Operator::Contains:
		return ":contains(" + value_ + ")";
	case Operator::OwnContains:
		return ":contains-own(" + value_ + ")";
	case Operator::Matches:
		return ":matches(" + value_ + ")";
	case Operator::MatchesOwn:
		return ":matches-own(" + value_ + ")";
	default:
		return std::string();
	}
}
