#include "gtest/gtest.h"
#include "TestCase.hpp"
#include "TestsUtils.hpp"

namespace lxml2query
{
    class SelectorTest : public ::testing::TestWithParam<TestCase>
    {
    protected:
        void SetUp() override
        {
            doc_.parse("./content.html");
        }

        Document doc_;
    };

    TEST_P(SelectorTest, FindAndMatch)
    {
        const TestCase &testCase = GetParam();

        std::stringstream ss;
        ss << std::endl;
        ss << "  Description: " << testCase.description << std::endl;
        ss << "  Selector: " << testCase.selector << std::endl;
        ss << "  Expected Matches: ";
        for (const auto &match : testCase.expectedMatches)
        {
            ss << match << " ";
        }
        ss << std::endl;

        EXPECT_NO_THROW(doc_.Find(testCase.selector)) << ss.str();

        SelectorPtr selector = SelectorParser::Create(testCase.selector);
        auto res = doc_.Find(selector);

        ss << "  Actual Selector: " << selector->toString() << std::endl;
        ss << "  Actual Matches (" << res.size() << "): ";

        std::vector<std::string> resStr;
        for (auto node : res)
        {
            std::string id = GetNodeId(node);
            resStr.push_back(id);
            ss << id << " ";
        }
        ss << std::endl;

        auto input_selector = stripSelector(testCase.selector);
        auto output_selector = stripSelector(selector->toString());

        ss << "  Input Selector:  " << input_selector << std::endl;
        ss << "  Output Selector: " << output_selector << std::endl;

        EXPECT_TRUE(input_selector == output_selector) << ss.str();
        EXPECT_TRUE(CompareVector(resStr, testCase.expectedMatches)) << ss.str();
    }

    INSTANTIATE_TEST_SUITE_P(
        SelectorTests,
        SelectorTest,
        ::testing::ValuesIn(val));
}