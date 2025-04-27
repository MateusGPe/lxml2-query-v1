#pragma once

#include <libxml/tree.h>
#include <libxml/HTMLparser.h>

#include <optional>
#include <string>
#include <functional>
#include <locale>

#include "Selector.h"
#include "Helpers.h"

class QueryResult;

class Node
{
public:
	using AttributeList = std::vector<std::pair<std::string, std::string>>;

	Node() = delete;
	Node(xmlNodePtr node) : node_(node) {}
	virtual ~Node() = default;

	std::optional<Node> Parent() const;
	std::optional<Node> NextSibling() const;
	std::optional<Node> PrevSibling() const;
	std::optional<Node> ChildAt(std::size_t i) const;
	unsigned long ChildNum() const;
	std::optional<std::string> Attribute(const std::string &) const;
	std::optional<std::string> Text() const;
	std::optional<std::string> OwnText() const;
	std::optional<std::string> TextNormalized() const;
	std::optional<std::string> OwnTextNormalized() const;
	std::optional<std::string> TagName() const;
	AttributeList Attributes() const;
	QueryResult NextSiblings() const;
	QueryResult Siblings() const;
	QueryResult Find(std::string_view) const;
	QueryResult Find(const SelectorPtr) const;

	operator xmlNodePtr() const
	{
		return node_;
	}

	xmlNodePtr operator()() const
	{
		return node_;
	}

	bool valid()
	{
		return node_ != nullptr;
	}

protected:
	xmlNodePtr node_;
};