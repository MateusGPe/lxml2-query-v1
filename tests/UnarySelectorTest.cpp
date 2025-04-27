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

    TEST(UnarySelectorTest, Not_Match)
    {
        auto selector = std::make_shared<MockSelector>(false);
        UnarySelector unarySelector(UnarySelector::Operator::Not, selector);
        xmlNodePtr node = createNode("<div>Test</div>");
        EXPECT_TRUE(unarySelector.matches(node));
        freeNode(node);
    }

    TEST(UnarySelectorTest, Not_NoMatch)
    {
        auto selector = std::make_shared<MockSelector>(true);
        UnarySelector unarySelector(UnarySelector::Operator::Not, selector);
        xmlNodePtr node = createNode("<div>Test</div>");
        EXPECT_FALSE(unarySelector.matches(node));
        freeNode(node);
    }

    TEST(UnarySelectorTest, HasDescendant_Match)
    {
        auto selector = std::make_shared<MockSelector>(true);
        xmlNodePtr node = createNode("<div><span>Test</span></div>");
        UnarySelector unarySelector(UnarySelector::Operator::HasDescendant, selector);
        EXPECT_TRUE(unarySelector.matches(node));
        freeNode(node);
    }

    TEST(UnarySelectorTest, HasDescendant_NoMatch)
    {
        auto selector = std::make_shared<MockSelector>(false);
        xmlNodePtr node = createNode("<div><span>Test</span></div>");
        UnarySelector unarySelector(UnarySelector::Operator::HasDescendant, selector);
        EXPECT_FALSE(unarySelector.matches(node));
        freeNode(node);
    }

    TEST(UnarySelectorTest, HasDescendant_NestedMatch)
    {
        auto selector = std::make_shared<MockSelector>(true);
        xmlNodePtr node = createNode("<div><span><p>Test</p></span></div>");
        UnarySelector unarySelector(UnarySelector::Operator::HasDescendant, selector);
        EXPECT_TRUE(unarySelector.matches(node));
        freeNode(node);
    }

    TEST(UnarySelectorTest, HasChild_Match)
    {
        auto selector = std::make_shared<MockSelector>(true);
        xmlNodePtr node = createNode("<div><span>Test</span></div>");
        UnarySelector unarySelector(UnarySelector::Operator::HasChild, selector);
        EXPECT_TRUE(unarySelector.matches(node));
        freeNode(node);
    }

    TEST(UnarySelectorTest, HasChild_NoMatch)
    {
        auto selector = std::make_shared<MockSelector>(false);
        xmlNodePtr node = createNode("<div><span>Test</span></div>");
        UnarySelector unarySelector(UnarySelector::Operator::HasChild, selector);
        EXPECT_FALSE(unarySelector.matches(node));
        freeNode(node);
    }

    TEST(UnarySelectorTest, HasChild_NestedNoMatch)
    {
        auto selector = std::make_shared<MockSelector>(true);
        xmlNodePtr node = createNode("<div>Test</div>");
        UnarySelector unarySelector(UnarySelector::Operator::HasChild, selector);
        EXPECT_FALSE(unarySelector.matches(node));
        freeNode(node);
    }

    TEST(UnarySelectorTest, ToString_Not)
    {
        auto selector = std::make_shared<MockSelector>(true);
        UnarySelector unarySelector(UnarySelector::Operator::Not, selector);
        EXPECT_EQ(unarySelector.toString(), ":not(MockTrue)");
    }

    TEST(UnarySelectorTest, ToString_HasDescendant)
    {
        auto selector = std::make_shared<MockSelector>(true);
        UnarySelector unarySelector(UnarySelector::Operator::HasDescendant, selector);
        EXPECT_EQ(unarySelector.toString(), ":has(MockTrue)");
    }

    TEST(UnarySelectorTest, ToString_HasChild)
    {
        auto selector = std::make_shared<MockSelector>(true);
        UnarySelector unarySelector(UnarySelector::Operator::HasChild, selector);
        EXPECT_EQ(unarySelector.toString(), ":has-child(MockTrue)");
    }

    TEST(UnarySelectorTest, HasDescendant_NonElementNode)
    {
        auto selector = std::make_shared<MockSelector>(true);
        xmlNodePtr node = createNode("<div>Text</div>");
        UnarySelector unarySelector(UnarySelector::Operator::HasDescendant, selector);

        xmlNodePtr textNode = node->children;
        EXPECT_FALSE(unarySelector.matches(textNode));
        freeNode(node);
    }

    TEST(UnarySelectorTest, HasChild_NonElementNode)
    {
        auto selector = std::make_shared<MockSelector>(true);
        xmlNodePtr node = createNode("<div>Text</div>");
        UnarySelector unarySelector(UnarySelector::Operator::HasChild, selector);

        xmlNodePtr textNode = node->children;
        EXPECT_FALSE(unarySelector.matches(textNode));
        freeNode(node);
    }
}