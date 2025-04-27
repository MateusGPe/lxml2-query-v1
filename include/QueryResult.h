#pragma once

#include "Selector.h"
#include "Node.h"

#include <set>
#include <string>
#include <optional>
#include <iterator>
#include <algorithm>
#include <vector>

#include <libxml/tree.h>
#include <libxml/HTMLparser.h>

class QueryResult
{
public:
	using NodeSet = std::set<xmlNodePtr>;

	class iterator
	{
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = Node;
		using difference_type = std::ptrdiff_t;

		iterator(NodeSet::iterator it) : iter_(it) {}

		iterator &operator++()
		{
			++iter_;
			return *this;
		}
		iterator operator++(int)
		{
			iterator inter = *this;
			++(*this);
			return inter;
		}

		Node operator*() const { return Node(*iter_); }

		bool operator==(const iterator &other) const { return iter_ == other.iter_; }
		bool operator!=(const iterator &other) const { return iter_ != other.iter_; }

		iterator &operator+=(difference_type n)
		{
			std::advance(iter_, n);
			return *this;
		}

		iterator operator+(difference_type n) const
		{
			iterator inter = *this;
			std::advance(inter.iter_, n);
			return inter;
		}

		difference_type operator-(const iterator &other) const { return std::distance(iter_, other.iter_); }

		Node operator[](difference_type n) const { return *(*this + n); }

	private:
		NodeSet::iterator iter_;
	};

	QueryResult(xmlNodePtr node);
	QueryResult(const NodeSet &nodes) : nodes_(nodes) {}
	~QueryResult() = default;

	iterator begin();
	iterator end();

	Node front() const { return *nodes_.cbegin(); }
	Node back() const { return *std::prev(nodes_.cend()); }
	bool empty() const { return nodes_.empty(); }

	NodeSet operator()()
	{
		return nodes_;
	}

	std::optional<Node> operator[](std::size_t index) const;
	QueryResult Find(std::string_view selector) const;
	QueryResult Find(const SelectorPtr &selector) const;
	static QueryResult Find(xmlNodePtr node, std::string_view selector);
	static QueryResult Find(xmlNodePtr node, const SelectorPtr &selector);
	std::optional<Node> at(std::size_t i) const;
	std::size_t size() const;

	std::vector<Node> toVector() const;

protected:
	NodeSet nodes_;
};