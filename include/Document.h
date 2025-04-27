#pragma once

#include <libxml/tree.h>
#include <libxml/HTMLparser.h>

#include <string>

#include "QueryResult.h"

class Document
{
public:
	Document() : htmldoc_(nullptr, xmlFreeDoc), root_(nullptr)
	{
		LIBXML_TEST_VERSION
	}

	bool parse(const std::string &input);
	bool parseMemory(const std::string &input);

	~Document() = default;

	QueryResult Find(std::string_view) const;
	QueryResult Find(const SelectorPtr &) const;

	xmlNodePtr getRoot() { return root_; }

	bool isValid() const { return htmldoc_ && root_ != nullptr; }

protected:
	std::unique_ptr<xmlDoc, decltype(&xmlFreeDoc)> htmldoc_;
	xmlNodePtr root_;
};