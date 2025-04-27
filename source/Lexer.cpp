#include "Lexer.h"
#include <stdexcept>
#include <array>

#define error(message) std::runtime_error((std::string((message)) + " -  at: " + std::to_string(offset_)))

// namespace
// {
//     template <size_t Start, size_t End, size_t Diff, size_t Size, typename T>
//     constexpr void fillArrayRange(std::array<T, Size> &arr)
//     {
//         for (size_t i = Start; i < End; ++i)
//             arr[i] = (Diff == 1) ? -1 : static_cast<T>(i - Diff);
//     }
//     template <size_t S>
//     constexpr std::array<char, S> generateHexLookupTable()
//     {
//         std::array<char, S> lut{};
//         fillArrayRange<0, 10, 0, S>(lut);
//         fillArrayRange<10, 17, 1, S>(lut);
//         fillArrayRange<17, 23, 7, S>(lut);
//         fillArrayRange<23, 49, 1, S>(lut);
//         fillArrayRange<49, 55, 39, S>(lut);
//         return lut;
//     }
//     constexpr auto hex_lut = generateHexLookupTable<55>();
// }

char SelectorLexer::CurrentChar() const
{
    return input_[offset_];
}

bool SelectorLexer::HasRemaining() const
{
    return offset_ < length_;
}

bool SelectorLexer::EndOfFile() const
{
    return offset_ >= length_;
}

std::string_view SelectorLexer::PeekSubstring(size_t size)
{
    if (offset_ + size >= length_)
    {
        if (offset_ >= length_)
        {
            return "";
        }
        size = length_ - offset_;
    }
    return input_.substr(offset_, size);
}

void SelectorLexer::Advance(int i)
{
    long long offset = static_cast<long long>(offset_) + i;
    if (size_t(offset) >= length_)
    {
        offset_ = length_;
        return;
    }
    offset_ = size_t(offset);
}

char SelectorLexer::ConsumeChar()
{
    // std::cerr << "(" << offset_ << ", '" << input_[offset_] << "') ";
    if (offset_ >= length_)
    {
        return '\0';
    }

    char ch = input_[offset_];

    offset_++;

    return ch;
}

char SelectorLexer::PeekChar() const
{
    if (offset_ >= length_)
    {
        return '\0';
    }
    return input_[offset_];
}

bool SelectorLexer::SkipSpaceCmts()
{
    const std::size_t startOffset = offset_;

    while (offset_ < length_)
    {
        char ch_ = input_[offset_];
        if (std::isspace(ch_))
        {
            offset_++;
        }
        else if (ch_ == '/' && length_ > (offset_ + 1) && input_[offset_ + 1] == '*')
        {
            std::size_t pos = input_.find("*/", offset_ + 2);
            if (pos == std::string::npos)
            {
                offset_ = length_;
                break; // Unclosed comment
            }
            offset_ = pos + 2;
            ch_ = input_[offset_];
        }
        else
        {
            break;
        }
    }
    return offset_ > startOffset;
}

int SelectorLexer::ReadInteger()
{
    std::size_t offset;
    int i;

    try
    {
        i = std::stoi(input_.data() + offset_, &offset);
    }
    catch (const std::invalid_argument &)
    {
        throw error("expected integer, but didn't find it.");
    }
    catch (const std::out_of_range &)
    {
        throw error("integer out of range.");
    }

    if (offset == 0)
    {
        throw error("expected integer, but didn't find it.");
    }
    offset_ += offset;

    return i;
}

std::string SelectorLexer::ReadString()
{
    const std::size_t length = input_.size();
    std::size_t offset = offset_;
    if (length < offset + 2)
    {
        throw error("expected string, found EOF instead");
    }

    char quote = input_[offset];
    offset++;

    std::string _string;

    while (offset < length)
    {
        char c = input_[offset];
        if (c == '\\')
        {
            if (length > offset + 1)
            {
                c = input_[offset + 1];
                if (c == '\r')
                {
                    if (length > offset + 2 && input_[offset + 2] == '\n')
                    {
                        offset += 3;
                        continue;
                    }
                }

                if (c == '\r' || c == '\n' || c == '\f')
                {
                    offset += 2;
                    continue;
                }
            }
            offset_ = offset;
            _string += ReadEscapeSequence();
            offset = offset_;
        }
        else if (c == quote)
        {
            break;
        }
        else if (c == '\r' || c == '\n' || c == '\f')
        {
            throw error("unexpected end of line in string");
        }
        else
        {
            std::size_t start = offset;
            while (offset < input_.size())
            {
                c = input_[offset];
                if (c == quote || c == '\\' || c == '\r' || c == '\n' || c == '\f')
                {
                    break;
                }
                offset++;
            }
            _string += input_.substr(start, offset - start);
        }
    }

    if (offset >= length)
    {
        throw error("EOF in string");
    }

    offset++;
    offset_ = offset;

    return _string;
}

std::string SelectorLexer::ReadName()
{
    const std::size_t length = input_.size();
    std::size_t offset = offset_;
    std::string name;
    while (offset < length)
    {
        char c = input_[offset];

        if (IsIdentifierChar(c))
        {
            std::size_t start = offset;
            while (offset < length && IsIdentifierChar(input_[offset]))
            {
                offset++;
            }
            name += input_.substr(start, offset - start);
            continue;
        }

        if (c == '\\')
        {
            offset_ = offset;
            name += ReadEscapeSequence();
            offset = offset_;
            continue;
        }

        break;
    }

    if (name == "")
    {
        throw error("expected name, found EOF instead");
    }

    offset_ = offset;
    return name;
}

std::string SelectorLexer::ReadRegex()
{
    std::string rgex;

    while (offset_ < input_.size())
    {
        if (input_[offset_] == '\\')
        {
            offset_++;
            if (input_[offset_] == '/')
            {
                rgex += input_[offset_];
                offset_++;
                continue;
            }
            rgex += '\\';
            continue;
        }
        if (input_[offset_] == '/')
        {
            break;
        }
        rgex += input_[offset_];
        offset_++;
    }

    if (offset_ > input_.size())
    {
        throw error("expected regex, found EOF instead");
    }

    if (input_[offset_] != '/')
    {
        throw error("expected regex end (/), found invalid char");
    }
    offset_++;
    return rgex;
}

std::string SelectorLexer::ReadIdentifier()
{
    bool startingDash = false;
    if (input_.size() > offset_ && input_[offset_] == '-')
    {
        startingDash = true;
        offset_++;
    }

    if (input_.size() <= offset_)
    {
        throw error("expected identifier, found EOF instead");
    }

    char c = input_[offset_];
    if (!IsIdentifierStartChar(c) && c != '\\')
    {
        throw error("expected identifier, found invalid char");
    }

    std::string name = ReadName();
    if (startingDash)
    {
        name = "-" + name;
    }

    return name;
}

bool SelectorLexer::IsIdentifierChar(char c)
{
    return std::isalnum(c) || (c == '_') || (c == '-') || ((unsigned)c > 0x7F);
}

bool SelectorLexer::IsIdentifierStartChar(char c)
{
    return std::isalpha(c) || (c == '_') || ((unsigned)c > 0x7F);
}

int SelectorLexer::HexDigitValue(char c)
{
    c = static_cast<char>(std::tolower(c));

    if (c >= 'a' && c <= 'f')
    {
        return c - 'a' + 10;
    }
    if (c >= '0' && c <= '9')
    {
        return c - '0';
    }
    return -1;
}
// #include <iostream>
/*std::string SelectorLexer::ReadEscapeSequence()
{
    if (input_.size() < offset_ + 2 || input_[offset_] != '\\')
    {
        throw error("invalid escape sequence");
    }

    std::size_t start = offset_ + 1;
    char c = input_[start];
    if (c == '\r' || c == '\n' || c == '\f')
    {
        throw error("escaped line ending outside string");
    }

    int d = HexDigitValue(c);

    if (d < 0)
    {
        offset_ += 2;
        return std::string(input_).substr(start, 1);
    }

    std::size_t i = 0;
    std::string sequence;
    c = 0;

    for (i = start; i < offset_ + 7 && i < input_.size() && (d >= 0); i++)
    {
        unsigned int d2(((unsigned int)d) & 0xf);

        d = HexDigitValue(input_[i + 1]);

        if ((i - start) % 2)
        {
            c += d2;
            sequence.push_back(c);
            c = 0;
            continue;
        }

        c += (d2 << 4);
    }

    if (sequence.size() == 0 || c != 0)
    {
        throw error("invalid hex digit");
    }

    if (input_.size() > i)
    {
        switch (input_[i])
        {
        case '\r':
            i++;
            if (input_.size() > i && input_[i] == '\n')
            {
                i++;
            }
            break;
        case ' ':
        case '\t':
        case '\n':
        case '\f':
            i++;
            break;
        }
    }

    offset_ = i;

    std::cerr << "Escape sequence: " << sequence << std::endl;
    return sequence;
}*/

std::string SelectorLexer::ReadEscapeSequence()
{
    if (input_.size() < offset_ + 2 || input_[offset_] != '\\')
    {
        throw error("invalid escape sequence");
    }

    offset_++;

    char c = input_[offset_];
    if (c == '\r' || c == '\n' || c == '\f')
    {
        throw error("escaped line ending outside string");
    }

    int d = HexDigitValue(c);

    if (d < 0)
    {
        offset_++;
        return std::string(1, c);
    }

    std::string sequence;
    unsigned int codePoint = 0;
    std::size_t hexDigits = 0;

    for (std::size_t i = offset_; i < offset_ + 6 && i < input_.size(); i++)
    {
        int hexValue = HexDigitValue(input_[i]);
        if (hexValue < 0)
        {
            break;
        }
        codePoint = (codePoint << 4) | static_cast<unsigned>(hexValue);
        hexDigits++;
        offset_++;
    }

    if (hexDigits == 0)
    {
        throw error("invalid hex escape sequence");
    }

    if (offset_ < input_.size())
    {
        char nextChar = input_[offset_];
        if (nextChar == ' ' || nextChar == '\t' || nextChar == '\n' || nextChar == '\f' || nextChar == '\r')
        {
            offset_++;
            if (nextChar == '\r' && offset_ < input_.size() && input_[offset_] == '\n')
            {
                offset_++;
            }
        }
    }

    if (codePoint <= 0x7F)
    {
        sequence.push_back(static_cast<char>(codePoint));
    }
    else if (codePoint <= 0x7FF)
    {
        sequence.push_back(static_cast<char>(0xC0 | ((codePoint >> 6) & 0x1F)));
        sequence.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }
    else if (codePoint <= 0xFFFF)
    {
        sequence.push_back(static_cast<char>(0xE0 | ((codePoint >> 12) & 0x0F)));
        sequence.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        sequence.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }
    else if (codePoint <= 0x10FFFF)
    {
        sequence.push_back(static_cast<char>(0xF0 | ((codePoint >> 18) & 0x07)));
        sequence.push_back(static_cast<char>(0x80 | ((codePoint >> 12) & 0x3F)));
        sequence.push_back(static_cast<char>(0x80 | ((codePoint >> 6) & 0x3F)));
        sequence.push_back(static_cast<char>(0x80 | (codePoint & 0x3F)));
    }
    else
    {
        throw error("invalid Unicode code point");
    }

    return sequence;
}

size_t SelectorLexer::GetOffset() const
{
    return offset_;
}

#undef error