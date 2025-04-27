#include "Node.h"
#include "QueryResult.h"

std::optional<Node> Node::Parent() const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}

	if (node_->parent == nullptr)
	{
		return std::nullopt;
	}

	return Node(node_->parent);
}

std::optional<Node> Node::NextSibling() const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}

	xmlNodePtr sibling = node_->next;

	while (sibling != nullptr)
	{
		if (sibling->type == XML_ELEMENT_NODE)
		{
			return Node(sibling);
		}
		sibling = sibling->next;
	}

	return std::nullopt;
}

std::optional<Node> Node::PrevSibling() const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}

	xmlNodePtr sibling = node_->prev;

	while (sibling != nullptr)
	{
		if (sibling->type == XML_ELEMENT_NODE)
		{
			return Node(sibling);
		}
		sibling = sibling->prev;
	}

	return std::nullopt;
}

std::optional<Node> Node::ChildAt(std::size_t i) const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}

	if (node_->type != XML_ELEMENT_NODE)
	{
		return std::nullopt;
	}

	std::size_t count = 0;
	for (xmlNodePtr child = node_->children; child != nullptr; child = child->next)
	{
		if (child->type != XML_ELEMENT_NODE)
		{
			continue;
		}
		if (count == i)
		{
			return Node(child);
		}
		count++;
	}

	return std::nullopt;
}

unsigned long Node::ChildNum() const
{
	if (node_->type != XML_ELEMENT_NODE)
	{
		return 0;
	}

	return xmlChildElementCount(node_);
}

std::optional<std::string> Node::Attribute(const std::string &key) const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}

	if (node_->type != XML_ELEMENT_NODE)
	{
		return std::nullopt;
	}
	xmlChar *val = xmlGetProp(node_, reinterpret_cast<const xmlChar *>(key.c_str()));

	if (val != nullptr)
	{
		std::string ret = (char *)val;
		xmlFree(val);
		return ret;
	}

	return std::nullopt;
}

std::optional<std::string> Node::Text() const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}
	return NodeText(node_);
}

std::optional<std::string> Node::OwnText() const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}
	return NodeOwnText(node_);
}

std::optional<std::string> Node::TextNormalized() const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}
	return normalizeHtmlText(NodeText(node_));
}

std::optional<std::string> Node::OwnTextNormalized() const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}
	return normalizeHtmlText(NodeOwnText(node_));
}

std::optional<std::string> Node::TagName() const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}
	if (node_->type != XML_ELEMENT_NODE)
	{
		return std::nullopt;
	}

	return toLower(normalizeHtmlText(reinterpret_cast<const char *>(node_->name)));
}

Node::AttributeList Node::Attributes() const
{
	AttributeList attributes;

	if (node_ == nullptr)
	{
		return attributes;
	}

	xmlAttr *attr = node_->properties;

	while (attr && attr->name && attr->children)
	{
		attributes.emplace_back(std::string(reinterpret_cast<const char *>(attr->name)), GetPropNodeValue(node_->doc, attr->children));
		attr = attr->next;
	}
	return attributes;
}

QueryResult Node::Siblings() const
{
	QueryResult::NodeSet nodes;

	if (node_ == nullptr)
	{
		return nodes;
	}

	if (node_->parent == nullptr)
	{
		return nodes;
	}

	for (xmlNodePtr siblings = node_->parent->children; siblings != nullptr; siblings = siblings->next)
	{
		if (siblings->type == XML_ELEMENT_NODE && siblings != node_)
		{
			nodes.emplace(siblings);
		}
	}
	return nodes;
}

QueryResult Node::Find(std::string_view selector) const
{
	return QueryResult::Find(node_, selector);
}

QueryResult Node::Find(const SelectorPtr selector) const
{
	return QueryResult::Find(node_, selector);
}