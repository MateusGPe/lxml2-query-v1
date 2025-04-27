#pragma once

#include "Selector.h"
#include "Helpers.h"

class AttributeSelector : public Selector
{
public:
    enum class Operator
    {
        Exists,
        Equals,
        Includes,
        DashMatch,
        Prefix,
        Suffix,
        SubString,
        Regex
    };

    AttributeSelector() = delete;
    AttributeSelector(Operator _operator, std::string_view key, std::string_view value, bool sensitive) : key_(toLower(key)), value_(sensitive ? toLower(value) : value), operator_(_operator), icase_(sensitive) {}
    bool matches(xmlNodePtr node) const override;
    ~AttributeSelector() = default;

    std::string toString() const override;

private:
    std::string key_;
    std::string value_;
    Operator operator_;
    bool icase_;
};
