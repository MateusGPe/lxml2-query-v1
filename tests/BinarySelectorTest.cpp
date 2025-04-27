#include <gtest/gtest.h>
#include "TestsUtils.hpp"

namespace lxml2query
{
    class MockSelector : public Selector
    {
    public:
        MockSelector(bool match) : match_(match) {}
        bool matches(xmlNodePtr node) const override { return match_; }
        std::string toString() const override { return match_ ? "MockTrue" : "MockFalse"; }

    private:
        bool match_;
    };

    class MockName : public Selector
    {
    public:
        MockName(const std::string &match) : match_(match) {}
        bool matches(xmlNodePtr node) const override
        {
            bool result = std::string((char *)node->name) == match_;
            GTEST_LOG_(INFO) << "node->name: " << node->name << " == " << match_ << " = " << std::boolalpha << result;
            return result;
        }
        std::string toString() const override { return match_; }

    private:
        std::string match_;
    };

    class MockAlphaSelector : public Selector
    {
    public:
        MockAlphaSelector(bool match) : match_(match) {}
        bool matches(xmlNodePtr node) const override { return match_; }
        std::string toString() const override { return match_ ? "alpha" : "beta"; }

    private:
        bool match_;
    };

    TEST(BinarySelectorTest, Union_TrueTrue)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(BinarySelector::Operator::Union, lselector, rselector);
        xmlNodePtr node = createNode("<test/>");
        EXPECT_TRUE(selector.matches(node));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Union_TrueFalse)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);

        xmlNodePtr node = createNode("<test/>");

        BinarySelector selector1(BinarySelector::Operator::Union, lselector, rselector);
        EXPECT_TRUE(selector1.matches(node));

        BinarySelector selector2(BinarySelector::Operator::Union, rselector, lselector);
        EXPECT_TRUE(selector2.matches(node));

        freeNode(node);
    }

    TEST(BinarySelectorTest, Union_FalseFalse)
    {
        auto lselector = std::make_shared<MockSelector>(false);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(BinarySelector::Operator::Union, lselector, rselector);
        xmlNodePtr node = createNode("<test/>");
        EXPECT_FALSE(selector.matches(node));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Intersection_TrueTrue)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(BinarySelector::Operator::Intersection, lselector, rselector);
        xmlNodePtr node = createNode("<test/>");
        EXPECT_TRUE(selector.matches(node));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Intersection_TrueFalse)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        
        xmlNodePtr node = createNode("<test/>");

        BinarySelector selector1(BinarySelector::Operator::Intersection, lselector, rselector);
        EXPECT_FALSE(selector1.matches(node));

        BinarySelector selector2(BinarySelector::Operator::Intersection, rselector, lselector);
        EXPECT_FALSE(selector2.matches(node));
        
        freeNode(node);
    }

    TEST(BinarySelectorTest, Intersection_FalseFalse)
    {
        auto lselector = std::make_shared<MockSelector>(false);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(BinarySelector::Operator::Intersection, lselector, rselector);
        xmlNodePtr node = createNode("<test/>");
        EXPECT_FALSE(selector.matches(node));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Child_Match)
    {
        xmlNodePtr node = createNode("<parent><child/></parent>");
        xmlNodePtr child = node->children;
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(BinarySelector::Operator::Child, lselector, rselector);
        EXPECT_TRUE(selector.matches(child));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Child_NoMatchParent)
    {
        xmlNodePtr node = createNode("<parent><child/></parent>");
        xmlNodePtr child = node->children;
        auto lselector = std::make_shared<MockSelector>(false);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(BinarySelector::Operator::Child, lselector, rselector);
        EXPECT_FALSE(selector.matches(child));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Child_NoMatchChild)
    {
        xmlNodePtr node = createNode("<parent><child/></parent>");
        xmlNodePtr child = node->children;

        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);

        BinarySelector selector(BinarySelector::Operator::Child, lselector, rselector);
        EXPECT_FALSE(selector.matches(child));

        freeNode(node);
    }

    TEST(BinarySelectorTest, Descendant_Match)
    {
        xmlNodePtr node = createNode("<parent><grandparent><child/></grandparent></parent>");
        xmlNodePtr parent = node;
        xmlNodePtr child = node->children->children;

        auto lselector = std::make_shared<MockName>("parent");
        auto rselector = std::make_shared<MockName>("child");

        BinarySelector selector1(BinarySelector::Operator::Descendant, lselector, rselector);

        EXPECT_TRUE(selector1.matches(child));
        EXPECT_EQ(selector1.toString(), "parent child");

        BinarySelector selector2(BinarySelector::Operator::Descendant, rselector, lselector);

        EXPECT_FALSE(selector2.matches(child));
        EXPECT_EQ(selector2.toString(), "child parent");

        freeNode(node);
    }

    TEST(BinarySelectorTest, Descendant_NoMatchAncestor)
    {
        xmlNodePtr node = createNode("<parent><grandparent><child/></grandparent></parent>");
        xmlNodePtr child = node->children->children->children;

        auto lselector = std::make_shared<MockSelector>(false);
        auto rselector = std::make_shared<MockSelector>(true);

        BinarySelector selector(BinarySelector::Operator::Descendant, lselector, rselector);
        EXPECT_FALSE(selector.matches(child));

        freeNode(node);
    }

    TEST(BinarySelectorTest, Descendant_NoMatchChild)
    {
        xmlNodePtr node = createNode("<parent><grandparent><child/></grandparent></parent>");
        xmlNodePtr child = node->children->children->children;
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(BinarySelector::Operator::Descendant, lselector, rselector);
        EXPECT_FALSE(selector.matches(child));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Adjacent_Match)
    {
        xmlNodePtr node = createNode("<parent><sibling1/><sibling2/></parent>");
        xmlNodePtr sibling2 = node->children->next;
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(lselector, rselector, true);
        EXPECT_TRUE(selector.matches(sibling2));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Adjacent_NoMatchSibling)
    {
        xmlNodePtr node = createNode("<parent><sibling1/><sibling2/></parent>");
        xmlNodePtr sibling2 = node->children->next;
        auto lselector = std::make_shared<MockSelector>(false);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(lselector, rselector, true);
        EXPECT_FALSE(selector.matches(sibling2));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Adjacent_NoMatchChild)
    {
        xmlNodePtr node = createNode("<parent><sibling1/><sibling2/></parent>");
        xmlNodePtr sibling2 = node->children->next;
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(lselector, rselector, true);
        EXPECT_FALSE(selector.matches(sibling2));
        freeNode(node);
    }

    TEST(BinarySelectorTest, GeneralSibling_Match)
    {
        xmlNodePtr node = createNode("<parent><sibling1/><sibling2/><sibling3/></parent>");
        xmlNodePtr sibling3 = node->children->next->next;
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(lselector, rselector, false);
        EXPECT_TRUE(selector.matches(sibling3));
        freeNode(node);
    }

    TEST(BinarySelectorTest, GeneralSibling_NoMatchSibling)
    {
        xmlNodePtr node = createNode("<parent><sibling1/><sibling2/><sibling3/></parent>");
        xmlNodePtr sibling3 = node->children->next->next;
        auto lselector = std::make_shared<MockSelector>(false);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(lselector, rselector, false);
        EXPECT_FALSE(selector.matches(sibling3));
        freeNode(node);
    }

    TEST(BinarySelectorTest, GeneralSibling_NoMatchChild)
    {
        xmlNodePtr node = createNode("<parent><sibling1/><sibling2/><sibling3/></parent>");
        xmlNodePtr sibling3 = node->children->next->next;
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(lselector, rselector, false);
        EXPECT_FALSE(selector.matches(sibling3));
        freeNode(node);
    }

    TEST(BinarySelectorTest, ToString_Union)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(BinarySelector::Operator::Union, lselector, rselector);
        EXPECT_EQ(selector.toString(), "MockTrue, MockFalse");
    }

    TEST(BinarySelectorTest, ToString_Intersection)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(BinarySelector::Operator::Intersection, lselector, rselector);
        EXPECT_EQ(selector.toString(), "MockTrue MockFalse");
    }

    TEST(BinarySelectorTest, ToString_Child)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(BinarySelector::Operator::Child, lselector, rselector);
        EXPECT_EQ(selector.toString(), "MockTrue > MockFalse");
    }

    TEST(BinarySelectorTest, ToString_Descendant)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(BinarySelector::Operator::Descendant, lselector, rselector);
        EXPECT_EQ(selector.toString(), "MockTrue MockFalse");
    }

    TEST(BinarySelectorTest, ToString_Adjacent)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(lselector, rselector, true);
        EXPECT_EQ(selector.toString(), "MockTrue + MockFalse");

        BinarySelector selector2(lselector, rselector, false);
        EXPECT_EQ(selector2.toString(), "MockTrue ~ MockFalse");
    }

    TEST(BinarySelectorTest, ToString_Descendant_Alphanumeric)
    {
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockAlphaSelector>(true);
        BinarySelector selector(BinarySelector::Operator::Descendant, lselector, rselector);
        EXPECT_EQ(selector.toString(), "MockTrue alpha");
    }

    TEST(BinarySelectorTest, Adjacent_NonElementSibling)
    {
        xmlNodePtr node = createNode("<parent><sibling1/><sibling2/></parent>");
        xmlNodePtr sibling2 = node->children->next;
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(lselector, rselector, false);
        EXPECT_TRUE(selector.matches(sibling2));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Adjacent_NoPreviousSibling)
    {
        xmlNodePtr node = createNode("<parent><sibling1/></parent>");
        xmlNodePtr sibling1 = node->children;
        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(lselector, rselector, true);
        EXPECT_FALSE(selector.matches(sibling1));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Descendant_NestedDescendant)
    {
        xmlNodePtr node = createNode("<parent><grandparent><child><grandchild/></child></grandparent></parent>");
        xmlNodePtr grandchild = node->children->children->children;

        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(true);

        BinarySelector selector(BinarySelector::Operator::Descendant, lselector, rselector);

        EXPECT_TRUE(selector.matches(grandchild));

        freeNode(node);
    }

    TEST(BinarySelectorTest, Descendant_NoMatchInMiddle)
    {
        xmlNodePtr node = createNode("<parent><grandparent><child><grandchild/></child></grandparent></parent>");
        xmlNodePtr grandchild = node->children->children->children->children;

        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(false);
        BinarySelector selector(BinarySelector::Operator::Descendant, lselector, rselector);

        EXPECT_FALSE(selector.matches(grandchild));

        freeNode(node);
    }

    TEST(BinarySelectorTest, Child_NoParent)
    {
        xmlNodePtr node = createNode("<test/>");

        auto lselector = std::make_shared<MockSelector>(true);
        auto rselector = std::make_shared<MockSelector>(true);
        BinarySelector selector(BinarySelector::Operator::Child, lselector, rselector);

        EXPECT_FALSE(selector.matches(node));
        freeNode(node);
    }

    TEST(BinarySelectorTest, Adjacent_MultipleSiblings)
    {
        xmlNodePtr node = createNode("<parent><sibling1/><sibling2/><sibling3/></parent>");
        xmlNodePtr sibling3 = node->children->next->next;

        auto lselector = std::make_shared<MockName>("sibling1");
        auto rselector = std::make_shared<MockName>("sibling3");
        BinarySelector selector(lselector, rselector, true);

        EXPECT_FALSE(selector.matches(sibling3)); // Should be false, as it's not directly adjacent

        BinarySelector selector2(lselector, rselector, false);

        EXPECT_TRUE(selector2.matches(sibling3)); // Should be true, as it's a general sibling

        freeNode(node);
    }

    TEST(BinarySelectorTest, Adjacent_CommentNodeSibling)
    {
        xmlNodePtr node = createNode("<parent><sibling1/><sibling2/><sibling3/></parent>");
        xmlNodePtr sibling2 = node->children->next->next;

        auto lselector = std::make_shared<MockName>("sibling1");
        auto rselector = std::make_shared<MockName>("sibling3");
        BinarySelector selector(lselector, rselector, true);
        EXPECT_FALSE(selector.matches(sibling2)); // Should be false, as it's not directly adjacent

        BinarySelector selector2(lselector, rselector, false);
        EXPECT_TRUE(selector2.matches(sibling2));

        freeNode(node);
    }
}