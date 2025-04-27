#pragma once

#include "Selector.h"

class BinarySelector : public Selector
{
public:
    enum class Operator
    {
        Union,
        Intersection,
        Child,
        Descendant,
        Adjacent
    };

public:
    BinarySelector() = delete;
    BinarySelector(Operator _operator, SelectorPtr lselector, SelectorPtr rselector) : lselector_(lselector), rselector_(rselector), operator_(_operator), adjacent_(false) {}
    BinarySelector(SelectorPtr lselector, SelectorPtr rselector, bool adjacent) : lselector_(lselector), rselector_(rselector), operator_(Operator::Adjacent), adjacent_(adjacent) {}
    ~BinarySelector() = default;
    bool matches(xmlNodePtr node) const override;

    std::string toString() const override;

private:
    SelectorPtr lselector_;
    SelectorPtr rselector_;
    Operator operator_;
    bool adjacent_;
};