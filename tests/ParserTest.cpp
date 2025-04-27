#include <gtest/gtest.h>
//#include <gtest/gtest-spi.h>

#include "TestsUtils.hpp"

namespace lxml2query
{
    class SelectorParserTest : public ::testing::Test
    {
    protected:
        std::unique_ptr<SelectorParser> parser_;

        void SetUp() override {}

        void TearDown() override {}

        SelectorPtr Parse(std::string_view input)
        {
            return SelectorParser::Create(input);
        }

        xmlNodePtr createXmlNode(const char *name, const char *content = nullptr)
        {
            xmlNodePtr node = xmlNewNode(nullptr, BAD_CAST name);
            if (content)
            {
                xmlNodeAddContent(node, BAD_CAST content);
            }
            return node;
        }

        void freeXmlNode(xmlNodePtr node)
        {
            xmlFreeNode(node);
        }
    };

    TEST_F(SelectorParserTest, Create_ValidInput)
    {
        auto selector = Parse("div");
        EXPECT_NE(selector, nullptr);
    }

    TEST_F(SelectorParserTest, Create_EmptyInput)
    {
        EXPECT_THROW(Parse(""), std::runtime_error);
    }

    TEST_F(SelectorParserTest, ParseTypeSelector_Valid)
    {
        auto selector = Parse("div");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), "div");
    }

    TEST_F(SelectorParserTest, ParseIdSelector_Valid)
    {
        auto selector = Parse("#myId");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), "#myId");
    }

    TEST_F(SelectorParserTest, ParseClassSelector_Valid)
    {
        auto selector = Parse(".myClass");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), ".myClass");
    }

    TEST_F(SelectorParserTest, ParseAttributeSelector_Valid)
    {
        auto selector = Parse("[data-value=\"test\"]");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), "[data-value=\"test\"]");
    }

    TEST_F(SelectorParserTest, ParseAttributeSelector_NoMatch)
    {
        auto selector = Parse("[data-value=\"test\"]");
        ASSERT_NE(selector, nullptr);
        xmlNodePtr node = createXmlNode("div");
        xmlNewProp(node, BAD_CAST "data-value", BAD_CAST "other");
        EXPECT_FALSE(selector->matches(node));
        freeXmlNode(node);
    }

    TEST_F(SelectorParserTest, ParsePseudoclassSelector_Valid)
    {
        auto selector = Parse(":first-child()");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), ":first-child()");
    }

    TEST_F(SelectorParserTest, ParseNth_Valid)
    {
        auto selector = Parse(":nth-child(2n+1)");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), ":nth-child(odd)"); // 2n+1
    }

    TEST_F(SelectorParserTest, ParseMultipleSelectors_Valid)
    {
        auto selector = Parse("div#myId.myClass[data-value=\"test\"]:first-child():nth-child(2n+3)");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), "div #myId .myClass [data-value=\"test\"] :first-child() :nth-child(2n+3)");
    }

    TEST_F(SelectorParserTest, ParseMultipleClassSelectors_Valid)
    {
        auto selector = Parse(".class1.class2.class3");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), ".class1 .class2 .class3");
    }

    TEST_F(SelectorParserTest, ParseMultipleAttributeSelectors_Valid)
    {
        auto selector = Parse("[attr1=\"val1\"][attr2=\"val2\"][attr3=\"val3\"]");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), "[attr1=\"val1\"] [attr2=\"val2\"] [attr3=\"val3\"]");
    }

    TEST_F(SelectorParserTest, ParseAttributeSelector_WithSpaces_Valid)
    {
        auto selector = Parse("[ data-value = \"test\" ]");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), "[data-value=\"test\"]");
    }

    TEST_F(SelectorParserTest, ParseAttributeSelector_DifferentOperators_Valid)
    {
        auto selector1 = Parse("[attr~=\"value\"]");
        ASSERT_NE(selector1, nullptr);
        EXPECT_EQ(selector1->toString(), "[attr~=\"value\"]");

        auto selector2 = Parse("[attr|=\"value\"]");
        ASSERT_NE(selector2, nullptr);
        EXPECT_EQ(selector2->toString(), "[attr|=\"value\"]");

        auto selector3 = Parse("[attr^=\"value\"]");
        ASSERT_NE(selector3, nullptr);
        EXPECT_EQ(selector3->toString(), "[attr^=\"value\"]");

        auto selector4 = Parse("[attr$=\"value\"]");
        ASSERT_NE(selector4, nullptr);
        EXPECT_EQ(selector4->toString(), "[attr$=\"value\"]");

        auto selector5 = Parse("[attr*=\"value\"]");
        ASSERT_NE(selector5, nullptr);
        EXPECT_EQ(selector5->toString(), "[attr*=\"value\"]");
    }

    TEST_F(SelectorParserTest, ParseNthChild_DifferentForms_Valid)
    {
        auto selector1 = Parse(":nth-child(even)");

        ASSERT_NE(selector1, nullptr);
        EXPECT_EQ(selector1->toString(), ":nth-child(even)");

        auto selector2 = Parse(":nth-child(odd)");

        ASSERT_NE(selector2, nullptr);
        EXPECT_EQ(selector2->toString(), ":nth-child(odd)");

        auto selector3 = Parse(":nth-child(3n)");

        ASSERT_NE(selector3, nullptr);
        EXPECT_EQ(selector3->toString(), ":nth-child(3n)");

        auto selector4 = Parse(":nth-child(3n-1)");

        ASSERT_NE(selector4, nullptr);
        EXPECT_EQ(selector4->toString(), ":nth-child(3n-1)");

        auto selector5 = Parse(":nth-child(0)");

        ASSERT_NE(selector5, nullptr);
        EXPECT_EQ(selector5->toString(), ":nth-child(0n)");

        auto selector6 = Parse(":nth-child(-1)");

        ASSERT_NE(selector6, nullptr);
        EXPECT_EQ(selector6->toString(), ":nth-child(0n-1)");

        auto selector7 = Parse(":nth-child(+1)");

        ASSERT_NE(selector7, nullptr);
        EXPECT_EQ(selector7->toString(), ":first-child()");
    }

    TEST_F(SelectorParserTest, ParsePseudoclass_DifferentForms_Valid)
    {
        auto selector1 = Parse(":last-child()");
        ASSERT_NE(selector1, nullptr);
        EXPECT_EQ(selector1->toString(), ":last-child()");

        auto selector2 = Parse(":only-child()");
        ASSERT_NE(selector2, nullptr);
        EXPECT_EQ(selector2->toString(), ":only-child()");

        auto selector3 = Parse(":empty()");
        ASSERT_NE(selector3, nullptr);
        EXPECT_EQ(selector3->toString(), ":empty()");
    }

    TEST_F(SelectorParserTest, ParseCombinators_Valid)
    {
        auto selector1 = Parse("div p");
        ASSERT_NE(selector1, nullptr);
        EXPECT_EQ(selector1->toString(), "div p");

        auto selector2 = Parse("div > p");
        ASSERT_NE(selector2, nullptr);
        EXPECT_EQ(selector2->toString(), "div > p");

        auto selector3 = Parse("div + p");
        ASSERT_NE(selector3, nullptr);
        EXPECT_EQ(selector3->toString(), "div + p");

        auto selector4 = Parse("div ~ p");
        ASSERT_NE(selector4, nullptr);
        EXPECT_EQ(selector4->toString(), "div ~ p");
    }

    TEST_F(SelectorParserTest, ParseComplexCombinations_Valid)
    {
        auto selector = Parse("div#id.class > p[attr=\"val\"] + span:first-child()");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), "div #id .class > p [attr=\"val\"] + span :first-child()");
    }

    TEST_F(SelectorParserTest, ParseQuotedAttributeValues_Valid)
    {
        auto selector1 = Parse("[attr=\"quoted value\"]");
        ASSERT_NE(selector1, nullptr);
        EXPECT_EQ(selector1->toString(), "[attr=\"quoted value\"]");

        auto selector2 = Parse("[attr='quoted value']");
        ASSERT_NE(selector2, nullptr);
        EXPECT_EQ(selector2->toString(), "[attr=\"quoted value\"]");
    }

    TEST_F(SelectorParserTest, ParseEscapedCharacters_Valid)
    {
        auto selector1 = Parse("#id\\:with\\:colon");
        ASSERT_NE(selector1, nullptr);
        EXPECT_EQ(selector1->toString(), "#id:with:colon");

        auto selector2 = Parse(".class\\.with\\.dot");
        ASSERT_NE(selector2, nullptr);
        EXPECT_EQ(selector2->toString(), ".class.with.dot");

        auto selector3 = Parse("[attr=\"value\\\"with\\\"quotes\"]");
        ASSERT_NE(selector3, nullptr);
        EXPECT_EQ(selector3->toString(), "[attr=\"value\\\"with\\\"quotes\"]");

        auto selector4 = Parse("[attr='value\\'with\\'quotes']");
        ASSERT_NE(selector4, nullptr);
        EXPECT_EQ(selector4->toString(), "[attr=\"value'with'quotes\"]");

        auto selector94 = Parse("p.highlight[data-index][disabled][disabled] + input.item[lang=\"en\"][disabled][data-index]:not(.hidden)");
        ASSERT_NE(selector94, nullptr);
        EXPECT_EQ(selector94->toString(), "p .highlight [data-index] [disabled] [disabled] + input .item [lang=\"en\"] [disabled] [data-index] :not(.hidden)");

        auto selector9 = Parse("button.item:nth-child(3n+1)  section.error.text[data-index][data-value=\"123\"][href=\"#\"][data-index]");
        ASSERT_NE(selector9, nullptr);
        EXPECT_EQ(selector9->toString(), "button .item :nth-child(3n+1) section .error .text [data-index] [data-value=\"123\"] [href=\"#\"] [data-index]");
    }

    TEST_F(SelectorParserTest, ParseUniversalSelector_Valid)
    {
        auto selector = Parse("*");
        ASSERT_NE(selector, nullptr);
        EXPECT_EQ(selector->toString(), "*");
    }

    TEST_F(SelectorParserTest, ParseInvalidSelectors_ThrowsException)
    {
        EXPECT_NO_THROW(Parse("div[attr='value']"));
        EXPECT_THROW(Parse("div#"), std::runtime_error);
        EXPECT_THROW(Parse("div."), std::runtime_error);
        EXPECT_THROW(Parse("div["), std::runtime_error);
        EXPECT_THROW(Parse("div:"), std::runtime_error);
        EXPECT_THROW(Parse("div>"), std::runtime_error);
        EXPECT_THROW(Parse("div+"), std::runtime_error);
        EXPECT_THROW(Parse("div~"), std::runtime_error);
        EXPECT_THROW(Parse("div[attr=\"value]"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(abc)"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child()"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(n+)"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(n-)"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(n-a)"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(an+)"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(an-)"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(an-b)"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(an+b)"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(an-b)"), std::runtime_error);
        EXPECT_THROW(Parse("div:nth-child(a+b)"), std::runtime_error);
    }

    TEST_F(SelectorParserTest, ParseRandomSelectors_Valid)
    {
        for (int i = 0; i < (1 << 16/*22*/); ++i)
        {
            std::string selector = generateRandomSelector();
            SelectorPtr selectorPtr;

            EXPECT_NO_THROW(selectorPtr = Parse(selector));
            EXPECT_NE(selectorPtr, nullptr);
            EXPECT_EQ(fmtSelector(Parse(selector)->toString()), fmtSelector(selector)) << "\tRandom selector: " << selector;
        }
    }
} // namespace lxml2query