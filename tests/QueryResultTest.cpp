#include <gtest/gtest.h>
#include "TestsUtils.hpp"

namespace lxml2query
{
    class QueryResultTest : public ::testing::Test
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
        }

        xmlDocPtr doc_ = nullptr;
        xmlNodePtr root_ = nullptr;
        std::string html_content_ = "<html><body><div id='div1'><p class='p1'>Text1</p><p class='p2'>Text2</p></div><div id='div2'><span class='s1'>Text3</span></div></body></html>";
    };

    TEST_F(QueryResultTest, Constructor_SingleNode)
    {
        xmlNodePtr div1 = GetElementById(root_, "div1");
        ASSERT_NE(div1, nullptr);
        QueryResult result(div1);

        auto nodes = result();
        ASSERT_EQ(nodes.size(), 1);
        ASSERT_EQ(*nodes.begin(), div1);
    }

    TEST_F(QueryResultTest, Constructor_NodeSet)
    {
        QueryResult::NodeSet nodes;
        xmlNodePtr div1 = GetElementById(root_, "div1");
        xmlNodePtr div2 = GetElementById(root_, "div2");
        ASSERT_NE(div1, nullptr);
        ASSERT_NE(div2, nullptr);
        nodes.insert(div1);
        nodes.insert(div2);

        QueryResult result(nodes);
        auto resultNodes = result();
        ASSERT_EQ(resultNodes.size(), 2);
        ASSERT_TRUE(resultNodes.find(div1) != resultNodes.end());
        ASSERT_TRUE(resultNodes.find(div2) != resultNodes.end());
    }

    TEST_F(QueryResultTest, Iterator_BasicIteration)
    {
        QueryResult::NodeSet nodes;
        xmlNodePtr div1 = GetElementById(root_, "div1");
        xmlNodePtr div2 = GetElementById(root_, "div2");
        nodes.insert(div1);
        nodes.insert(div2);
        QueryResult result(nodes);

        std::vector<xmlNodePtr> iteratedNodes;
        for (auto node : result)
        {
            iteratedNodes.push_back(node);
        }

        ASSERT_EQ(iteratedNodes.size(), 2);
        ASSERT_TRUE(std::find(iteratedNodes.begin(), iteratedNodes.end(), div1) != iteratedNodes.end());
        ASSERT_TRUE(std::find(iteratedNodes.begin(), iteratedNodes.end(), div2) != iteratedNodes.end());
    }

    TEST_F(QueryResultTest, Iterator_IncrementAndDereference)
    {
        QueryResult::NodeSet nodes;
        xmlNodePtr div1 = GetElementById(root_, "div1");
        nodes.insert(div1);
        QueryResult result(nodes);

        auto it = result.begin();
        ASSERT_EQ((*it), div1);
        ++it;
        ASSERT_EQ(it, result.end());
    }

    TEST_F(QueryResultTest, OperatorIndex_ValidIndex)
    {
        QueryResult::NodeSet nodes;
        xmlNodePtr div1 = GetElementById(root_, "div1");
        nodes.insert(div1);
        QueryResult result(nodes);

        auto node = result[0];
        ASSERT_TRUE(node.has_value());
        ASSERT_EQ(node.value(), div1);
    }

    TEST_F(QueryResultTest, OperatorIndex_InvalidIndex)
    {
        QueryResult result(QueryResult::NodeSet{});
        ASSERT_FALSE(result[0].has_value());
    }

    TEST_F(QueryResultTest, Find_StringSelector)
    {
        QueryResult result(root_);
        QueryResult found = result.Find("div p");
        ASSERT_EQ(found.size(), 2);
    }

    TEST_F(QueryResultTest, Find_SelectorPtr)
    {
        QueryResult result(root_);
        SelectorPtr selector = std::make_shared<TagSelector>(std::string("p"));
        QueryResult found = result.Find(selector);
        ASSERT_EQ(found.size(), 2);
    }

    TEST_F(QueryResultTest, Find_StaticStringSelector)
    {
        QueryResult found = QueryResult::Find(root_, "div p");
        ASSERT_EQ(found.size(), 2);
    }

    TEST_F(QueryResultTest, Find_StaticSelectorPtr)
    {
        SelectorPtr selector = std::make_shared<TagSelector>(std::string("p"));
        QueryResult found = QueryResult::Find(root_, selector);

        for (auto it = found.begin(); it != found.end(); ++it)
            printXmlNode(*it);
        ASSERT_EQ(found.size(), 2);
    }

    TEST_F(QueryResultTest, At_ValidIndex)
    {
        QueryResult::NodeSet nodes;
        xmlNodePtr div1 = GetElementById(root_, "div1");
        nodes.insert(div1);
        QueryResult result(nodes);

        auto node = result.at(0);
        ASSERT_TRUE(node.has_value());
        ASSERT_EQ(node.value(), div1);
    }

    TEST_F(QueryResultTest, At_InvalidIndex)
    {
        QueryResult result(QueryResult::NodeSet{});
        ASSERT_FALSE(result.at(0).has_value());
    }

    TEST_F(QueryResultTest, Size_BasicTest)
    {
        QueryResult::NodeSet nodes;
        xmlNodePtr div1 = GetElementById(root_, "div1");
        nodes.insert(div1);
        QueryResult result(nodes);
        ASSERT_EQ(result.size(), 1);
    }

    TEST_F(QueryResultTest, ToVector_BasicTest)
    {
        QueryResult::NodeSet nodes;
        xmlNodePtr div1 = GetElementById(root_, "div1");
        xmlNodePtr div2 = GetElementById(root_, "div2");
        nodes.insert(div1);
        nodes.insert(div2);
        QueryResult result(nodes);

        std::vector<Node> vec = result.toVector(), expected = {div1, div2};
        ASSERT_EQ(vec.size(), 2);
        ASSERT_TRUE(CompareVector(vec, expected));
        //ASSERT_EQ(vec[0], div1);
        //ASSERT_EQ(vec[1], div2);
    }
}