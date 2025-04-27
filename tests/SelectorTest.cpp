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

    TEST(SelectorTest, Filter_AllMatch)
    {
        Selector::NodeSet nodes;
        xmlNodePtr node1 = createNode("<div>Test1</div>");
        xmlNodePtr node2 = createNode("<span>Test2</span>");
        nodes.insert(node1);
        nodes.insert(node2);

        MockSelector selector(true);
        Selector::NodeSet result = selector.Filter(nodes);

        EXPECT_EQ(result.size(), 2);
        EXPECT_EQ(result.count(node1), 1);
        EXPECT_EQ(result.count(node2), 1);

        freeNode(node1);
        freeNode(node2);
    }

    TEST(SelectorTest, Filter_NoMatch)
    {
        Selector::NodeSet nodes;
        xmlNodePtr node1 = createNode("<div>Test1</div>");
        xmlNodePtr node2 = createNode("<span>Test2</span>");
        nodes.insert(node1);
        nodes.insert(node2);

        MockSelector selector(false);
        Selector::NodeSet result = selector.Filter(nodes);

        EXPECT_TRUE(result.empty());

        freeNode(node1);
        freeNode(node2);
    }

    TEST(SelectorTest, Filter_PartialMatch)
    {
        Selector::NodeSet nodes;
        xmlNodePtr node1 = createNode("<div>Test1</div>");
        xmlNodePtr node2 = createNode("<span>Test2</span>");
        nodes.insert(node1);
        nodes.insert(node2);

        class PartialMockSelector : public Selector
        {
        public:
            bool matches(xmlNodePtr node) const override
            {
                return std::string((char *)node->name) == "div";
            }
            std::string toString() const override { return "PartialMock"; }
        };

        PartialMockSelector selector;
        Selector::NodeSet result = selector.Filter(nodes);

        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result.count(node1), 1);

        freeNode(node1);
        freeNode(node2);
    }

    TEST(SelectorTest, MatchAll_EmptyNode)
    {
        MockSelector selector(true);
        Selector::NodeSet result = selector.MatchAll(nullptr);
        EXPECT_TRUE(result.empty());
    }

    TEST(SelectorTest, MatchAll_SingleNode)
    {
        xmlNodePtr node = createNode("<div>Test</div>");
        MockSelector selector(true);
        Selector::NodeSet result = selector.MatchAll(node);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result.count(node), 1);
        freeNode(node);
    }

    TEST(SelectorTest, MatchAll_NestedNodes)
    {
        xmlNodePtr node = createNode("<div><span>Test1</span><p>Test2</p></div>");
        MockSelector selector(true);
        Selector::NodeSet result = selector.MatchAll(node);
        EXPECT_EQ(result.size(), 3);
        EXPECT_EQ(result.count(node), 1);
        EXPECT_EQ(result.count(node->children), 1);
        EXPECT_EQ(result.count(node->children->next), 1);
        freeNode(node);
    }

    TEST(SelectorTest, MatchAll_PartialMatch)
    {
        xmlNodePtr node = createNode("<div><span>Test1</span><p>Test2</p></div>");

        class PartialMockSelector : public Selector
        {
        public:
            bool matches(xmlNodePtr node) const override
            {
                return std::string((char *)node->name) == "span";
            }
            std::string toString() const override { return "PartialMock"; }
        };

        PartialMockSelector selector;
        Selector::NodeSet result = selector.MatchAll(node);
        EXPECT_EQ(result.size(), 1);
        EXPECT_EQ(result.count(node->children), 1);
        freeNode(node);
    }

    TEST(SelectorTest, MatchAll_TextNodes)
    {
        xmlNodePtr node = createNode("<div>Text<span>Test1</span>Text2</div>");
        MockSelector selector(true);
        Selector::NodeSet result = selector.MatchAll(node);
        EXPECT_EQ(result.size(), 2);
        EXPECT_EQ(result.count(node), 1);
        EXPECT_EQ(result.count(node->children->next), 1);
        freeNode(node);
    }

    TEST(SelectorTest, ToString_Default)
    {
        Selector selector;
        EXPECT_EQ(selector.toString(), "*");
    }

    TEST(SelectorTest, ToString_Mock)
    {
        MockSelector selector(true);
        EXPECT_EQ(selector.toString(), "MockTrue");
    }
}