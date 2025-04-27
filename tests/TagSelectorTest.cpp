#include <gtest/gtest.h>
#include "TestsUtils.hpp"

namespace lxml2query
{
    class TagSelectorTest : public ::testing::Test
    {
    protected:
        void SetUp() override
        {
            doc_ = htmlReadMemory(html_content_.c_str(), html_content_.size(), "", nullptr, HTML_PARSE_NOWARNING | HTML_PARSE_NOERROR);
            ASSERT_NE(doc_, nullptr);
            root_ = xmlDocGetRootElement(doc_);
            ASSERT_NE(root_, nullptr);
        }

        void TearDown() override
        {
            if (doc_)
            {
                xmlFreeDoc(doc_);
            }
            xmlCleanupParser();
        }

        xmlDocPtr doc_ = nullptr;
        xmlNodePtr root_ = nullptr;
        std::string html_content_ = "<html><body>"
                                    "<div id='div1'><p id='p1'>Text1</p><p id='p2'>Text2</p><span id='span1'>Span1</span></div>"
                                    "<div id='div2'><span id='s1'>Text3</span></div>"
                                    "</body></html>";
    };

    TEST_F(TagSelectorTest, EmptyOperator_AlwaysFalse)
    {
        TagSelector selector(TagSelector::Operator::Empty);

        ASSERT_FALSE(selector.matches(root_));
        ASSERT_FALSE(selector.matches(nullptr));
    }

    TEST_F(TagSelectorTest, OnlyChildOperator_True)
    {
        TagSelector selector(TagSelector::Operator::OnlyChild);
        xmlNodePtr span1 = GetElementById(root_, "s1");

        ASSERT_NE(span1, nullptr);
        ASSERT_TRUE(selector.matches(span1));
    }

    TEST_F(TagSelectorTest, OnlyChildOperator_False)
    {
        TagSelector selector(TagSelector::Operator::OnlyChild);
        xmlNodePtr p1 = GetElementById(root_, "p1");

        ASSERT_NE(p1, nullptr);
        ASSERT_FALSE(selector.matches(p1));
    }

    TEST_F(TagSelectorTest, OnlyChildOfTypeOperator_True)
    {
        TagSelector selector(true);
        xmlNodePtr p1 = GetElementById(root_, "p1");

        ASSERT_NE(p1, nullptr);
        ASSERT_FALSE(selector.matches(p1));
    }

    TEST_F(TagSelectorTest, OnlyChildOfTypeOperator_False)
    {
        TagSelector selector(true);
        xmlNodePtr span1 = GetElementById(root_, "s1");

        ASSERT_NE(span1, nullptr);
        ASSERT_TRUE(selector.matches(span1));
    }

    TEST_F(TagSelectorTest, NthChildOperator_True)
    {
        TagSelector selector(0, 1, false, false); // 1st child
        xmlNodePtr p1 = GetElementById(root_, "p1");

        ASSERT_NE(p1, nullptr);
        ASSERT_TRUE(selector.matches(p1));
    }

    TEST_F(TagSelectorTest, NthChildOperator_False)
    {
        TagSelector selector(0, 2, false, false); // 2nd child
        xmlNodePtr p1 = GetElementById(root_, "p1");

        ASSERT_NE(p1, nullptr);
        ASSERT_FALSE(selector.matches(p1));
    }

    TEST_F(TagSelectorTest, NthLastChildOperator_True)
    {
        TagSelector selector(0, 1, true, false); // Last child
        xmlNodePtr span1 = GetElementById(root_, "span1");

        ASSERT_NE(span1, nullptr);
        ASSERT_TRUE(selector.matches(span1));
    }

    TEST_F(TagSelectorTest, NthLastChildOperator_False)
    {
        TagSelector selector(0, 1, true, false); // Last child
        xmlNodePtr p1 = GetElementById(root_, "p1");

        ASSERT_NE(p1, nullptr);
        ASSERT_FALSE(selector.matches(p1));
    }

    TEST_F(TagSelectorTest, NthChildOfTypeOperator_True)
    {
        TagSelector selector(0, 1, false, true); // 1st child of type p
        xmlNodePtr p1 = GetElementById(root_, "p1");

        ASSERT_NE(p1, nullptr);
        ASSERT_TRUE(selector.matches(p1));
    }

    TEST_F(TagSelectorTest, NthChildOfTypeOperator_False)
    {
        TagSelector selector(0, 2, false, true); // 2nd child of type p
        xmlNodePtr span1 = GetElementById(root_, "span1");

        ASSERT_NE(span1, nullptr);
        ASSERT_FALSE(selector.matches(span1));
    }

    TEST_F(TagSelectorTest, TagOperator_True)
    {
        std::string p("p");
        TagSelector selector(p);
        xmlNodePtr p1 = GetElementById(root_, "p1");

        ASSERT_NE(p1, nullptr);
        ASSERT_TRUE(selector.matches(p1));
    }

    TEST_F(TagSelectorTest, TagOperator_False)
    {
        TagSelector selector(std::string_view("span"));
        xmlNodePtr p1 = GetElementById(root_, "p1");

        ASSERT_NE(p1, nullptr);
        ASSERT_FALSE(selector.matches(p1));
    }
} // namespace lxml2query