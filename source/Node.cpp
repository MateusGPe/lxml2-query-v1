#include "Node.h"
#include "QueryResult.h"
#include <libxml/xmlsave.h>

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

QueryResult Node::NextSiblings() const
{
	QueryResult::NodeSet nodes;

	if (node_ == nullptr)
	{
		return nodes;
	}

	xmlNodePtr sibling = node_->next;

	while (sibling != nullptr)
	{
		if (sibling->type == XML_ELEMENT_NODE)
		{
			nodes.emplace(sibling);
		}
		sibling = sibling->next;
	}

	return nodes;
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

QueryResult Node::PrevSiblings() const
{
	QueryResult::NodeSet nodes;

	if (node_ == nullptr)
	{
		return nodes;
	}

	xmlNodePtr sibling = node_->prev;

	std::vector<Node> temp_nodes;
	while (sibling != nullptr)
	{
		if (sibling->type == XML_ELEMENT_NODE)
		{
			temp_nodes.push_back(Node(sibling));
		}
		sibling = sibling->prev;
	}

	sibling = node_->prev;
	while (sibling != nullptr)
	{
		if (sibling->type == XML_ELEMENT_NODE)
		{
			nodes.emplace(sibling);
		}
		sibling = sibling->prev;
	}

	return nodes;
}

QueryResult Node::Children() const
{
	QueryResult::NodeSet nodes;

	if (node_ == nullptr || node_->type != XML_ELEMENT_NODE)
	{
		return nodes;
	}

	for (xmlNodePtr child = node_->children; child != nullptr; child = child->next)
	{
		if (child->type == XML_ELEMENT_NODE)
		{
			nodes.emplace(child);
		}
	}

	return nodes;
}

std::optional<Node> Node::FirstElementChild() const
{
	if (node_ == nullptr || node_->type != XML_ELEMENT_NODE)
	{
		return std::nullopt;
	}

	for (xmlNodePtr child = node_->children; child != nullptr; child = child->next)
	{
		if (child->type == XML_ELEMENT_NODE)
		{
			return Node(child);
		}
	}

	return std::nullopt;
}

std::optional<Node> Node::LastElementChild() const
{
	if (node_ == nullptr || node_->type != XML_ELEMENT_NODE)
	{
		return std::nullopt;
	}

	xmlNodePtr last_element_child = nullptr;

	for (xmlNodePtr child = node_->children; child != nullptr; child = child->next)
	{
		if (child->type == XML_ELEMENT_NODE)
		{
			last_element_child = child;
		}
	}

	if (last_element_child != nullptr)
	{
		return Node(last_element_child);
	}

	return std::nullopt;
}

bool Node::HasAttribute(const std::string &key) const
{
	if (node_ == nullptr || node_->type != XML_ELEMENT_NODE)
	{
		return false;
	}

	return xmlHasProp(node_, reinterpret_cast<const xmlChar *>(key.c_str())) != nullptr;
}

bool Node::HasClass(const std::string &className) const
{
	if (node_ == nullptr || node_->type != XML_ELEMENT_NODE)
	{
		return false;
	}

	std::optional<std::string> class_attr = Attribute("class");

	if (!class_attr)
	{
		return false;
	}

	std::stringstream ss(*class_attr);
	std::string token;
	while (std::getline(ss, token, ' '))
	{
		token.erase(0, token.find_first_not_of(' '));
		token.erase(token.find_last_not_of(' ') + 1);

		if (!token.empty() && token == className)
		{
			return true;
		}
	}

	return false;
}

std::vector<std::string> Node::Classes() const
{
	std::vector<std::string> class_list;

	if (node_ == nullptr || node_->type != XML_ELEMENT_NODE)
	{
		return class_list;
	}

	std::optional<std::string> class_attr = Attribute("class");

	if (!class_attr)
	{
		return class_list;
	}

	std::stringstream ss(*class_attr);
	std::string token;
	while (std::getline(ss, token, ' '))
	{
		token.erase(0, token.find_first_not_of(' '));
		token.erase(token.find_last_not_of(' ') + 1);
		if (!token.empty())
		{
			class_list.push_back(token);
		}
	}

	return class_list;
}

std::optional<Node> Node::Closest(std::string_view selector) const
{
	if (node_ == nullptr)
	{
		return std::nullopt;
	}

	xmlNodePtr current = node_;

	while (current != nullptr)
	{
		Node current_node_obj(current);

		QueryResult match_result = QueryResult::Find(current, selector);

		if (!match_result.empty() /*&& match_result.size() == 1 */)
		{
			return current_node_obj;
		}

		current = current->parent;
	}

	return std::nullopt;
}