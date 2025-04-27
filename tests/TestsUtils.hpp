#pragma once

#include "../include/lxml2-query.h"
#include <regex>
#include <random>
// #include <sstream>

/* std::string generateRandomSelector()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99);

    std::vector<std::string> types = {"div", "span", "p", "a", "button", "input", "ul", "li", "article", "section"};
    std::vector<std::string> ids = {"#main", "#header", "#content", "#sidebar", "#footer"};
    std::vector<std::string> classes = {".text", ".button", ".link", ".item", ".container", ".highlight", ".error"};
    std::vector<std::string> attributes = {"[data-value=\"123\"]", "[href=\"#\"]", "[type=\"text\"]", "[disabled]", "[data-index]", "[lang=\"en\"]"};
    std::vector<std::string> pseudoClasses = {":contains(a10)", ":first-child()", ":last-child", ":nth-child(3n+1)", ":not(.hidden)", ":empty"};
    std::vector<std::string> combinators = {"  ", " > ", " + ", " ~ ", ", "};

    std::stringstream selector;

    // Type selector (optional)
    if (dist(gen) % 3 != 0)
    { // 2/3 probability
        selector << types[dist(gen) % types.size()];
    }

    // ID selector (optional)
    if (dist(gen) % 4 == 0)
    {
        selector << ids[dist(gen) % ids.size()];
    }

    // Class selectors (optional, multiple)
    int numClasses = dist(gen) % 5; // 0, 1, or 2 classes
    for (int i = 0; i < numClasses; ++i)
    {
        if (dist(gen) % 2 == 0)
        {
            selector << classes[dist(gen) % classes.size()];
        }
    }

    // Attribute selectors (optional, multiple)
    int numAttributes = dist(gen) % 10; // 0 or 1 attribute
    for (int i = 0; i < numAttributes; ++i)
    {
        if (dist(gen) % 2 == 0)
        {
            selector << attributes[dist(gen) % attributes.size()];
        }
    }

    // Pseudo-classes (optional, multiple)
    int numPseudoClasses = dist(gen) % 2; // 0 or 1 pseudo-class
    for (int i = 0; i < numPseudoClasses; ++i)
    {
        if (dist(gen) % 2 == 0)
        {
            selector << pseudoClasses[dist(gen) % pseudoClasses.size()];
        }
    }

    // Combinator and another selector (optional)
    if (dist(gen) % 3 == 0 && !selector.str().empty())
    {
        selector << combinators[dist(gen) % combinators.size()] << generateRandomSelector(); // Recursive call
    }

    if (selector.str().empty())
    {
        return generateRandomSelector();
    }

    return selector.str();
} */

static inline std::string generateRandomSelector()
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dist(0, 99);

    static const std::vector<std::string> types = {"div", "span", "p", "a", "button", "input", "ul", "li", "article", "section", "header", "footer", "nav", "aside", "main", "form", "table", "td", "th"};
    static const std::vector<std::string> ids = {"#main", "#header", "#content", "#sidebar", "#footer", "#nav", "#search", "#submit", "#form", "#item"};
    static const std::vector<std::string> classes = {".text", ".button", ".link", ".item", ".container", ".highlight", ".error", ".active", ".hidden", ".selected", ".menu", ".title", ".image", ".data", ".field"};
    static const std::vector<std::string> attributes = {
        "[data-value=\"123\"]", "[href=\"#\"]", "[type=\"text\"]", "[disabled]", "[data-index]", "[lang=\"en\"s]",
        "[data-type=\"image\"]", "[aria-hidden=\"true\"]", "[data-status=\"success\"]", "[required]", "[data-id=\"abc\"s]",
        "[data-name=\"user\"]", "[data-count=\"10\"]", "[data-target=\"#target\"]", "[data-action=\"submit\"]", "[data-url=\"https://example.com\" s]"};
    static const std::vector<std::string> pseudoClasses = {
        ":contains(a10)", ":first-child()", ":last-child", ":nth-child(3n+1)", ":not(.hidden)", ":empty",
        ":nth-of-type(2n-10)", ":only-child", ":only-of-type", ":first-of-type", ":last-of-type", ":nth-last-child(6n+1)"};
    static const std::vector<std::string> combinators = {"  ", " > ", " + ", " ~ ", ", "};
    static const std::vector<std::string> attributeOperators = {"=", "~=", "|=", "^=", "$=", "*="};

    std::stringstream selector;

    if (dist(gen) % 3 != 0)
    {
        selector << types[dist(gen) % types.size()];
    }

    std::vector<std::string> selectors;

    if (dist(gen) % 4 == 0)
    {
        selectors.push_back(ids[dist(gen) % ids.size()]);
    }

    int numClasses = dist(gen) % 4;
    std::vector<std::string> shuffledClasses = classes;
    std::sample(classes.begin(), classes.end(), std::back_inserter(shuffledClasses), numClasses, gen);
    for (int i = 0; i < numClasses; ++i)
    {
        if (dist(gen) % 2 == 0)
        {
            selectors.push_back(shuffledClasses[i]);
        }
    }

    int numAttributes = dist(gen) % 3;
    std::vector<std::string> shuffledAttributes = attributes;
    std::sample(attributes.begin(), attributes.end(), std::back_inserter(shuffledAttributes), numAttributes, gen);
    for (int i = 0; i < numAttributes; ++i)
    {
        if (dist(gen) % 2 == 0)
        {
            std::string attr = shuffledAttributes[i];
            size_t equalPos = attr.find('=');
            if (equalPos != std::string::npos)
            {
                std::string operatorStr = attributeOperators[dist(gen) % attributeOperators.size()];
                if (operatorStr != "=")
                {
                    attr.replace(equalPos, 1, operatorStr);
                }
            }
            selectors.push_back(attr);
        }
    }

    int numPseudoClasses = dist(gen) % 3;
    std::vector<std::string> shuffledPseudoClasses = pseudoClasses;
    std::sample(pseudoClasses.begin(), pseudoClasses.end(), std::back_inserter(shuffledPseudoClasses), numPseudoClasses, gen);
    for (int i = 0; i < numPseudoClasses; ++i)
    {
        if (dist(gen) % 2 == 0)
        {
            selectors.push_back(shuffledPseudoClasses[i]);
        }
    }

    std::shuffle(selectors.begin(), selectors.end(), gen);
    for (const auto &sel : selectors)
    {
        selector << sel;
    }

    if (dist(gen) % 3 == 0 && !selector.str().empty())
    {
        selector << combinators[dist(gen) % combinators.size()] << generateRandomSelector();
    }

    if (selector.str().empty())
    {
        return generateRandomSelector();
    }

    return selector.str();
}
// #include <algorithm>

static inline std::string replaceString(std::string &str, const std::string &from, const std::string &to)
{
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos)
    {
        str.replace(pos, from.size(), to);
        pos += to.size();
    }
    return str;
}

static inline std::string rewriteString(const std::string &input)
{
    static const std::regex pattern(R"(\[class~=([^\]]+)\])");
    std::smatch match;

    if (std::regex_search(input, match, pattern))
    {
        std::string result(input);
        replaceString(result, "[class~=" + match[1].str() + "]", "." + normalizeHtmlText(match[1].str()));
        return result;
    }
    return input;
}

static inline std::string stripSelector(const std::string &input)
{
    std::string result = rewriteString(toLower(normalizeHtmlText(input)));
    size_t pos = 0;

    while ((pos = result.find("()", pos)) != std::string::npos)
    {
        result.erase(pos, 2);
    }

    replaceString(result, "\\0000e9", "é");
    replaceString(result, "\\e9", "é");

    replaceString(result, "(0n+", "(");
    replaceString(result, "(odd)", "(2n+1)");
    replaceString(result, "(even)", "(2n)");

    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\\'), result.end());
    result.erase(std::remove(result.begin(), result.end(), '\''), result.end());
    result.erase(std::remove(result.begin(), result.end(), '"'), result.end());

    return result;
}

static inline std::string fmtSelector(const std::string &input)
{
    std::string result = normalizeHtmlText(input);

    size_t pos = 0;
    while ((pos = result.find("()", pos)) != std::string::npos)
    {
        result.erase(pos, 2);
    }
    result.erase(std::remove(result.begin(), result.end(), ' '), result.end());
    std::replace(result.begin(), result.end(), '\'', '\"');

    return result;
}

static inline xmlNodePtr createNode(const char *xml)
{
    xmlDocPtr doc = xmlReadMemory(xml, strlen(xml), "test.xml", nullptr, 0);
    if (!doc)
        return nullptr;
    xmlNodePtr root = xmlDocGetRootElement(doc);
    if (!root)
    {
        xmlFreeDoc(doc);
        return nullptr;
    }
    return root;
}

static inline void freeNode(xmlNodePtr node)
{
    if (node && node->doc)
    {
        xmlFreeDoc(node->doc);
    }
}

static inline std::string GetNodeId(xmlNodePtr current)
{
    xmlAttr *attr = current->properties;

    while (attr && attr->name && attr->children)
    {
        if (std::string((char *)attr->name) == "id")
        {
            return GetPropNodeValue(current->doc, attr->children);
        }
        attr = attr->next;
    }
    return "";
}

static inline xmlNodePtr GetElementById(xmlNodePtr node, std::string_view id)
{
    if (node == nullptr)
    {
        return nullptr;
    }

    std::vector<xmlNodePtr> stack;
    stack.reserve(128);
    stack.push_back(node);

    while (!stack.empty())
    {
        xmlNodePtr current = stack.back();
        stack.pop_back();

        if (current->type == XML_ELEMENT_NODE)
        {
            if (GetNodeId(current) == id)
            {
                return current;
            }
        }

        if (current->type == XML_ELEMENT_NODE)
        {
            for (xmlNodePtr child = current->children; child != nullptr; child = child->next)
            {
                if (child->type == XML_ELEMENT_NODE)
                {
                    stack.push_back(child);
                }
            }
        }
    }
    return nullptr;
}

template <typename T>
static inline bool CompareVector(const std::vector<T> &vec1, const std::vector<T> &vec2)
{
    std::set<T> set1(vec1.begin(), vec1.end());
    std::set<T> set2(vec2.begin(), vec2.end());

    return set1 == set2;
}

static inline void printXmlNode(xmlNodePtr node, int indent = 0)
{
    if (node == nullptr)
    {
        return;
    }

    for (int i = 0; i < indent; ++i)
    {
        std::cout << "  ";
    }

    if (node->type == XML_ELEMENT_NODE)
    {
        std::cout << "<" << node->name << ">";

        // Print attributes
        xmlAttrPtr attr = node->properties;
        while (attr != nullptr)
        {
            std::cout << " " << attr->name << "=\"";
            xmlChar *value = xmlNodeGetContent(attr->children);
            if (value)
            {
                std::cout << (char *)value;
                xmlFree(value);
            }
            std::cout << "\"";
            attr = attr->next;
        }

        std::cout << std::endl;

        // Print children recursively
        xmlNodePtr child = node->children;
        while (child != nullptr)
        {
            printXmlNode(child, indent + 1);
            child = child->next;
        }

        for (int i = 0; i < indent; ++i)
        {
            std::cout << "  ";
        }
        std::cout << "</" << node->name << ">" << std::endl;
    }
    else if (node->type == XML_TEXT_NODE)
    {
        xmlChar *content = xmlNodeGetContent(node);
        if (content != nullptr && !xmlIsBlankNode(node))
        { // Avoid printing whitespace-only text nodes
            std::cout << (char *)content << std::endl;
        }
        xmlFree(content);
    }
    else if (node->type == XML_COMMENT_NODE)
    {
        xmlChar *content = xmlNodeGetContent(node);
        if (content)
        {
            std::cout << "" << std::endl;
            xmlFree(content);
        }
    }
    else if (node->type == XML_PI_NODE)
    { // Processing instruction
        std::cout << "<?" << node->name << " " << xmlNodeGetContent(node) << "?>" << std::endl;
    }
    else if (node->type == XML_CDATA_SECTION_NODE)
    {
        xmlChar *content = xmlNodeGetContent(node);
        if (content)
        {
            std::cout << "<![CDATA[" << (char *)content << "]]>" << std::endl;
            xmlFree(content);
        }
    }
}