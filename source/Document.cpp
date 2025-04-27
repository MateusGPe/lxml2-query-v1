#include "Document.h"

#include <stdexcept>

bool Document::parse(const std::string &input)
{
	htmldoc_.reset(htmlReadFile(input.c_str(), "UTF-8",
								HTML_PARSE_NOBLANKS | HTML_PARSE_COMPACT | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET));

	if (htmldoc_)
	{
		root_ = xmlDocGetRootElement(htmldoc_.get());
		if (root_)
		{
			return true;
		}
		htmldoc_.reset();
	}
	return false;
}

bool Document::parseMemory(const std::string &input)
{
	htmldoc_.reset(htmlReadMemory(input.c_str(), static_cast<int>(input.length()), nullptr, "UTF-8",
								  HTML_PARSE_NOBLANKS | HTML_PARSE_COMPACT | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING | HTML_PARSE_NONET));

	if (htmldoc_)
	{
		root_ = xmlDocGetRootElement(htmldoc_.get());
		if (root_)
		{
			return true;
		}
		htmldoc_.reset();
	}
	return false;
}

QueryResult Document::Find(std::string_view strsel) const
{
	if (!htmldoc_)
	{
		throw std::runtime_error("Document not initialized");
	}
	return QueryResult::Find(root_, strsel);
}

QueryResult Document::Find(const SelectorPtr &selector) const
{
	if (!htmldoc_)
	{
		throw std::runtime_error("Document not initialized");
	}
	return QueryResult::Find(root_, selector);
}