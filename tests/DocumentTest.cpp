#include <gtest/gtest.h>
#include "TestsUtils.hpp"
//#include <sstream>

namespace lxml2query
{
	TEST(DocumentTest, parseMemory_ValidHtml)
	{
		const std::string input_html = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Mini HTML Page</title>
</head>
<body>
    <header>
        <h1>Welcome!</h1>
        <nav>
            <a href="#">Home</a> |
            <a href="#">About</a> |
            <a href="#">Contact</a>
        </nav>
    </header>

    <main>
        <section>
            <h2>This is a Mini HTML Page</h2>
            <p>This is a simple example of an HTML page. It includes a header, navigation, main content, and a footer.</p>
            <ul>
                <li>Item 1</li>
                <li>Item 2</li>
                <li>Item 3</li>
            </ul>
        </section>
    </main>

    <footer>
        <p>&copy; 2024 Mini HTML</p>
    </footer>
</body>
</html>)";

		Document doc;
		ASSERT_TRUE(doc.parseMemory(input_html));
		ASSERT_TRUE(doc.isValid());
	}

	TEST(DocumentTest, parseMemory_EmptyString)
	{
		Document doc;
		ASSERT_FALSE(doc.parseMemory(""));
		ASSERT_FALSE(doc.isValid());
	}

	TEST(DocumentTest, parseMemory_NullTerminatedString)
	{
		const char *null_terminated_html = "<p>Test</p>";
		Document doc;
		ASSERT_TRUE(doc.parseMemory(null_terminated_html));
		ASSERT_TRUE(doc.isValid());
	}

	TEST(DocumentTest, parseMemory_LargeString)
	{
		std::stringstream large_html;
		large_html << "<root>";
		for (int i = 0; i < 1000; ++i)
		{
			large_html << "<item>" << i << "</item>";
		}
		large_html << "</root>";

		Document doc;
		ASSERT_TRUE(doc.parseMemory(large_html.str()));
		ASSERT_TRUE(doc.isValid());
	}
} // namespace lxml2query