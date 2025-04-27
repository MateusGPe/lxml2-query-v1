# lxml-query: Robust CSS Selector Engine for libxml2

[![CMake on a single platform](https://github.com/MateusGPe/lxml2-query/actions/workflows/cmake-single-platform.yml/badge.svg)](https://github.com/MateusGPe/lxml2-query/actions/workflows/cmake-single-platform.yml)

**lxml-query** is a powerful C++ library that brings comprehensive CSS selector support to libxml2, enabling efficient and precise querying of XML and HTML documents. Built upon the foundation of [Cascadia](https://github.com/andybalholm/cascadia) and inspired by [gumbo-query](https://github.com/lazytiger/gumbo-query/), lxml-query addresses the limitations of its predecessors by leveraging the well-maintained libxml2 parser and implementing crucial bug fixes.

## Key Features

* **Robust HTML/XML Parsing:** Utilizes libxml2, a mature and actively maintained library, for reliable document parsing.
* **Extensive CSS Selector Coverage:** Implements a wide range of CSS selectors, including:
    * **Basic Selectors:** Type, class, ID, and universal selectors.
    * **Attribute Selectors:** Presence, value, and substring matching selectors (e.g., `[attr=value]`, `[attr^=value]`, `[attr*=value]`).
    * **Pseudo-classes:** Functional (e.g., `not(...)`, `has(...)`), tree-structural (e.g., `nth-child(An + B)`, `first-of-type`), and custom extensions.
    * **Combinators:** Descendant, child, next-sibling, and partial subsequent-sibling support.
* **Enhanced Error Handling:** Provides improved error reporting and handling compared to gumbo-query.
* **Custom Pseudo-classes:** Extends standard CSS with custom pseudo-classes like `haschild`, `contains`, `matches`, and their "own" variants for advanced querying.
* **Regex Attribute Matching:** Introduces the `#=` attribute selector for regular expression matching, offering flexible attribute filtering.
* **Modern C++/CMake Integration:** Leverages the [C++/CMake modern boilerplate](https://github.com/Lectem/cpp-boilerplate/) for seamless integration into modern C++ projects.
* **Bug Fixes and Improvements:** Addresses and corrects bugs found in gumbo-query, ensuring more accurate and reliable selector matching.

## Selector Support Details

### Basic Selectors

* Type selectors: `element`
* Class selectors: `.class`
* ID selectors: `#id`
* Universal selector: `*`

### Attribute Selectors

* **Presence and Value:**
    * `[attr]`
    * `[attr=value]`
    * `[attr~=value]` (word matching)
    * `[attr|=value]` (prefix matching)

* **Substring Matching:**
    * `[attr^=value]` (starts with)
    * `[attr$=value]` (ends with)
    * `[attr*=value]` (contains)
    * `[attr#=value]` (regex matching)

### Pseudo-classes

* **Functional:**
    * `not(...)`
    * `has(...)`

* **Tree-Structural:**
    * `nth-child(An + B)`
    * `nth-last-child(An + B)`
    * `nth-of-type(An + B)`
    * `nth-last-of-type(An + B)`
    * `first-child`
    * `last-child`
    * `first-of-type`
    * `last-of-type`
    * `only-child`
    * `only-of-type`
    * `empty`
    * `root`
    * `link` (alias for `:any-link`)
    * `any-link`
    * `lang`
    * `enabled`
    * `disabled`
    * `checked`

* **Custom:**
    * `has-child`
    * `contains`
    * `contains-own`
    * `matches`
    * `matches-own`

### Combinators

* Descendant: `element descendant`
* Child: `element > child`
* Next-sibling: `element + sibling`
* Subsequent-sibling: `element ~ sibling` (partial, only first sibling)

## Motivation

lxml-query addresses the maintenance gap of gumbo-query by providing a more reliable and feature-rich CSS selector engine based on libxml2. While gumbo-query's foundation on Gumbo was commendable, the lack of ongoing maintenance and the introduction of new bugs necessitated a more robust solution.

## Note

This project is actively developed and considered a work in progress. Contributions and feedback are welcome!