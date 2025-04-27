#pragma once

#include "Selector.h"

class TagSelector : public Selector
{
public:
    enum class Operator
    {
        Empty,
        Root,
        Link,
        Lang,
        OnlyChild,
        NthChild,
        Tag,
        Enabled,
        Disabled,
        Checked,
    };

    TagSelector() = delete;
    TagSelector(Operator _operator) : operator_(_operator), oftype_(false), a_(0), b_(0), last_(false), refvalue_("") {}
    TagSelector(Operator _operator, std::string_view refval) : operator_(_operator), oftype_(false), a_(0), b_(0), last_(false), refvalue_(refval) {}
    TagSelector(bool oftype) : operator_(Operator::OnlyChild), oftype_(oftype), a_(0), b_(0), last_(false), refvalue_("") {}
    TagSelector(int a, int b, bool last, bool oftype) : operator_(Operator::NthChild), oftype_(oftype), a_(a), b_(b), last_(last), refvalue_("") {}
    TagSelector(std::string_view tagname) : operator_(Operator::Tag), oftype_(false), a_(0), b_(0), last_(false), refvalue_(tagname) {}
    ~TagSelector() = default;
    bool matches(xmlNodePtr node) const override;

    std::string toString() const override;

private:
    Operator operator_;

    bool oftype_;
    int a_;
    int b_;
    bool last_;

    std::string refvalue_;
};
