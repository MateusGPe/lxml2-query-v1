#include "TagSelector.h"
#include "Node.h"

#include <iostream>
#include <iomanip>

namespace
{
	static const std::unordered_map<std::string_view, bool> togglabe{
		{"optgroup", true},
		{"menuitem", true},
		{"fieldset", true},
		{"button", false},
		{"input", false},
		{"select", false},
		{"textarea", false},
		{"option", false},
	};
}

static inline bool langMatches(xmlNodePtr node, std::string_view lang)
{
	if (node->type != XML_ELEMENT_NODE)
	{
		return false;
	}

	std::string lang_dash(lang);
	lang_dash += "-";

	for (xmlNodePtr cur = node; cur != nullptr; cur = cur->parent)
	{
		if (cur->type != XML_ELEMENT_NODE && cur->type != XML_DOCUMENT_NODE)
		{
			continue;
		}

		std::string cur_lang = GetAttributeValue(cur, "lang").value_or("");

		if (cur_lang == lang || cur_lang.find(lang_dash) == 0)
		{
			return true;
		}
	}
	return false;
}

static inline bool hasLegendInPreviousSiblings(xmlNodePtr node)
{
	for (xmlNodePtr cur = node->prev; cur != nullptr; cur = cur->prev)
	{
		if (cur->type != XML_ELEMENT_NODE)
		{
			continue;
		}
		if (IsEqualElement(cur, "legend"))
		{
			return true;
		}
	}
	return false;
}

static inline bool inDisabledFieldset(xmlNodePtr node)
{
	for (xmlNodePtr cur = node; cur != nullptr && cur->parent != nullptr; cur = cur->parent)
	{
		if (cur->type != XML_ELEMENT_NODE)
		{
			continue;
		}
		if ((IsEqualElement(cur->parent, "fieldset")) && (xmlHasProp(cur->parent, (const xmlChar *)"disabled") != nullptr))
		{
			if (IsEqualElement(cur, "legend") || hasLegendInPreviousSiblings(cur))
			{
				return true;
			}
		}
	}
	return false;
}

static inline bool disabledMatch(xmlNodePtr node)
{
	if (node->type != XML_ELEMENT_NODE)
	{
		return false;
	}

	auto it = togglabe.find(getLowerElementName(node));
	if (it == togglabe.end())
	{
		return false;
	}

	if (xmlHasProp(node, (const xmlChar *)"disabled") != nullptr)
	{
		return true;
	}
	else if (it->second)
	{
		return false;
	}

	return it->second && inDisabledFieldset(node);
}

static inline bool checkedMatch(xmlNodePtr node)
{
	if (node->type != XML_ELEMENT_NODE)
	{
		return false;
	}

	if (IsEqualElement(node, "input") || IsEqualElement(node, "menuitem"))
	{
		std::string type = GetAttributeValue(node, "type").value_or("");
		if (type == "radio" || type == "checkbox")
		{
			return xmlHasProp(node, (const xmlChar *)"checked") != nullptr;
		}
	}
	else if (IsEqualElement(node, "option"))
	{
		return xmlHasProp(node, (const xmlChar *)"selected") != nullptr;
	}
	return false;
}

bool TagSelector::matches(xmlNodePtr node) const
{
	if (node == nullptr)
	{
		return false;
	}
	switch (operator_)
	{
	case Operator::Empty:
	{
		return NodeIsEmpty(node);
	}
	case Operator::OnlyChild:
	{
		return NodeIsOnlyChild(node, oftype_);
	}
	case Operator::NthChild:
	{
		auto [node_count, node_pos] = NodeSiblingsCount(node, oftype_, last_);

		if (node_pos == -1)
		{
			return false;
		}

		if (last_)
		{
			node_pos = node_count - node_pos + 1;
		}

		node_pos -= b_;
		if (a_ == 0)
		{
			return node_pos == 0;
		}

		if (a_ == node_pos)
		{
			return true;
		}

		return ((node_pos % a_) == 0) && ((node_pos / a_) >= 0);
	}
	case Operator::Tag:
		return node->type == XML_ELEMENT_NODE && IsEqualElement(node, refvalue_);
	case Operator::Root:
		return xmlDocGetRootElement(node->doc) == node;
	case Operator::Link:
	{
		const std::string tname(reinterpret_cast<const char *>(node->name));
		return (xmlHasProp(node, (const xmlChar *)"href") != nullptr) && (tname == "a" || tname == "area" || tname == "link");
	}
	case Operator::Lang:
		return langMatches(node, refvalue_);
	case Operator::Enabled:
		return !disabledMatch(node);
	case Operator::Disabled:
		return disabledMatch(node);
	case Operator::Checked:
		return checkedMatch(node);
	default:
		return false;
	}
}

std::string TagSelector::toString() const
{
	std::stringstream ss;

	switch (operator_)
	{
	case Operator::Empty:
		ss << ":empty()";
		break;
	case Operator::OnlyChild:
		ss << ":only-" << (oftype_ ? "of-type()" : "child()");
		break;
	case Operator::NthChild:
		if (a_ == 0 && b_ == 1)
		{
			ss << (last_ ? ":last-" : ":first-") << (oftype_ ? "of-type" : "child") << "()";
		}
		else if (a_ == 2 && b_ == 0)
		{
			ss << ":nth-" << (last_ ? "last-" : "") << (oftype_ ? "of-type" : "child") << "(even)";
		}
		else if (a_ == 2 && b_ == 1)
		{
			ss << ":nth-" << (last_ ? "last-" : "") << (oftype_ ? "of-type" : "child") << "(odd)";
		}
		else
		{
			ss << ":nth-" << (last_ ? "last-" : "") << (oftype_ ? "of-type" : "child") << "(";
			ss << a_ << "n";
			if (b_ != 0)
			{
				ss << (b_ > 0 ? "+" : "") << b_;
			}
			ss << ")";
		}
		break;
	case Operator::Tag:
		ss << refvalue_;
		break;
	case Operator::Lang:
		ss << ":lang(" << refvalue_ << ")";
		break;
	case Operator::Root:
		ss << ":root()";
		break;
	case Operator::Link:
		ss << ":link()";
		break;
	case Operator::Enabled:
		ss << ":enabled()";
		break;
	case Operator::Disabled:
		ss << ":disabled()";
		break;
	case Operator::Checked:
		ss << ":checked()";
		break;
	default:
		break;
	}
	return ss.str();
}
