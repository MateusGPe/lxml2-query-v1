#pragma once

#include "Selector.h"

class UnarySelector : public Selector
{
public:
    enum class Operator
    {
        Not,
        HasDescendant,
        HasChild,
    };

    UnarySelector() = delete;
    UnarySelector(Operator _operator, SelectorPtr selector) : selector_(selector), operator_(_operator) {}
    ~UnarySelector() = default;
    bool matches(xmlNodePtr node) const override;
    std::string toString() const override;

private:
    // bool hasDescendantMatch(xmlNodePtr node, SelectorPtr selector) const;
    // bool hasChildMatch(xmlNodePtr node, SelectorPtr selector) const;

private:
    SelectorPtr selector_;
    Operator operator_;
};