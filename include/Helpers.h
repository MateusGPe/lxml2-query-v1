#pragma once

#include <string>
#include <functional>
#include <locale>
#include <optional>

#include <string.h>
#include <libxml/tree.h>
#include <libxml/HTMLparser.h>
#include <sstream>

static inline std::string toLower(std::string_view s)
{
    std::string result(s);
    std::transform(result.begin(), result.end(), result.begin(), [](char c) -> char
                   { return static_cast<char>(std::tolower(c)); });
    return result;
}

static inline std::string toUpper(std::string_view s)
{
    std::string result(s);
    std::transform(result.begin(), result.end(), result.begin(), [](char c) -> char
                   { return static_cast<char>(std::toupper(c)); });
    return result;
}

static inline std::string normalizeHtmlText(std::string_view text)
{
    std::string normalized;
    bool inWhitespace = false;

    auto end = text.end();

    std::size_t lastNonSpace = text.find_last_not_of(" \t\n\r\f\v");

    if (lastNonSpace != std::string::npos)
    {
        end = text.begin() + lastNonSpace + 1;
    }
    else
    {
        return "";
    }

    auto it = text.begin();

    while (it != end && std::isspace(*it))
    {
        ++it;
    }

    auto size = end - it;

    if (size < 1)
    {
        return "";
    }

    normalized.reserve(static_cast<size_t>(size));

    for (; it != end; ++it)
    {
        char c(*it);

        if (std::isspace(c))
        {
            if (!inWhitespace)
            {
                normalized += ' '; // Add a single space
                inWhitespace = true;
            }
        }
        else
        {
            normalized += c;
            inWhitespace = false;
        }
    }

    return normalized;
}
/*
static inline xmlChar *trim_case(xmlChar *s, bool modCase, bool toUpper)
{
    std::size_t pos = 0, offset = 0;

    std::function<int(int)> fn = [](int c)
    { return c; };

    if (modCase)
    {
        fn = toUpper ? (int (*)(int))std::toupper : (int (*)(int))std::tolower;
    }

    while (std::isspace(s[pos]))
    {
        pos++;
    }

    while (s[pos] != '\0')
    {
        if (std::isspace(s[pos]))
        {
            s[offset] = ' ';
            offset++;

            while (std::isspace(s[pos]))
            {
                pos++;
            }
            continue;
        }
        s[offset] = fn(s[pos]);
        offset++;
        pos++;
    }
    if (offset > 0)
    {
        if (s[offset - 1] == ' ')
        {
            offset--;
        }
    }
    s[offset] = '\0';
    return s;
}
*/

static inline bool NodeIsEmpty(xmlNodePtr node)
{
    if (node == nullptr)
    {
        return true;
    }
    if (node->type != XML_ELEMENT_NODE)
    {
        return false;
    }
    for (xmlNodePtr child = node->children; child != nullptr; child = child->next)
    {
        if (child->type == XML_ELEMENT_NODE || (!xmlIsBlankNode(child) && child->type != XML_COMMENT_NODE))
        {
            return false;
        }
    }
    return true;
}

static inline std::string getLowerElementName(xmlNodePtr node)
{
    std::string nodeName(reinterpret_cast<const char *>(node->name));
    std::transform(nodeName.begin(), nodeName.end(), nodeName.begin(), ::tolower);
    return nodeName;
}

static inline bool IsEqualElement(xmlNodePtr node1, xmlNodePtr node2)
{
    if (!node1 || !node2)
    {
        return false;
    }
    if (node1->name == nullptr || node2->name == nullptr)
    {
        return false;
    }
    return getLowerElementName(node1) == getLowerElementName(node2);
}

static inline bool IsEqualElement(xmlNodePtr node1, std::string_view name)
{
    if (!node1)
    {
        return false;
    }
    if (node1->name == nullptr)
    {
        return false;
    }
    return toLower(normalizeHtmlText(reinterpret_cast<const char *>(node1->name))) == name;
}

static inline bool NodeIsOnlyChild(xmlNodePtr node, bool ofType)
{
    if (node->type != XML_ELEMENT_NODE)
    {
        return false;
    }

    xmlNodePtr parent = node->parent;
    if (parent == nullptr)
    {
        return true;
    }

    unsigned int count = 0;

    for (xmlNodePtr child = parent->children; child != nullptr; child = child->next)
    {
        if ((child->type != XML_ELEMENT_NODE) ||
            (ofType && !IsEqualElement(node, child)))
        {
            continue;
        }

        count++;

        if (count > 1)
        {
            return false;
        }
    }

    return true; // count == 1;
}

static inline std::pair<int, int> NodeSiblingsCount(xmlNodePtr node, bool ofType, bool last)
{
    if (node->type != XML_ELEMENT_NODE)
    {
        return std::make_pair(0, 0);
    }

    xmlNodePtr parent = node->parent;

    if (parent == nullptr)
    {
        return std::make_pair(0, 0);
    }

    int npos = -1;
    int count = 0;

    for (xmlNodePtr child = parent->children; child != nullptr; child = child->next)
    {
        if (child->type != XML_ELEMENT_NODE)
        {
            continue;
        }
        if (ofType)
        {
            if (!IsEqualElement(node, child))
            {
                continue;
            }
        }

        count++;

        if (node == child)
        {
            npos = count;
            if (!last)
            {
                break;
            }
        }
    }
    return std::make_pair(count, npos);
}

static inline std::string GetPropNodeValue(xmlDocPtr doc, xmlNodePtr node)
{
    std::string value;

    xmlChar *txt = xmlNodeListGetString(doc, node, 1);
    if (txt != nullptr)
    {
        value.assign((char *)txt);
        xmlFree(txt);
    }
    return value;
}
static inline std::optional<std::string> GetAttributeValue(xmlNodePtr node, std::string_view key)
{
    if (node == nullptr)
    {
        return std::nullopt;
    }

    xmlChar *txt = xmlGetProp(node, reinterpret_cast<const xmlChar *>(key.data()));
    if (txt != nullptr)
    {
        std::string value;
        value.assign((char *)txt);
        xmlFree(txt);
        return value;
    }
    return std::nullopt;
}

static inline std::string NodeText(xmlNodePtr node)
{
    std::string text;
    if (node->type != XML_ELEMENT_NODE)
    {
        return text;
    }
    xmlChar *content = xmlNodeGetContent(node);

    if (content != nullptr)
    {
        text.assign((char *)content);
        xmlFree(content);
    }

    return text;
}

static inline std::string NodeOwnText(xmlNodePtr node)
{
    std::string text;

    for (xmlNodePtr child = node->children; child != nullptr; child = child->next)
    {
        if (child->type == XML_TEXT_NODE)
        {
            xmlChar *content = xmlNodeGetContent(child);
            if (content != nullptr)
            {
                text += (const char *)content;
                xmlFree(content);
            }
        }
    }

    return text;
}

/*template <typename Stream>
static inline void stream_node(Stream &stream, xmlNode *a_node, const char c_indent = ' ', std::size_t n_indent = 1)
{
    if (a_node != nullptr)
    {
        xmlNode *stop = a_node->parent;
        xmlNode *cur_node = a_node;
        std::string indent("");
        int skip = 0;

        for (;;)
        {
            if (!skip)
            {
                if (cur_node->type == XML_ELEMENT_NODE)
                {
                    stream << indent << "<" << cur_node->name << "";
                    xmlAttr *attr = cur_node->properties;

                    while (attr && attr->name && attr->children)
                    {
                        std::string value(trim(GetPropNodeValue(cur_node->doc, attr->children)));
                        stream << " " << trim((char *)attr->name) << "=\"" << value << "\"";
                        attr = attr->next;
                    }
                    stream << ">" << std::endl;
                }
                else if (cur_node->type == XML_TEXT_NODE)
                {
                    xmlChar *content = xmlNodeGetContent(cur_node);

                    if (content != nullptr)
                    {
                        std::string txt((char *)trim(content));

                        if (!txt.empty())
                        {
                            stream << indent << txt << std::endl;
                        }
                        xmlFree(content);
                    }
                }

                if (cur_node->children)
                {
                    indent.append(n_indent, c_indent);
                    cur_node = cur_node->children;
                    continue;
                }
            }
            if (cur_node->next)
            {
                cur_node = cur_node->next;
                skip = 0;
            }
            else
            {
                if (!indent.empty())
                {
                    indent.erase(indent.length() - n_indent, n_indent);
                }

                cur_node = cur_node->parent;
                if (cur_node == stop)
                    break;

                if (cur_node->type == XML_ELEMENT_NODE)
                {
                    stream << indent << "</" << cur_node->name << ">" << std::endl;
                }
                skip = 1;
            }
        }
    }
}*/

static inline bool Includes(std::string_view text, std::string_view value)
{
    std::size_t start = 0;
    std::size_t end = 0;

    while (end < text.length())
    {
        while (start < text.length() && std::isspace(text[start]))
        {
            start++;
        }

        end = start;

        while (end < text.length() && !std::isspace(text[end]))
        {
            end++;
        }

        if (end > start)
        {
            if (text.substr(start, end - start) == value)
            {
                return true;
            }
        }
        start = end;
    }
    return false;
}
