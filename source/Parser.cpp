#include "Parser.h"

#include "AttributeSelector.h"
#include "BinarySelector.h"
#include "TagSelector.h"
#include "TextSelector.h"
#include "UnarySelector.h"

#define error(message) \
    std::runtime_error((std::string((message)) + std::string(" -  at: ") + std::to_string(lx_.GetOffset())))

namespace
{
    enum class SelectorType
    {
        Not,
        Has,
        HasChild,
        Contains,
        ContainsOwn,
        Matches,
        MatchesOwn,
        NthChild,
        NthLastChild,
        NthOfType,
        NthLastOfType,
        FirstChild,
        LastChild,
        FirstOfType,
        LastOfType,
        OnlyChild,
        OnlyOfType,
        Empty,
        Root,
        Link,
        Lang,
        Enabled,
        Disabled,
        Checked
    };

    static const std::unordered_map<std::string_view, SelectorType> selectorNames = {
        {"not", SelectorType::Not},
        {"has", SelectorType::Has},
        {"has-child", SelectorType::HasChild},
        {"contains", SelectorType::Contains},
        {"contains-own", SelectorType::ContainsOwn},
        {"matches", SelectorType::Matches},
        {"matches-own", SelectorType::MatchesOwn},
        {"nth-child", SelectorType::NthChild},
        {"nth-last-child", SelectorType::NthLastChild},
        {"nth-of-type", SelectorType::NthOfType},
        {"nth-last-of-type", SelectorType::NthLastOfType},
        {"first-child", SelectorType::FirstChild},
        {"last-child", SelectorType::LastChild},
        {"first-of-type", SelectorType::FirstOfType},
        {"last-of-type", SelectorType::LastOfType},
        {"only-child", SelectorType::OnlyChild},
        {"only-of-type", SelectorType::OnlyOfType},
        {"empty", SelectorType::Empty},
        {"root", SelectorType::Root},
        {"link", SelectorType::Link},
        {"any-link", SelectorType::Link},
        {"lang", SelectorType::Lang},
        {"enabled", SelectorType::Enabled},
        {"disabled", SelectorType::Disabled},
        {"checked", SelectorType::Checked}};
} // namespace

SelectorPtr SelectorParser::Create(std::string_view selectorString)
{
    return SelectorParser(selectorString).ParseSelectorGroup();
}

SelectorPtr SelectorParser::ParseSelectorGroup()
{
    SelectorPtr selector = ParseSelector();
    while (lx_.HasRemaining())
    {
        if (!lx_.TryMatch(','))
        {
            return selector;
        }
        SelectorPtr rselector = ParseSelector();
        selector =
            std::make_shared<BinarySelector>(BinarySelector::Operator::Union, selector, rselector);
    }

    return selector;
}

SelectorPtr SelectorParser::ParseSelector()
{
    lx_.SkipSpaceCmts();
    SelectorPtr selector = ParseSimpleSelectorSequence();
    while (true)
    {
        char combinator = 0;
        if (lx_.SkipSpaceCmts())
        {
            combinator = ' ';
        }

        if (!lx_.HasRemaining())
        {
            return selector;
        }

        if (lx_.IsAnyOf('+', '>', '~'))
        {
            combinator = lx_.CurrentChar();
            lx_.ConsumeChar();
            lx_.SkipSpaceCmts();
        }
        else if (lx_.IsAnyOf(')', ','))
        {
            return selector;
        }

        if (combinator == 0)
        {
            return selector;
        }

        SelectorPtr prev_selector = selector;
        SelectorPtr rselector = ParseSimpleSelectorSequence();

        bool success = true;
        switch (combinator)
        {
        case ' ':
        {
            selector = std::make_shared<BinarySelector>(BinarySelector::Operator::Descendant, prev_selector, rselector);
        }
        break;
        case '>':
        {
            selector = std::make_shared<BinarySelector>(BinarySelector::Operator::Child, prev_selector, rselector);
        }
        break;
        case '+':
        {
            selector = std::make_shared<BinarySelector>(prev_selector, rselector, true);
        }
        break;
        case '~':
        {
            selector = std::make_shared<BinarySelector>(prev_selector, rselector, false);
        }
        break;
        default:
        {
            success = false;
        }
        break;
        }
        if (!success)
        {
            throw error("impossible");
        }
    }
}

SelectorPtr SelectorParser::ParseSimpleSelectorSequence()
{
    SelectorPtr selector = nullptr;
    if (lx_.EndOfFile())
    {
        throw error("ParseSimpleSelectorSequence: expected selector, found EOF instead");
    }

    if (lx_.IsAnyOf('*'))
    {
        lx_.ConsumeChar();
    }
    else if (lx_.IsNotAnyOf('#', '.', '[', ':'))
    {
        selector = ParseTypeSelector();
    }

    while (lx_.HasRemaining())
    {
        SelectorPtr rselector = nullptr;

        bool breakloop(false);

        switch (lx_.CurrentChar())
        {
        case '#':
        {
            rselector = ParseIdSelector();
        }
        break;
        case '.':
        {
            rselector = ParseClassSelector();
        }
        break;
        case '[':
        {
            rselector = ParseAttributeSelector();
        }
        break;
        case ':':
        {
            rselector = ParsePseudoclassSelector();
        }
        break;
        default:
        {
            breakloop = true;
        }
        break;
        }

        if (breakloop == true)
        {
            break;
        }

        selector = (selector == nullptr) ? rselector : std::make_shared<BinarySelector>(BinarySelector::Operator::Intersection, selector, rselector);
    }
    return (selector == nullptr) ? std::make_shared<Selector>() : selector;
}

void SelectorParser::ParseNth(int &a, int &b)
{
    bool has_int(false);
    int current(0);

    a = 0;
    b = 0;

    while (true)
    {
        lx_.SkipSpaceCmts();
        if (lx_.EndOfFile())
        {
            throw error("unexpected EOF while attempting to parse expression of form an+b");
        }

        char c = lx_.CurrentChar();

        if (std::isdigit(c) || c == '+' || c == '-')
        {
            has_int = true;
            if (current != 0)
            {
                b += current;
            }
            current = lx_.ReadInteger();
        }
        else if (c == 'n' || c == 'N')
        {
            lx_.ConsumeChar();
            if (current == 0)
            {
                a = 1;
            }
            else
            {
                a += current;
                current = 0;
            }
        }
        else
        {
            break;
        }
    }
    if (!has_int)
    {
        throw error("expected integer, but didn't find it.");
    }
    if (current != 0)
    {
        b += current;
    }
}

SelectorPtr SelectorParser::ParsePseudoclassSelector()
{
    if (!lx_.TryMatch(':'))
    {
        throw error("expected pseudoclass selector (:pseudoclass), found invalid char");
    }

    std::string name = toLower(lx_.ReadIdentifier());

    auto it = selectorNames.find(name);

    if (it == selectorNames.end())
    {
        throw error("unsupported pseudoclass: " + name);
    }

    SelectorType type = it->second;

#define CONSUME_PARENS_AND_CHECK(T)                                               \
    if (lx_.TryConsumeSpaced<true>('('))                                          \
    {                                                                             \
        if (!lx_.TryConsumeSpaced<false>(')'))                                    \
        {                                                                         \
            throw error("PseudoClass (" #T "): expected ')' but didn't find it"); \
        }                                                                         \
    }

    switch (type)
    {
    case SelectorType::Not:
    case SelectorType::Has:
    case SelectorType::HasChild:
    {
        if (!lx_.TryConsumeSpaced<true>('('))
        {
            throw error("PseudoClass (Not,Has,HasChild): expected '(' but didn't find it");
        }

        SelectorPtr sel = ParseSelectorGroup();
        if (!lx_.TryConsumeSpaced<false>(')'))
        {
            throw error("PseudoClass (Not,Has,HasChild): expected ')' but didn't find it");
        }

        UnarySelector::Operator _operator;
        switch (type)
        {
        case SelectorType::Not:
            _operator = UnarySelector::Operator::Not;
            break;
        case SelectorType::Has:
            _operator = UnarySelector::Operator::HasDescendant;
            break;
        case SelectorType::HasChild:
            _operator = UnarySelector::Operator::HasChild;
            break;
        default:
            throw error("Internal error, _operator:" + name);
            break;
        }

        return SelectorPtr(std::make_shared<UnarySelector>(_operator, sel));
    }
    case SelectorType::Contains:
    case SelectorType::ContainsOwn:
    {
        if (!lx_.TryConsumeSpaced<true>('('))
        {
            throw error("PseudoClass (Contains,ContainsOwn): expected '(' but didn't find it");
        }

        std::string value;
        char c = lx_.CurrentChar();
        if (c == '\'' || c == '"')
        {
            value = lx_.ReadString();
        }
        else
        {
            value = lx_.ReadIdentifier();
        }
        value = toLower(value);
        lx_.SkipSpaceCmts();

        if (!lx_.TryConsumeSpaced<false>(')'))
        {
            throw error("PseudoClass (Contains,ContainsOwn): expected ')' but didn't find it");
        }

        TextSelector::Operator _operator;
        if (type == SelectorType::Contains)
        {
            _operator = TextSelector::Operator::Contains;
        }
        else
        {
            _operator = TextSelector::Operator::OwnContains;
        }
        return std::make_shared<TextSelector>(_operator, value);
    }
    case SelectorType::Matches:
    case SelectorType::MatchesOwn:
    {
        if (!lx_.TryConsumeSpaced<true>('('))
        {
            throw error("PseudoClass (Matches,MatchesOwn): expected '(' but didn't find it");
        }

        lx_.SkipSpaceCmts();
        std::string value(lx_.ReadString());
        lx_.SkipSpaceCmts();

        if (!lx_.TryConsumeSpaced<false>(')'))
        {
            throw error("PseudoClass (Matches,MatchesOwn): expected ')' but didn't find it");
        }

        TextSelector::Operator _operator;

        if (type == SelectorType::Matches)
        {
            _operator = TextSelector::Operator::Matches;
        }
        else
        {
            _operator = TextSelector::Operator::MatchesOwn;
        }
        return std::make_shared<TextSelector>(_operator, value);
    }
    case SelectorType::NthChild:
    case SelectorType::NthLastChild:
    case SelectorType::NthOfType:
    case SelectorType::NthLastOfType:
    {
        if (!lx_.TryConsumeSpaced<true>('('))
        {
            throw error("PseudoClass (Nth's): expected '(' but didn't find it");
        }

        int a, b;

        if (lx_.PeekSubstring(3) == "odd")
        {
            lx_.Advance(3);
            a = 2;
            b = 1;
        }
        else if (lx_.PeekSubstring(4) == "even")
        {
            lx_.Advance(4);
            a = 2;
            b = 0;
        }
        else
        {
            ParseNth(a, b);
        }

        if (!lx_.TryConsumeSpaced<false>(')'))
        {
            throw error("PseudoClass (Nth's): expected ')' but didn't find it");
        }

        bool Last(type == SelectorType::NthLastChild || type == SelectorType::NthLastOfType),
            OfType(type == SelectorType::NthOfType || type == SelectorType::NthLastOfType);

        return std::make_shared<TagSelector>(a, b, Last, OfType);
    }
    case SelectorType::FirstChild:
        CONSUME_PARENS_AND_CHECK(FirstChild);
        return std::make_shared<TagSelector>(0, 1, false, false);
    case SelectorType::LastChild:
        CONSUME_PARENS_AND_CHECK(LastChild);
        return std::make_shared<TagSelector>(0, 1, true, false);
    case SelectorType::FirstOfType:
        CONSUME_PARENS_AND_CHECK(FirstOfType);
        return std::make_shared<TagSelector>(0, 1, false, true);
    case SelectorType::LastOfType:
        CONSUME_PARENS_AND_CHECK(LastOfType);
        return std::make_shared<TagSelector>(0, 1, true, true);
    case SelectorType::OnlyChild:
        CONSUME_PARENS_AND_CHECK(OnlyChild);
        return std::make_shared<TagSelector>(false);
    case SelectorType::OnlyOfType:
        CONSUME_PARENS_AND_CHECK(OnlyOfType);
        return std::make_shared<TagSelector>(true);
    case SelectorType::Empty:
        CONSUME_PARENS_AND_CHECK(Empty);
        return std::make_shared<TagSelector>(TagSelector::Operator::Empty);
    case SelectorType::Root:
        CONSUME_PARENS_AND_CHECK(Root);
        return std::make_shared<TagSelector>(TagSelector::Operator::Root);
    case SelectorType::Link:
        CONSUME_PARENS_AND_CHECK(Link);
        return std::make_shared<TagSelector>(TagSelector::Operator::Link);
    case SelectorType::Enabled:
        CONSUME_PARENS_AND_CHECK(Enabled);
        return std::make_shared<TagSelector>(TagSelector::Operator::Enabled);
    case SelectorType::Disabled:
        CONSUME_PARENS_AND_CHECK(Disabled);
        return std::make_shared<TagSelector>(TagSelector::Operator::Disabled);
    case SelectorType::Checked:
        CONSUME_PARENS_AND_CHECK(Checked);
        return std::make_shared<TagSelector>(TagSelector::Operator::Checked);
    case SelectorType::Lang:
    {
        std::string lang;
        if (!lx_.TryConsumeSpaced<true>('('))
        {
            throw error("PseudoClass (Lang): expected '(' but didn't find it");
        }
        if (lx_.IsAnyOf('\'', '"'))
        {
            lang = lx_.ReadString();
        }
        else
        {
            lang = lx_.ReadIdentifier();
        }
        if (lang.empty())
        {
            throw error("PseudoClass (Lang): expected language but didn't find it");
        }
        if (!lx_.TryConsumeSpaced<false>(')'))
        {
            throw error("PseudoClass (Lang): expected ')' but didn't find it");
        }
        return std::make_shared<TagSelector>(TagSelector::Operator::Lang, lang);
    }
    default:
        throw error("Internal error, _operator:" + name);
    }
#undef CONSUME_PARENS_AND_CHECK
}

SelectorPtr SelectorParser::ParseAttributeSelector()
{
    if (!lx_.TryMatch('['))
    {
        throw error("expected attribute selector ([attribute]), found invalid char");
    }

    lx_.SkipSpaceCmts();

    std::string key = lx_.ReadIdentifier();

    lx_.SkipSpaceCmts();

    if (lx_.EndOfFile())
    {
        throw error("unexpected EOF in attribute selector");
    }

    if (lx_.CurrentChar() == ']')
    {
        lx_.ConsumeChar();
        return std::make_shared<AttributeSelector>(AttributeSelector::Operator::Exists, key, "", false);
    }

    std::string _operator(lx_.PeekSubstring(2));

    lx_.Advance(2);

    if (_operator.empty())
    {
        throw error("unexpected EOF in attribute selector");
    }

    if (_operator[0] == '=')
    {
        _operator = "=";
        lx_.Advance(-1);
    }
    else if (_operator[1] != '=')
    {
        std::stringstream ss;
        lx_.Advance(-3);

        ss << "expected equality operator, found invalid char '" << lx_.PeekSubstring(10) << " - " << uint16_t(_operator[0]) << "' key: '" << key << "' ";

        throw error(ss.str());
        // throw error("expected equality operator, found invalid char");
    }

    lx_.SkipSpaceCmts();

    if (lx_.EndOfFile())
    {
        throw error("unexpected EOF in attribute selector");
    }

    std::string value;
    if (_operator == "#=")
    {
        if (lx_.CurrentChar() != '/')
        {
            throw error("expected regex, found invalid char");
        }
        lx_.ConsumeChar();
        value = lx_.ReadRegex();
    }
    else
    {
        char c = lx_.CurrentChar();
        if (c == '\'' || c == '"')
        {
            value = lx_.ReadString();
        }
        else
        {
            value = lx_.ReadIdentifier();
        }
    }

    bool icase = !lx_.TryConsumeSpaced<false>('s');

    lx_.TryConsumeSpaced<false>('i');
    lx_.SkipSpaceCmts();

    if (lx_.EndOfFile() || lx_.CurrentChar() != ']')
    {
        throw error("expected attribute selector ([attribute]), found invalid char");
    }

    lx_.ConsumeChar();

#define OPERATOR_CASE(c, op)                       \
    case c:                                        \
        attr_op = AttributeSelector::Operator::op; \
        break;

    AttributeSelector::Operator attr_op;
    switch (_operator[0])
    {
        OPERATOR_CASE('=', Equals)
        OPERATOR_CASE('~', Includes)
        OPERATOR_CASE('|', DashMatch)
        OPERATOR_CASE('^', Prefix)
        OPERATOR_CASE('$', Suffix)
        OPERATOR_CASE('*', SubString)
        OPERATOR_CASE('#', Regex)
    default:
        throw error("unsupported _operator:" + _operator);
    }
#undef OPERATOR_CASE
    return std::make_shared<AttributeSelector>(attr_op, key, value, icase);
}

SelectorPtr SelectorParser::ParseClassSelector()
{
    if (!lx_.TryMatch('.'))
    {
        throw error("expected class selector (.class), found invalid char");
    }
    return std::make_shared<AttributeSelector>(AttributeSelector::Operator::Includes, "class", lx_.ReadIdentifier(), false);
}

SelectorPtr SelectorParser::ParseIdSelector()
{
    if (!lx_.TryMatch('#'))
    {
        throw error("expected id selector (#id), found invalid char");
    }
    return std::make_shared<AttributeSelector>(AttributeSelector::Operator::Equals, "id", lx_.ReadName(), false);
}

SelectorPtr SelectorParser::ParseTypeSelector()
{
    return std::make_shared<TagSelector>(lx_.ReadIdentifier());
}
#undef error