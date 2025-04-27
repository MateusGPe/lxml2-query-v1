#pragma once

#include "Selector.h"

class TextSelector : public Selector
{
public:
    enum class Operator
    {
        OwnContains,
        Contains,
        Matches,
        MatchesOwn,
    };

    TextSelector() = delete;
    TextSelector(Operator _operator, std::string_view value) : value_(value), operator_(_operator) {}
    ~TextSelector() = default;

    bool matches(xmlNodePtr node) const override;

    std::string toString() const override;

private:
    std::string value_;
    Operator operator_;
};