#pragma once

#include <libxml/HTMLparser.h>
#include <libxml/tree.h>
#include <set>

#include <string>
#include <regex>
#include <memory>

class Selector
{
public:
    using NodeSet = std::set<xmlNodePtr>;

    Selector() = default;
    virtual bool matches(xmlNodePtr) const { return true; }
    virtual std::string toString() const { return "*"; };
    virtual NodeSet Filter(NodeSet nodes) const;
    virtual NodeSet MatchAll(xmlNodePtr node) const;
    virtual ~Selector() = default;

protected:
    void matchAllInto(xmlNodePtr node, NodeSet &nodes) const;
};

using SelectorPtr = std::shared_ptr<Selector>;
