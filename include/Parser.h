#pragma once

#include <string>
//#include <stdexcept>
//#include <array>
//#include <iostream>
//#include <libxml/tree.h>
//#include <libxml/HTMLparser.h>
#include "Selector.h"
#include "Lexer.h"

class SelectorParser
{
private:
	SelectorParser(std::string_view input) : lx_(input) {}

public:
	virtual ~SelectorParser() = default;

public:
	static SelectorPtr Create(std::string_view);

protected:
	SelectorPtr ParseSelectorGroup();
	SelectorPtr ParseSelector();
	SelectorPtr ParseSimpleSelectorSequence();

	void ParseNth(int &a, int &b);

	SelectorPtr ParsePseudoclassSelector();
	SelectorPtr ParseAttributeSelector();
	SelectorPtr ParseClassSelector();
	SelectorPtr ParseIdSelector();
	SelectorPtr ParseTypeSelector();

protected:
	SelectorLexer lx_;
};