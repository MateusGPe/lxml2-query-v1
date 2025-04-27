#include <gtest/gtest.h>
#include "TestsUtils.hpp"

namespace lxml2query
{
    TEST(HtmlUtilsTest, normalizeHtmlText_WhitespaceNormalization)
    {
        EXPECT_EQ(normalizeHtmlText("  Hello  World  "), "Hello World");
        EXPECT_EQ(normalizeHtmlText("\t\n\r  Test \t\n\r"), "Test");
        EXPECT_EQ(normalizeHtmlText("Leading spaces"), "Leading spaces");
        EXPECT_EQ(normalizeHtmlText("Trailing spaces  "), "Trailing spaces");
        EXPECT_EQ(normalizeHtmlText("  "), "");
        EXPECT_EQ(normalizeHtmlText(""), "");
        EXPECT_EQ(normalizeHtmlText("No spaces"), "No spaces");
        EXPECT_EQ(normalizeHtmlText("  \t\n\r\f\v  test  \t\n\r\f\v  "), "test");
    }

    TEST(HtmlUtilsTest, NodeIsEmpty_EmptyNode)
    {
        xmlDocPtr doc = htmlReadMemory("<div></div>", strlen("<div></div>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);
        ASSERT_TRUE(NodeIsEmpty(root->children->children));
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, NodeIsEmpty_NonEmptyNode)
    {
        xmlDocPtr doc = htmlReadMemory("<div><p>Test</p></div>", strlen("<div><p>Test</p></div>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);
        ASSERT_FALSE(NodeIsEmpty(root));
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, IsEqualElement_EqualElements)
    {
        xmlDocPtr doc = htmlReadMemory("<div><p>Test</p><p>Test</p></div>", strlen("<div><p>Test</p><p>Test</p></div>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);

        xmlNodePtr node1 = root->children->children->children;
        xmlNodePtr node2 = node1->next;
        ASSERT_TRUE(IsEqualElement(node1, node2));
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, IsEqualElement_DifferentElements)
    {
        xmlDocPtr doc = htmlReadMemory("<div><p>Test</p><span>Test</span></div>", strlen("<div><p>Test</p><span>Test</span></div>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);

        xmlNodePtr node1 = root->children->children->children;
        xmlNodePtr node2 = node1->next->next;
        ASSERT_FALSE(IsEqualElement(node1, node2));
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, NodeIsOnlyChild_OnlyChild)
    {
        xmlDocPtr doc = htmlReadMemory("<div><p>Test</p></div>", strlen("<div><p>Test</p></div>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);
        ASSERT_TRUE(NodeIsOnlyChild(root->children->children->children, false));
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, NodeIsOnlyChild_NotOnlyChild)
    {
        xmlDocPtr doc = htmlReadMemory("<div><p>Test</p><p>Test2</p></div>", strlen("<div><p>Test</p><p>Test2</p></div>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);
        ASSERT_FALSE(NodeIsOnlyChild(root->children->children->children, false));
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, NodeSiblingsCount_BasicCount)
    {
        xmlDocPtr doc = htmlReadMemory("<div><p>Test</p><p>Test2</p><p>Test3</p></div>", strlen("<div><p>Test</p><p>Test2</p><p>Test3</p></div>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);

        std::pair<int, int> result = NodeSiblingsCount(root->children->children->children, false, true);
        EXPECT_EQ(result.first, 3);
        EXPECT_EQ(result.second, 1);
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, NodeGetAttributeValue_BasicTest)
    {
        xmlDocPtr doc = htmlReadMemory("<a href=\"test\"></a>", strlen("<a href=\"test\"></a>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);

        std::string value = GetPropNodeValue(doc, root->children->children->properties->children);
        EXPECT_EQ(value, "test");
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, toLower_BasicTest)
    {
        EXPECT_EQ(toLower("TEST"), "test");
        EXPECT_EQ(toLower("Test"), "test");
        EXPECT_EQ(toLower("test"), "test");
        EXPECT_EQ(toLower(""), "");
    }

    TEST(HtmlUtilsTest, toUpper_BasicTest)
    {
        EXPECT_EQ(toUpper("test"), "TEST");
        EXPECT_EQ(toUpper("Test"), "TEST");
        EXPECT_EQ(toUpper("TEST"), "TEST");
        EXPECT_EQ(toUpper(""), "");
    }

    TEST(HtmlUtilsTest, NodeText_BasicTest)
    {
        xmlDocPtr doc = htmlReadMemory("<div>Test Content</div>", strlen("<div>Test Content</div>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);
        EXPECT_EQ(NodeText(root->children->children), "Test Content");
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, NodeOwnText_BasicTest)
    {
        xmlDocPtr doc = htmlReadMemory("<div>Own Text<p>Child Text</p></div>", strlen("<div>Own Text<p>Child Text</p></div>\0"), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
        ASSERT_NE(doc, nullptr);

        xmlNodePtr root = xmlDocGetRootElement(doc);
        ASSERT_NE(root, nullptr);
        EXPECT_EQ(NodeOwnText(root->children->children), "Own Text");
        xmlFreeDoc(doc);
    }

    TEST(HtmlUtilsTest, Includes_BasicTest)
    {
        EXPECT_TRUE(Includes("this is a test", "is"));
        EXPECT_FALSE(Includes("this is a test", "not"));
        EXPECT_TRUE(Includes("test test test", "test"));
        EXPECT_FALSE(Includes("", "test"));
        EXPECT_FALSE(Includes("test", ""));
        EXPECT_TRUE(Includes("one two three", "two"));
    }
}