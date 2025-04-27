#include "AttributeSelector.h"
#include "Node.h"
#include <iomanip>
#include <iostream>

bool AttributeSelector::matches(xmlNodePtr node) const
{
	if (!node)
	{
		return false;
	}
	if (node->type != XML_ELEMENT_NODE)
	{
		return false;
	}

	xmlAttr *attr = node->properties;

	while (attr && attr->name && attr->children)
	{
		/*if (key_ == "data-中文")
		{
			std::cout << "[UTF-8]: v:'" << toLower(normalizeHtmlText((char *)attr->name)) << "' r:'" << key_ << "' " << std::endl;
		}*/
		if (key_ != toLower(normalizeHtmlText(reinterpret_cast<const char*>(attr->name))))
		{
			attr = attr->next;
			continue;
		}

		std::string value((GetPropNodeValue(node->doc, attr->children)));

		if (icase_)
		{
			std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c)
						   { return std::tolower(c); });
		}

		switch (operator_)
		{
		case Operator::Exists:
			return true;
		case Operator::Equals:
			return value_ == value;
		case Operator::Includes:
			return Includes(value, value_);
		case Operator::DashMatch:
			if (value_ == value)
			{
				return true;
			}
			if (value.size() < value_.size())
			{
				return false;
			}
			return value.substr(0, value_.size()) == value_ && value[value_.size()] == '-';
		case Operator::Prefix:
			return value.size() >= value_.size() && value.substr(0, value_.size()) == value_;
		case Operator::Suffix:
			// std::cout << "value: " << value << " value_: " << value_ << std::endl;
			return value.size() >= value_.size() && value.substr(value.size() - value_.size()) == value_;
		case Operator::SubString:
			return value.find(value_) != std::string::npos;
		case Operator::Regex:
			return std::regex_search(value, std::regex(value_));
		default:
			return false;
		}

		attr = attr->next;
	}
	return false;
}

std::string AttributeSelector::toString() const
{
	std::stringstream ss;

	ss << "[" << key_;
	switch (operator_)
	{
	case Operator::Exists:
		return ss.str() + "]";
	case Operator::Equals:
		if (key_ == "id")
		{
			return "#" + value_;
		}
		ss << "=";
		break;
	case Operator::Includes:
		if (key_ == "class")
		{
			return "." + value_;
		}
		ss << "~=";
		break;
	case Operator::DashMatch:
		ss << "|=";
		break;
	case Operator::Prefix:
		ss << "^=";
		break;
	case Operator::Suffix:
		ss << "$=";
		break;
	case Operator::SubString:
		ss << "*=";
		break;
	case Operator::Regex:
		ss << "#=";
		break;
	default:
		break;
	}
	ss << std::quoted(value_);
	if (!icase_)
	{
		ss << "s";
	}
	return ss.str() + "]";
}
