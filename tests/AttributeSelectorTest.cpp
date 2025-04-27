#include <gtest/gtest.h>
#include "TestsUtils.hpp"

namespace lxml2query
{
    TEST(AttributeSelectorTest, Exists)
    {
        xmlNodePtr node = createNode("<test attr1='value1' attr2='value2'/>");
        AttributeSelector selector(AttributeSelector::Operator::Exists, "attr1", "", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Exists, "attr3", "", true);
        EXPECT_FALSE(selector2.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, Equals)
    {
        xmlNodePtr node = createNode("<test attr='value' attr2='VALUE'/>");
        AttributeSelector selector(AttributeSelector::Operator::Equals, "attr", "value", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Equals, "attr", "wrong", true);
        EXPECT_FALSE(selector2.matches(node));

        AttributeSelector selector3(AttributeSelector::Operator::Equals, "attr", "VALUE", false);
        EXPECT_FALSE(selector3.matches(node));

        AttributeSelector selector4(AttributeSelector::Operator::Equals, "attr2", "value", true);
        EXPECT_TRUE(selector4.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, Includes)
    {
        xmlNodePtr node = createNode("<test attr='value1 value2 VALUE4'/>");
        AttributeSelector selector(AttributeSelector::Operator::Includes, "attr", "value1", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Includes, "attr", "value3", true);
        EXPECT_FALSE(selector2.matches(node));

        AttributeSelector selector3(AttributeSelector::Operator::Includes, "attr", "VALUE1", false);
        EXPECT_FALSE(selector3.matches(node));

        AttributeSelector selector4(AttributeSelector::Operator::Includes, "attr", "value4", true);
        EXPECT_TRUE(selector4.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, DashMatch)
    {
        xmlNodePtr node = createNode("<test attr='value-test'/>");
        AttributeSelector selector(AttributeSelector::Operator::DashMatch, "attr", "value", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::DashMatch, "attr", "wrong", true);
        EXPECT_FALSE(selector2.matches(node));

        xmlNodePtr node2 = createNode("<test attr='value'/>");
        EXPECT_TRUE(selector.matches(node2));

        freeNode(node);
        freeNode(node2);
    }

    TEST(AttributeSelectorTest, Prefix)
    {
        xmlNodePtr node = createNode("<test attr='value-test'/>");
        AttributeSelector selector(AttributeSelector::Operator::Prefix, "attr", "value", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Prefix, "attr", "wrong", true);
        EXPECT_FALSE(selector2.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, Suffix)
    {
        xmlNodePtr node = createNode("<test attr='test-value'/>");
        AttributeSelector selector(AttributeSelector::Operator::Suffix, "attr", "-value", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Suffix, "attr", "wrong", true);
        EXPECT_FALSE(selector2.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, SubString)
    {
        xmlNodePtr node = createNode("<test attr='test-value-test'/>");
        AttributeSelector selector(AttributeSelector::Operator::SubString, "attr", "value", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::SubString, "attr", "wrong", true);
        EXPECT_FALSE(selector2.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, Regex)
    {
        xmlNodePtr node = createNode("<test attr='test-value-test'/>");
        AttributeSelector selector(AttributeSelector::Operator::Regex, "attr", "test.*test", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Regex, "attr", "wrong", true);
        EXPECT_FALSE(selector2.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, ToString)
    {
        AttributeSelector selector1(AttributeSelector::Operator::Equals, "id", "testId", false);
        EXPECT_EQ(selector1.toString(), "#testId");

        AttributeSelector selector2(AttributeSelector::Operator::Includes, "class", "testClass", false);
        EXPECT_EQ(selector2.toString(), ".testClass");

        AttributeSelector selector3(AttributeSelector::Operator::DashMatch, "attr", "value", true);
        EXPECT_EQ(selector3.toString(), "[attr|=\"value\"]");

        AttributeSelector selector4(AttributeSelector::Operator::Prefix, "attr", "value", true);
        EXPECT_EQ(selector4.toString(), "[attr^=\"value\"]");

        AttributeSelector selector5(AttributeSelector::Operator::Suffix, "attr", "value", true);
        EXPECT_EQ(selector5.toString(), "[attr$=\"value\"]");

        AttributeSelector selector6(AttributeSelector::Operator::SubString, "attr", "value", true);
        EXPECT_EQ(selector6.toString(), "[attr*=\"value\"]");

        AttributeSelector selector7(AttributeSelector::Operator::Regex, "attr", "value", true);
        EXPECT_EQ(selector7.toString(), "[attr#=\"value\"]");

        AttributeSelector selector8(AttributeSelector::Operator::Equals, "attr", "value", false);
        EXPECT_EQ(selector8.toString(), "[attr=\"value\"s]");

        AttributeSelector selector9(AttributeSelector::Operator::Exists, "attr", "value", false);
        EXPECT_EQ(selector9.toString(), "[attr]");
    }

    TEST(AttributeSelectorTest, Equals_MultipleAttributes)
    {
        xmlNodePtr node = createNode("<test attr1='value1' attr2='value2'/>");
        AttributeSelector selector1(AttributeSelector::Operator::Equals, "attr1", "value1", true);
        EXPECT_TRUE(selector1.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Equals, "attr2", "value2", true);
        EXPECT_TRUE(selector2.matches(node));

        AttributeSelector selector3(AttributeSelector::Operator::Equals, "attr1", "value2", true);
        EXPECT_FALSE(selector3.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, Includes_MultipleIncludes)
    {
        xmlNodePtr node = createNode("<test attr='value1 value2 value3'/>");
        AttributeSelector selector1(AttributeSelector::Operator::Includes, "attr", "value1", true);
        EXPECT_TRUE(selector1.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Includes, "attr", "value2", true);
        EXPECT_TRUE(selector2.matches(node));

        AttributeSelector selector3(AttributeSelector::Operator::Includes, "attr", "value3", true);
        EXPECT_TRUE(selector3.matches(node));

        AttributeSelector selector4(AttributeSelector::Operator::Includes, "attr", "value4", true);
        EXPECT_FALSE(selector4.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, DashMatch_DifferentDash)
    {
        xmlNodePtr node = createNode("<test attr='value_test'/>");
        AttributeSelector selector(AttributeSelector::Operator::DashMatch, "attr", "value", true);
        EXPECT_FALSE(selector.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, Prefix_PartialMatch)
    {
        xmlNodePtr node = createNode("<test attr='value-test'/>");
        AttributeSelector selector(AttributeSelector::Operator::Prefix, "attr", "val", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Prefix, "attr", "value-tes", true);
        EXPECT_TRUE(selector2.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, Suffix_PartialMatch)
    {
        xmlNodePtr node = createNode("<test attr='test-value'/>");
        AttributeSelector selector(AttributeSelector::Operator::Suffix, "attr", "lue", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Suffix, "attr", "test-val", true);
        EXPECT_FALSE(selector2.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, SubString_Overlapping)
    {
        xmlNodePtr node = createNode("<test attr='valuevalue'/>");
        AttributeSelector selector(AttributeSelector::Operator::SubString, "attr", "value", true);
        EXPECT_TRUE(selector.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, Regex_ComplexRegex)
    {
        xmlNodePtr node = createNode("<test attr='abc123def456ghi'/>");
        AttributeSelector selector(AttributeSelector::Operator::Regex, "attr", "[0-9]+", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Regex, "attr", "[a-z]{3}[0-9]{3}", true);
        EXPECT_TRUE(selector2.matches(node));

        freeNode(node);
    }

    TEST(AttributeSelectorTest, ToString_SpecialChars)
    {
        AttributeSelector selector1(AttributeSelector::Operator::Equals, "attr", "value with spaces", true);
        EXPECT_EQ(selector1.toString(), "[attr=\"value with spaces\"]");

        AttributeSelector selector2(AttributeSelector::Operator::Regex, "attr", ".*[a-z]+.*", true);
        EXPECT_EQ(selector2.toString(), "[attr#=\".*[a-z]+.*\"]");

        freeNode(createNode("<test attr='dummy'/>"));
    }

    TEST(AttributeSelectorTest, EdgeCase_EmptyString)
    {
        xmlNodePtr node = createNode("<test attr=''/>");
        AttributeSelector selector(AttributeSelector::Operator::Equals, "attr", "", true);
        EXPECT_TRUE(selector.matches(node));

        AttributeSelector selector2(AttributeSelector::Operator::Includes, "attr", "", true);
        EXPECT_FALSE(selector2.matches(node));

        AttributeSelector selector3(AttributeSelector::Operator::DashMatch, "attr", "", true);
        EXPECT_TRUE(selector3.matches(node));

        AttributeSelector selector4(AttributeSelector::Operator::Prefix, "attr", "", true);
        EXPECT_TRUE(selector4.matches(node));

        AttributeSelector selector5(AttributeSelector::Operator::Suffix, "attr", "", true);
        EXPECT_TRUE(selector5.matches(node));

        AttributeSelector selector6(AttributeSelector::Operator::SubString, "attr", "", true);
        EXPECT_TRUE(selector6.matches(node));

        AttributeSelector selector7(AttributeSelector::Operator::Regex, "attr", "", true);
        EXPECT_TRUE(selector7.matches(node));

        freeNode(node);
    }
}