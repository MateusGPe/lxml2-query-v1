#include <gtest/gtest.h>
#include "TestsUtils.hpp"

namespace lxml2query
{
    TEST(NodeTest, Parent)
    {
        xmlNodePtr node = createNode("<html><body><p>Test</p></body></html>");
        xmlNodePtr pNode = node->children->children; // <p>
        Node p(pNode);
        auto parent = p.Parent();
        ASSERT_TRUE(parent.has_value());
        EXPECT_EQ(parent.value()(), node->children); // <body>
        freeNode(node);
    }

    TEST(NodeTest, NextSibling)
    {
        xmlNodePtr node = createNode("<html><body><p>Test</p><div>Test2</div></body></html>");
        xmlNodePtr pNode = node->children->children; // <p>
        Node p(pNode);
        auto nextSibling = p.NextSibling();
        ASSERT_TRUE(nextSibling.has_value());
        EXPECT_EQ(nextSibling.value()(), node->children->children->next); // <div>
        freeNode(node);
    }

    TEST(NodeTest, PrevSibling)
    {
        xmlNodePtr node = createNode("<html><body><p>Test</p><div>Test2</div></body></html>");
        xmlNodePtr divNode = node->children->children->next; // <div>
        Node div(divNode);
        auto prevSibling = div.PrevSibling();
        ASSERT_TRUE(prevSibling.has_value());
        EXPECT_EQ(prevSibling.value()(), node->children->children); // <p>
        freeNode(node);
    }

    TEST(NodeTest, ChildAt)
    {
        xmlNodePtr node = createNode("<div><p>Test</p><span>Test2</span></div>");
        Node div(node);

        auto child = div.ChildAt(1);

        ASSERT_TRUE(child.has_value());
        EXPECT_EQ(child.value()(), node->children->next); // <span>
        freeNode(node);
    }

    TEST(NodeTest, ChildNum)
    {
        xmlNodePtr node = createNode("<div><p>Test</p><span>Test2</span></div>");

        Node span(node);

        EXPECT_EQ(span.ChildNum(), 2);

        freeNode(node);
    }

    TEST(NodeTest, Attribute)
    {
        xmlNodePtr node = createNode("<div id='test' class='class1 class2'>Test</div>");
        Node div(node);
        auto id = div.Attribute("id");
        ASSERT_TRUE(id.has_value());
        EXPECT_EQ(id.value(), "test");
        auto classAttr = div.Attribute("class");
        ASSERT_TRUE(classAttr.has_value());
        EXPECT_EQ(classAttr.value(), "class1 class2");
        auto nonExistent = div.Attribute("nonexistent");
        EXPECT_FALSE(nonExistent.has_value());
        freeNode(node);
    }

    TEST(NodeTest, Text)
    {
        xmlNodePtr node = createNode("<div>Test <p>Nested</p> text</div>");
        Node div(node);
        auto text = div.Text();
        ASSERT_TRUE(text.has_value());
        EXPECT_EQ(text.value(), "Test Nested text");
        freeNode(node);
    }

    TEST(NodeTest, OwnText)
    {
        xmlNodePtr node = createNode("<div>Test <p>Nested</p> text</div>");
        Node div(node);

        //std::cerr << div.TagName().value() << std::endl;

        auto ownText = div.OwnText();
        ASSERT_TRUE(ownText.has_value());
        EXPECT_EQ(ownText.value(), "Test  text");
        freeNode(node);
    }

    TEST(NodeTest, TextNormalized)
    {
        xmlNodePtr node = createNode("<div> Test  \n\t  Nested \t text\n </div>");
        Node div(node);
        auto textNormalized = div.TextNormalized();
        ASSERT_TRUE(textNormalized.has_value());
        EXPECT_EQ(textNormalized.value(), "Test Nested text");
        freeNode(node);
    }

    TEST(NodeTest, OwnTextNormalized)
    {
        xmlNodePtr node = createNode("<div> Test  \n\t  <p>Nested</p> \t text\n </div>");
        Node div(node);
        auto ownTextNormalized = div.OwnTextNormalized();
        ASSERT_TRUE(ownTextNormalized.has_value());
        EXPECT_EQ(ownTextNormalized.value(), "Test text");
        freeNode(node);
    }

    TEST(NodeTest, TagName)
    {
        xmlNodePtr node = createNode("<div>Test</div>");
        Node div(node);
        auto tagName = div.TagName();
        ASSERT_TRUE(tagName.has_value());
        EXPECT_EQ(tagName.value(), "div");
        freeNode(node);
    }

    TEST(NodeTest, Siblings)
    {
        xmlNodePtr node = createNode("<div><p>Test1</p><div>Test2</div><span>Test3</span></div>");
        Node p(node->children);
        QueryResult siblings = p.Siblings();

        EXPECT_EQ(siblings.size(), 2);
        EXPECT_EQ(siblings.at(0).value()(), node->children->next);
        EXPECT_EQ(siblings.at(1).value()(), node->children->next->next);
        
        freeNode(node);
    }

    TEST(NodeTest, FindStringSelector)
    {
        xmlNodePtr node = createNode("<html><body><p class='test'>Test1</p><div class='test'>Test2</div></body></html>");
        Node body(node->children);
        QueryResult found = body.Find(".test");
        EXPECT_EQ(found.size(), 2);
        EXPECT_EQ(found.at(0).value()(), node->children->children);
        EXPECT_EQ(found.at(1).value()(), node->children->children->next);
        freeNode(node);
    }

    TEST(NodeTest, FindSelectorPtr)
    {
        xmlNodePtr node = createNode("<html><body><p id='test'>Test1</p></body></html>");
        Node body(node->children);
        auto selector = std::make_shared<AttributeSelector>(AttributeSelector::Operator::Equals, "id", "test", true);
        QueryResult found = body.Find(selector);
        EXPECT_EQ(found.size(), 1);
        EXPECT_EQ(found.at(0).value()(), node->children->children);
        freeNode(node);
    }

    TEST(NodeTest, ValidNode)
    {
        xmlNodePtr node = createNode("<div>Test</div>");
        Node validNode(node);
        EXPECT_TRUE(validNode.valid());
        freeNode(node);

        Node invalidNode(nullptr);
        EXPECT_FALSE(invalidNode.valid());
    }
}