#pragma once
#include <vector>
#include <string>

struct TestCase
{
    std::string description;
    std::string selector;
    std::vector<std::string> expectedMatches;
};

std::ostream &operator<<(std::ostream &os, const TestCase &testCase)
{
#ifdef verbose_test
    os << "TestCase {"
       << "description: \"" << testCase.description << "\", "
       << "selector: \"" << testCase.selector << "\", "
       << "expectedMatches: [";
    for (size_t i = 0; i < testCase.expectedMatches.size(); ++i)
    {
        os << "\"" << testCase.expectedMatches[i] << "\"";
        if (i < testCase.expectedMatches.size() - 1)
        {
            os << ", ";
        }
    }
    os << "]}";
#else
    os << testCase.description;
#endif
    return os;
}

std::vector<TestCase> val{
    //{"Attribute presence selector, matching attribute with non-ASCII characters", "ul[data-中文]", {"attr-presence-ul1"}}, // libxml2 does not support unicode attribute names?
    {":link pseudo-class selectors, matching a and area elements with href attributes", "#pseudo-link :link", {"pseudo-link-a1", "pseudo-link-a2", "pseudo-link-area1"}},
    {":link pseudo-class selectors, matching link elements with href attributes", "#head :link", {"pseudo-link-link1", "pseudo-link-link2"}},
    {":lang pseudo-class selector, matching inherited language", "#pseudo-lang-div1:lang(en)", {"pseudo-lang-div1"}},
    {":lang pseudo-class selector, matching specified language with exact value", "#pseudo-lang-div2:lang(fr)", {"pseudo-lang-div2"}},
    {":lang pseudo-class selector, matching specified language with partial value", "#pseudo-lang-div3:lang(en)", {"pseudo-lang-div3"}},
    {":lang pseudo-class selector, not matching incorrect language", "#pseudo-lang-div4:lang(es-AR)", {}},
    {":enabled pseudo-class selector, matching all enabled form controls", "#pseudo-ui :enabled", {"pseudo-ui-input1", "pseudo-ui-input2", "pseudo-ui-input3", "pseudo-ui-input4", "pseudo-ui-input5", "pseudo-ui-input6", "pseudo-ui-input7", "pseudo-ui-input8", "pseudo-ui-input9", "pseudo-ui-textarea1", "pseudo-ui-button1"}},
    {":enabled pseudo-class selector, matching all disabled form controls", "#pseudo-ui :disabled", {"pseudo-ui-input10", "pseudo-ui-input11", "pseudo-ui-input12", "pseudo-ui-input13", "pseudo-ui-input14", "pseudo-ui-input15", "pseudo-ui-input16", "pseudo-ui-input17", "pseudo-ui-input18", "pseudo-ui-textarea2", "pseudo-ui-button2"}}, // Pseudo-classe :enabled are not supported
    {":checked pseudo-class selector, matching checked radio buttons and checkboxes", "#pseudo-ui :checked", {"pseudo-ui-input4", "pseudo-ui-input6", "pseudo-ui-input13", "pseudo-ui-input15"}},
    //{":first-line pseudo-element (one-colon syntax) selector, not matching any elements", "#pseudo-element:first-line", {}},  // Pseudo-classe :first-line are not supported
    //{"::first-line pseudo-element (two-colon syntax) selector, not matching any elements", "#pseudo-element::first-line", {}},  // Pseudo-classe ::first-line are not supported
    //{":first-letter pseudo-element (one-colon syntax) selector, not matching any elements", "#pseudo-element:first-letter", {}},  // Pseudo-classe :first-letter are not supported
    //{"::first-letter pseudo-element (two-colon syntax) selector, not matching any elements", "#pseudo-element::first-letter", {}},  // Pseudo-classe ::first-letter are not supported
    //{":before pseudo-element (one-colon syntax) selector, not matching any elements", "#pseudo-element:before", {}},  // Pseudo-classe :before are not supported
    //{"::before pseudo-element (two-colon syntax) selector, not matching any elements", "#pseudo-element::before", {}},  // Pseudo-classe ::before are not supported
    //{":after pseudo-element (one-colon syntax) selector, not matching any elements", "#pseudo-element:after", {}},  // Pseudo-classe :after are not supported
    //{"::after pseudo-element (two-colon syntax) selector, not matching any elements", "#pseudo-element::after", {}},  // Pseudo-classe ::after are not supported
    {"Type selector, matching html element", "html", {"html"}},
    {"Type selector, matching body element", "body", {"body"}},
    {"Universal selector, matching all children of element with specified ID", "#universal>*", {"universal-p1", "universal-hr1", "universal-pre1", "universal-p2", "universal-address1"}},
    {"Universal selector, matching all grandchildren of element with specified ID", "#universal>*>*", {"universal-code1", "universal-span1", "universal-a1", "universal-code2"}},
    {"Universal selector, matching all children of empty element with specified ID", "#empty>*", {}},
    {"Universal selector, matching all descendants of element with specified ID", "#universal *", {"universal-p1", "universal-code1", "universal-hr1", "universal-pre1", "universal-span1", "universal-p2", "universal-a1", "universal-address1", "universal-code2", "universal-a2"}},
    {"Attribute presence selector, matching align attribute with value", ".attr-presence-div1[align]", {"attr-presence-div1"}},
    {"Attribute presence selector, matching align attribute with empty value", ".attr-presence-div2[align]", {"attr-presence-div2"}},
    {"Attribute presence selector, matching title attribute, case insensitivity", "#attr-presence [TiTlE]", {"attr-presence-a1", "attr-presence-span1"}},
    {"Attribute presence selector, matching custom data-* attribute", "[data-attr-presence]", {"attr-presence-pre1", "attr-presence-blockquote1"}},
    {"Attribute presence selector, not matching attribute with similar name", ".attr-presence-div3[align] .attr-presence-div4[align]", {}},
    {"Attribute presence selector, not matching default option without selected attribute", "#attr-presence-select1 option[selected]", {}},
    {"Attribute presence selector, matching option with selected attribute", "#attr-presence-select2 option[selected]", {"attr-presence-select2-option4"}},
    {"Attribute presence selector, matching multiple options with selected attributes", "#attr-presence-select3 option[selected]", {"attr-presence-select3-option2", "attr-presence-select3-option3"}},
    {"Attribute value selector, matching align attribute with value", "#attr-value [align=\"center\"]", {"attr-value-div1"}},
    {"Attribute value selector, matching align attribute with empty value", "#attr-value [align=\"\"]", {"attr-value-div2"}},
    {"Attribute value selector, not matching align attribute with partial value", "#attr-value [align=\"c\"]", {}},
    {"Attribute value selector, not matching align attribute with incorrect value", "#attr-value [align=\"centera\"]", {}},
    {"Attribute value selector, matching custom data-* attribute with unicode escaped value", "[data-attr-value=\"\\e9\"]", {"attr-value-div3"}},
    {"Attribute value selector, matching custom data-* attribute with escaped character", "[data-attr-value_foo=\"\\e9\"]", {"attr-value-div4"}},
    {"Attribute value selector with single-quoted value, matching multiple inputs with type attributes", "#attr-value input[type='hidden'], #attr-value input[type='radio']", {"attr-value-input3", "attr-value-input4", "attr-value-input6", "attr-value-input8", "attr-value-input9"}},
    {"Attribute value selector with double-quoted value, matching multiple inputs with type attributes", "#attr-value input[type=\"hidden\"], #attr-value input[type='radio']", {"attr-value-input3", "attr-value-input4", "attr-value-input6", "attr-value-input8", "attr-value-input9"}},
    {"Attribute value selector with unquoted value, matching multiple inputs with type attributes", "#attr-value input[type=hidden], #attr-value input[type=radio]", {"attr-value-input3", "attr-value-input4", "attr-value-input6", "attr-value-input8", "attr-value-input9"}},
    {"Attribute value selector, matching attribute with value using non-ASCII characters", "[data-attr-value=中文]", {"attr-value-div5"}},
    {"Attribute whitespace-separated list selector, matching class attribute with value", "#attr-whitespace [class~=\"div1\"]", {"attr-whitespace-div1"}},
    {"Attribute whitespace-separated list selector, not matching class attribute with empty value", "#attr-whitespace [class~=\"\"]", {}},
    {"Attribute whitespace-separated list selector, not matching class attribute with partial value", "[data-attr-whitespace~=\"div\"]", {}},
    {"Attribute whitespace-separated list selector, matching custom data-* attribute with unicode escaped value", "[data-attr-whitespace~=\"\\0000e9\"]", {"attr-whitespace-div4"}},
    {"Attribute whitespace-separated list selector, matching custom data-* attribute with escaped character", "[data-attr-whitespace_foo~=\"\\e9\"]", {"attr-whitespace-div5"}},
    {"Attribute whitespace-separated list selector with single-quoted value, matching multiple links with rel attributes", "#attr-whitespace a[rel~='bookmark'], #attr-whitespace a[rel~='nofollow']", {"attr-whitespace-a1", "attr-whitespace-a2", "attr-whitespace-a3", "attr-whitespace-a5", "attr-whitespace-a7"}},
    {"Attribute whitespace-separated list selector with double-quoted value, matching multiple links with rel attributes", "#attr-whitespace a[rel~=\"bookmark\"], #attr-whitespace a[rel~='nofollow']", {"attr-whitespace-a1", "attr-whitespace-a2", "attr-whitespace-a3", "attr-whitespace-a5", "attr-whitespace-a7"}},
    {"Attribute whitespace-separated list selector with unquoted value, matching multiple links with rel attributes", "#attr-whitespace a[rel~=bookmark], #attr-whitespace a[rel~=nofollow]", {"attr-whitespace-a1", "attr-whitespace-a2", "attr-whitespace-a3", "attr-whitespace-a5", "attr-whitespace-a7"}},
    {"Attribute whitespace-separated list selector with double-quoted value, not matching value with space", "#attr-whitespace a[rel~=\"book mark\"]", {}},
    {"Attribute whitespace-separated list selector, matching title attribute with value using non-ASCII characters", "#attr-whitespace [title~=中文]", {"attr-whitespace-p1"}},
    {"Attribute hyphen-separated list selector, not matching unspecified lang attribute", "#attr-hyphen-div1[lang|=\"en\"]", {}},
    {"Attribute hyphen-separated list selector, matching lang attribute with exact value", "#attr-hyphen-div2[lang|=\"fr\"]", {"attr-hyphen-div2"}},
    {"Attribute hyphen-separated list selector, matching lang attribute with partial value", "#attr-hyphen-div3[lang|=\"en\"]", {"attr-hyphen-div3"}},
    {"Attribute hyphen-separated list selector, not matching incorrect value", "#attr-hyphen-div4[lang|=\"es-AR\"]", {}},
    {"Attribute begins with selector, matching href attributes beginning with specified substring", "#attr-begins a[href^=\"https://www\"]", {"attr-begins-a1", "attr-begins-a3"}},
    {"Attribute begins with selector, matching lang attributes beginning with specified substring, ", "#attr-begins [lang^=\"en-\"]", {"attr-begins-div2", "attr-begins-div4"}},
    {"Attribute begins with selector, not matching class attribute not beginning with specified substring", "#attr-begins[class^=apple]", {}},
    {"Attribute begins with selector with single-quoted value, matching class attribute beginning with specified substring", "#attr-begins [class^=' apple']", {"attr-begins-p1"}},
    {"Attribute begins with selector with double-quoted value, matching class attribute beginning with specified substring", "#attr-begins [class^=\" apple\"]", {"attr-begins-p1"}},
    {"Attribute begins with selector with unquoted value, not matching class attribute not beginning with specified substring", "#attr-begins [class^= apple]", {}},
    {"Attribute ends with selector, matching href attributes ending with specified substring", "#attr-ends a[href$=\".org\"]", {"attr-ends-a1", "attr-ends-a3"}},
    {"Attribute ends with selector, matching lang attributes ending with specified substring, ", "#attr-ends [lang$=\"-CH\"]", {"attr-ends-div2", "attr-ends-div4"}},
    {"Attribute ends with selector, not matching class attribute not ending with specified substring", "#attr-ends [class$=apple]", {}},
    {"Attribute ends with selector with single-quoted value, matching class attribute ending with specified substring", "#attr-ends [class$='apple ']", {"attr-ends-p1"}},
    {"Attribute ends with selector with double-quoted value, matching class attribute ending with specified substring", "#attr-ends [class$=\"apple \"]", {"attr-ends-p1"}},
    {"Attribute ends with selector with unquoted value, not matching class attribute not ending with specified substring", "#attr-ends [class$=apple ]", {}},
    {"Attribute contains selector, matching href attributes beginning with specified substring", "#attr-contains a[href*=\"https://www\"]", {"attr-contains-a1", "attr-contains-a3"}},
    {"Attribute contains selector, matching href attributes ending with specified substring", "#attr-contains a[href*=\".org\"]", {"attr-contains-a1", "attr-contains-a2"}},
    {"Attribute contains selector, matching href attributes containing specified substring", "#attr-contains a[href*=\".example.\"]", {"attr-contains-a1", "attr-contains-a3"}},
    {"Attribute contains selector, matching lang attributes beginning with specified substring, ", "#attr-contains [lang*=\"en-\"]", {"attr-contains-div2", "attr-contains-div6"}},
    {"Attribute contains selector, matching lang attributes ending with specified substring, ", "#attr-contains [lang*=\"-CH\"]", {"attr-contains-div3", "attr-contains-div5"}},
    {"Attribute contains selector with single-quoted value, matching class attribute beginning with specified substring", "#attr-contains [class*=' apple']", {"attr-contains-p1"}},
    {"Attribute contains selector with single-quoted value, matching class attribute ending with specified substring", "#attr-contains [class*='orange ']", {"attr-contains-p1"}},
    {"Attribute contains selector with single-quoted value, matching class attribute containing specified substring", "#attr-contains [class*='ple banana ora']", {"attr-contains-p1"}},
    {"Attribute contains selector with double-quoted value, matching class attribute beginning with specified substring", "#attr-contains [class*=\" apple\"]", {"attr-contains-p1"}},
    {"Attribute contains selector with double-quoted value, matching class attribute ending with specified substring", "#attr-contains [class*=\"orange \"]", {"attr-contains-p1"}},
    {"Attribute contains selector with double-quoted value, matching class attribute containing specified substring", "#attr-contains [class*=\"ple banana ora\"]", {"attr-contains-p1"}},
    {"Attribute contains selector with unquoted value, matching class attribute beginning with specified substring", "#attr-contains [class*= apple]", {"attr-contains-p1"}},
    {"Attribute contains selector with unquoted value, matching class attribute ending with specified substring", "#attr-contains [class*=orange ]", {"attr-contains-p1"}},
    {"Attribute contains selector with unquoted value, matching class attribute containing specified substring", "#attr-contains [class*= banana ]", {"attr-contains-p1"}},
    {":root pseudo-class selector, matching document root element", ":root", {"html"}},
    {":nth-child selector, matching the third child element", "#pseudo-nth-table1 :nth-child(3)", {"pseudo-nth-td3", "pseudo-nth-td9", "pseudo-nth-tr3", "pseudo-nth-td15"}},
    {":nth-child selector, matching every third child element", "#pseudo-nth li:nth-child(3n)", {"pseudo-nth-li3", "pseudo-nth-li6", "pseudo-nth-li9", "pseudo-nth-li12"}},
    {":nth-child selector, matching every second child element, starting from the fourth", "#pseudo-nth li:nth-child(2n+4)", {"pseudo-nth-li4", "pseudo-nth-li6", "pseudo-nth-li8", "pseudo-nth-li10", "pseudo-nth-li12"}},
    {":nth-child selector, matching every fourth child element, starting from the third", "#pseudo-nth-p1 :nth-child(4n-1)", {"pseudo-nth-em2", "pseudo-nth-span3"}},
    {":nth-last-child selector, matching the third last child element", "#pseudo-nth-table1 :nth-last-child(3)", {"pseudo-nth-tr1", "pseudo-nth-td4", "pseudo-nth-td10", "pseudo-nth-td16"}},
    {":nth-last-child selector, matching every third child element from the end", "#pseudo-nth li:nth-last-child(3n)", {"pseudo-nth-li1", "pseudo-nth-li4", "pseudo-nth-li7", "pseudo-nth-li10"}},
    {":nth-last-child selector, matching every second child element from the end, starting from the fourth last", "#pseudo-nth li:nth-last-child(2n+4)", {"pseudo-nth-li1", "pseudo-nth-li3", "pseudo-nth-li5", "pseudo-nth-li7", "pseudo-nth-li9"}},
    {":nth-last-child selector, matching every fourth element from the end, starting from the third last", "#pseudo-nth-p1 :nth-last-child(4n-1)", {"pseudo-nth-span2", "pseudo-nth-span4"}},
    {":nth-of-type selector, matching the third em element", "#pseudo-nth-p1 em:nth-of-type(3)", {"pseudo-nth-em3"}},
    {":nth-of-type selector, matching every second element of their type", "#pseudo-nth-p1 :nth-of-type(2n)", {"pseudo-nth-em2", "pseudo-nth-span2", "pseudo-nth-span4", "pseudo-nth-strong2", "pseudo-nth-em4"}},
    {":nth-of-type selector, matching every second elemetn of their type, starting from the first", "#pseudo-nth-p1 span:nth-of-type(2n-1)", {"pseudo-nth-span1", "pseudo-nth-span3"}},
    {":nth-last-of-type selector, matching the thrid last em element", "#pseudo-nth-p1 em:nth-last-of-type(3)", {"pseudo-nth-em2"}},
    {":nth-last-of-type selector, matching every second last element of their type", "#pseudo-nth-p1 :nth-last-of-type(2n)", {"pseudo-nth-span1", "pseudo-nth-em1", "pseudo-nth-strong1", "pseudo-nth-em3", "pseudo-nth-span3"}},
    {":nth-last-of-type selector, matching every second last element of their type, starting from the last", "#pseudo-nth-p1 span:nth-last-of-type(2n-1)", {"pseudo-nth-span2", "pseudo-nth-span4"}},
    {":first-of-type selector, matching the first em element", "#pseudo-nth-p1 em:first-of-type", {"pseudo-nth-em1"}},
    {":first-of-type selector, matching the first of every type of element", "#pseudo-nth-p1 :first-of-type", {"pseudo-nth-span1", "pseudo-nth-em1", "pseudo-nth-strong1"}},
    {":first-of-type selector, matching the first td element in each table row", "#pseudo-nth-table1 tr :first-of-type", {"pseudo-nth-td1", "pseudo-nth-td7", "pseudo-nth-td13"}},
    {":last-of-type selector, matching the last em elemnet", "#pseudo-nth-p1 em:last-of-type", {"pseudo-nth-em4"}},
    {":last-of-type selector, matching the last of every type of element", "#pseudo-nth-p1 :last-of-type", {"pseudo-nth-span4", "pseudo-nth-strong2", "pseudo-nth-em4"}},
    {":last-of-type selector, matching the last td element in each table row", "#pseudo-nth-table1 tr :last-of-type", {"pseudo-nth-td6", "pseudo-nth-td12", "pseudo-nth-td18"}},
    {":first-child pseudo-class selector, matching first child div element", "#pseudo-first-child div:first-child", {"pseudo-first-child-div1"}},
    {":first-child pseudo-class selector, doesn't match non-first-child elements", ".pseudo-first-child-div2:first-child, .pseudo-first-child-div3:first-child", {}},
    {":first-child pseudo-class selector, matching first-child of multiple elements", "#pseudo-first-child span:first-child", {"pseudo-first-child-span1", "pseudo-first-child-span3", "pseudo-first-child-span5"}},
    {":last-child pseudo-class selector, matching last child div element", "#pseudo-last-child div:last-child", {"pseudo-last-child-div3"}},
    {":last-child pseudo-class selector, doesn't match non-last-child elements", ".pseudo-last-child-div1:last-child, .pseudo-last-child-div2:first-child", {}},
    {":last-child pseudo-class selector, matching first-child of multiple elements", "#pseudo-last-child span:last-child", {"pseudo-last-child-span2", "pseudo-last-child-span4", "pseudo-last-child-span6"}},
    {":pseudo-only-child pseudo-class selector, matching all only-child elements", "#pseudo-only :only-child", {"pseudo-only-span1"}},
    {":pseudo-only-child pseudo-class selector, matching only-child em elements", "#pseudo-only em:only-child", {}},
    {":pseudo-only-of-type pseudo-class selector, matching all elements with no siblings of the same type", "#pseudo-only :only-of-type", {"pseudo-only-span1", "pseudo-only-em1"}},
    {":pseudo-only-of-type pseudo-class selector, matching em elements with no siblings of the same type", "#pseudo-only em:only-of-type", {"pseudo-only-em1"}},
    {":empty pseudo-class selector, matching empty p elements", "#pseudo-empty p:empty", {"pseudo-empty-p1", "pseudo-empty-p2", "pseudo-empty-p3"}},
    {":empty pseudo-class selector, matching all empty elements", "#pseudo-empty :empty", {"pseudo-empty-p1", "pseudo-empty-p2", "pseudo-empty-p3", "pseudo-empty-span1"}},
    {":not pseudo-class selector, matching ", "#not>:not(div)", {"not-p1", "not-p2", "not-p3"}},
    {":not pseudo-class selector, matching ", "#not * :not(:first-child)", {"not-em1", "not-em2", "not-em3"}},
    {":not pseudo-class selector, matching nothing", ":not(*)", {}},
    {":not pseudo-class selector, matching nothing", ":not(*)", {}},
    {"Class selector, matching element with specified class", ".class-p", {"class-p1", "class-p2", "class-p3"}},
    {"Class selector, chained, matching only elements with all specified classes", "#class .apple.orange.banana", {"class-div1", "class-div2", "class-p4", "class-div3", "class-p6", "class-div4"}},
    {"Class Selector, chained, with type selector", "div.apple.banana.orange", {"class-div1", "class-div2", "class-div3", "class-div4"}},
    {"Class selector, matching element with class value using non-ASCII characters", ".台北Táiběi", {"class-span1"}},
    {"Class selector, matching multiple elements with class value using non-ASCII characters", ".台北", {"class-span1", "class-span2"}},
    {"Class selector, chained, matching element with multiple class values using non-ASCII characters", ".台北Táiběi.台北", {"class-span1"}},
    {"Class selector, matching element with class with escaped character", ".foo\\:bar", {"class-span3"}},
    {"Class selector, matching element with class with escaped character", ".test\\.foo\\[5\\]bar", {"class-span4"}},
    {"ID selector, matching element with specified id", "#id #id-div1", {"id-div1"}},
    {"ID selector, chained, matching element with specified id", "#id-div1, #id-div1", {"id-div1"}},
    {"ID selector, chained, matching element with specified id", "#id-div1, #id-div2", {"id-div1", "id-div2"}},
    {"ID Selector, chained, with type selector", "div#id-div1, div#id-div2", {"id-div1", "id-div2"}},
    {"ID selector, not matching non-existent descendant", "#id #none", {}},
    {"ID selector, not matching non-existent ancestor", "#none #id-div1", {}},
    {"ID selector, matching multiple elements with duplicate id", "#id-li-duplicate", {"id-li-duplicate", "id-li-duplicate", "id-li-duplicate", "id-li-duplicate"}},
    {"ID selector, matching id value using non-ASCII characters", "#台北Táiběi", {"台北Táiběi"}},
    {"ID selector, matching id value using non-ASCII characters", "#台北", {"台北"}},
    {"ID selector, matching id values using non-ASCII characters", "#台北Táiběi, #台北", {"台北Táiběi", "台北"}},
    {"ID selector, matching element with id with escaped character", "#\\#foo\\:bar", {"#foo:bar"}},
    {"ID selector, matching element with id with escaped character", "#test\\.foo\\[5\\]bar", {"test.foo[5]bar"}},
    {"Descendant combinator, matching element that is a descendant of an element with id", "#descendant div", {"descendant-div1", "descendant-div2", "descendant-div3", "descendant-div4"}},
    {"Descendant combinator, matching element with id that is a descendant of an element", "body #descendant-div1", {"descendant-div1"}},
    {"Descendant combinator, matching element with id that is a descendant of an element", "div #descendant-div1", {"descendant-div1"}},
    {"Descendant combinator, matching element with id that is a descendant of an element with id", "#descendant #descendant-div2", {"descendant-div2"}},
    {"Descendant combinator, matching element with class that is a descendant of an element with id", "#descendant .descendant-div2", {"descendant-div2"}},
    {"Descendant combinator, matching element with class that is a descendant of an element with class", ".descendant-div1 .descendant-div3", {"descendant-div3"}},
    {"Descendant combinator, not matching element with id that is not a descendant of an element with id", "#descendant-div1 #descendant-div4", {}},
    {"Descendant combinator, whitespace characters", "#descendant\t\r\n#descendant-div2", {"descendant-div2"}},
    {"Child combinator, matching element that is a child of an element with id", "#child>div", {"child-div1", "child-div4"}},
    {"Child combinator, matching element with id that is a child of an element", "div>#child-div1", {"child-div1"}},
    {"Child combinator, matching element with id that is a child of an element with id", "#child>#child-div1", {"child-div1"}},
    {"Child combinator, matching element with id that is a child of an element with class", "#child-div1>.child-div2", {"child-div2"}},
    {"Child combinator, matching element with class that is a child of an element with class", ".child-div1>.child-div2", {"child-div2"}},
    {"Child combinator, not matching element with id that is not a child of an element with id", "#child>#child-div3", {}},
    {"Child combinator, not matching element with id that is not a child of an element with class", "#child-div1>.child-div3", {}},
    {"Child combinator, not matching element with class that is not a child of an element with class", ".child-div1>.child-div3", {}},
    {"Child combinator, surrounded by whitespace", "#child-div1\t\r\n>\t\r\n#child-div2", {"child-div2"}},
    {"Child combinator, whitespace after", "#child-div1>\t\r\n#child-div2", {"child-div2"}},
    {"Child combinator, whitespace before", "#child-div1\t\r\n>#child-div2", {"child-div2"}},
    {"Child combinator, no whitespace", "#child-div1>#child-div2", {"child-div2"}},
    {"Adjacent sibling combinator, matching element that is an adjacent sibling of an element with id", "#adjacent-div2+div", {"adjacent-div4"}},
    {"Adjacent sibling combinator, matching element with id that is an adjacent sibling of an element", "div+#adjacent-div4", {"adjacent-div4"}},
    {"Adjacent sibling combinator, matching element with id that is an adjacent sibling of an element with id", "#adjacent-div2+#adjacent-div4", {"adjacent-div4"}},
    {"Adjacent sibling combinator, matching element with class that is an adjacent sibling of an element with id", "#adjacent-div2+.adjacent-div4", {"adjacent-div4"}},
    {"Adjacent sibling combinator, matching element with class that is an adjacent sibling of an element with class", ".adjacent-div2+.adjacent-div4", {"adjacent-div4"}},
    {"Adjacent sibling combinator, matching p element that is an adjacent sibling of a div element", "#adjacent div+p", {"adjacent-p2"}},
    {"Adjacent sibling combinator, not matching element with id that is not an adjacent sibling of an element with id", "#adjacent-div2+#adjacent-p2, #adjacent-div2+#adjacent-div1", {}},
    {"Adjacent sibling combinator, surrounded by whitespace", "#adjacent-p2\t\r\n+\t\r\n#adjacent-p3", {"adjacent-p3"}},
    {"Adjacent sibling combinator, whitespace after", "#adjacent-p2+\t\r\n#adjacent-p3", {"adjacent-p3"}},
    {"Adjacent sibling combinator, whitespace before", "#adjacent-p2\t\r\n+#adjacent-p3", {"adjacent-p3"}},
    {"Adjacent sibling combinator, no whitespace", "#adjacent-p2+#adjacent-p3", {"adjacent-p3"}},
    {"General sibling combinator, matching element that is a sibling of an element with id", "#sibling-div2~div", {"sibling-div4", "sibling-div6"}},
    {"General sibling combinator, matching element with id that is a sibling of an element", "div~#sibling-div4", {"sibling-div4"}},
    {"General sibling combinator, matching element with id that is a sibling of an element with id", "#sibling-div2~#sibling-div4", {"sibling-div4"}},
    {"General sibling combinator, matching element with class that is a sibling of an element with id", "#sibling-div2~.sibling-div", {"sibling-div4", "sibling-div6"}},
    {"General sibling combinator, matching p element that is a sibling of a div element", "#sibling div~p", {"sibling-p2", "sibling-p3"}},
    {"General sibling combinator, not matching element with id that is not a sibling after a p element", "#sibling>p~div", {}},
    {"General sibling combinator, not matching element with id that is not a sibling after an element with id", "#sibling-div2~#sibling-div3, #sibling-div2~#sibling-div1", {}},
    {"General sibling combinator, surrounded by whitespace", "#sibling-p2\t\r\n~\t\r\n#sibling-p3", {"sibling-p3"}},
    {"General sibling combinator, whitespace after", "#sibling-p2~\t\r\n#sibling-p3", {"sibling-p3"}},
    {"General sibling combinator, whitespace before", "#sibling-p2\t\r\n~#sibling-p3", {"sibling-p3"}},
    {"General sibling combinator, no whitespace", "#sibling-p2~#sibling-p3", {"sibling-p3"}},
    {"Syntax, group of selectors separator, surrounded by whitespace", "#group em\t\r \n,\t\r \n#group strong", {"group-em1", "group-strong1"}},
    {"Syntax, group of selectors separator, whitespace after", "#group em,\t\r\n#group strong", {"group-em1", "group-strong1"}},
    {"Syntax, group of selectors separator, whitespace before", "#group em\t\r\n,#group strong", {"group-em1", "group-strong1"}},
    {"Syntax, group of selectors separator, no whitespace", "#group em,#group strong", {"group-em1", "group-strong1"}}};