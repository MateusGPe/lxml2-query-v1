#include <gtest/gtest.h>
#include "TestsUtils.hpp"

namespace lxml2query
{
    TEST(TextSelectorTest, Contains_Match)
    {
        xmlNodePtr node = createNode("<div>Test Content</div>");
        TextSelector selector(TextSelector::Operator::Contains, "test");
        EXPECT_TRUE(selector.matches(node));
        freeNode(node);
    }

    TEST(TextSelectorTest, Contains_NoMatch)
    {
        xmlNodePtr node = createNode("<div>Test Content</div>");
        TextSelector selector(TextSelector::Operator::Contains, "wrong");
        EXPECT_FALSE(selector.matches(node));
        freeNode(node);
    }

    TEST(TextSelectorTest, Contains_NormalizedText)
    {
        xmlNodePtr node = createNode("<div> Test \n Content </div>");
        TextSelector selector(TextSelector::Operator::Contains, "test content");
        EXPECT_TRUE(selector.matches(node));
        freeNode(node);
    }

    TEST(TextSelectorTest, OwnContains_Match)
    {
        xmlNodePtr node = createNode("<div>Test <span>Nested</span> Content</div>");
        TextSelector selector(TextSelector::Operator::OwnContains, "test content");
        EXPECT_TRUE(selector.matches(node));
        freeNode(node);
    }

    TEST(TextSelectorTest, OwnContains_NoMatchNested)
    {
        xmlNodePtr node = createNode("<div>Test <span>Nested</span> Content</div>");
        TextSelector selector(TextSelector::Operator::OwnContains, "nested");
        EXPECT_FALSE(selector.matches(node));
        freeNode(node);
    }

    TEST(TextSelectorTest, Matches_Match)
    {
        xmlNodePtr node = createNode("<div>Test 123 Content</div>");
        TextSelector selector(TextSelector::Operator::Matches, "\\d+");
        EXPECT_TRUE(selector.matches(node));
        freeNode(node);
    }

    TEST(TextSelectorTest, Matches_NoMatch)
    {
        xmlNodePtr node = createNode("<div>Test Content</div>");
        TextSelector selector(TextSelector::Operator::Matches, "\\d+");
        EXPECT_FALSE(selector.matches(node));
        freeNode(node);
    }

    TEST(TextSelectorTest, MatchesOwn_Match)
    {
        xmlNodePtr node = createNode("<div>Test 123 <span>Nested</span> Content</div>");
        TextSelector selector(TextSelector::Operator::MatchesOwn, "\\d+");
        EXPECT_TRUE(selector.matches(node));
        freeNode(node);
    }

    TEST(TextSelectorTest, MatchesOwn_NoMatchNested)
    {
        xmlNodePtr node = createNode("<div>Test <span>123</span> Content</div>");
        TextSelector selector(TextSelector::Operator::MatchesOwn, "\\d+");
        EXPECT_FALSE(selector.matches(node));
        freeNode(node);
    }

    TEST(TextSelectorTest, ToString_Contains)
    {
        TextSelector selector(TextSelector::Operator::Contains, "test");
        EXPECT_EQ(selector.toString(), ":contains(test)");
    }

    TEST(TextSelectorTest, ToString_OwnContains)
    {
        TextSelector selector(TextSelector::Operator::OwnContains, "test");
        EXPECT_EQ(selector.toString(), ":contains-own(test)");
    }

    TEST(TextSelectorTest, ToString_Matches)
    {
        TextSelector selector(TextSelector::Operator::Matches, "\\d+");
        EXPECT_EQ(selector.toString(), ":matches(\\d+)");
    }

    TEST(TextSelectorTest, ToString_MatchesOwn)
    {
        TextSelector selector(TextSelector::Operator::MatchesOwn, "\\d+");
        EXPECT_EQ(selector.toString(), ":matches-own(\\d+)");
    }

    TEST(TextSelectorTest, NullNode)
    {
        TextSelector selector(TextSelector::Operator::Contains, "test");
        EXPECT_FALSE(selector.matches(nullptr));
    }
}