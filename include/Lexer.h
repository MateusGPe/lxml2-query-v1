#pragma once

#include <string>
#include <string_view>

class SelectorLexer
{
public:
    SelectorLexer() = delete;
    SelectorLexer(std::string_view input) : input_(input), length_(input.length()), offset_(0) {}

    char CurrentChar() const;
    bool HasRemaining() const;
    bool EndOfFile() const;
    std::string_view PeekSubstring(size_t size);
    void Advance(int i);
    size_t GetOffset() const;
    char ConsumeChar();
    char PeekChar() const;

    template <typename... Args>
    bool TryMatch(Args... args);

    template <typename T, typename... Args>
    bool IsAnyOf(T value, Args... args) const;

    template <typename T, typename... Args>
    bool IsNotAnyOf(T value, Args... args) const;

    bool SkipSpaceCmts();
    int ReadInteger();
    std::string ReadString();
    std::string ReadName();
    std::string ReadRegex();
    std::string ReadIdentifier();

    template <bool skip_after>
    bool TryConsumeSpaced(const char c);

    bool IsIdentifierChar(char c);
    bool IsIdentifierStartChar(char c);
    int HexDigitValue(char c);
    std::string ReadEscapeSequence();

protected:
    const std::string_view input_;
    const std::size_t length_;
    std::size_t offset_;
};

template <typename... Args>
inline bool SelectorLexer::TryMatch(Args... args)
{
    if (IsAnyOf(args...))
    {
        offset_++;
        return true;
    }
    return false;
}

template <typename T, typename... Args>
inline bool SelectorLexer::IsAnyOf(T value, Args... args) const
{
    if constexpr (sizeof...(args) > 0)
    {
        return value == input_[offset_] || IsAnyOf(args...);
    }
    return value == input_[offset_];
}

template <typename T, typename... Args>
inline bool SelectorLexer::IsNotAnyOf(T value, Args... args) const
{
    if constexpr (sizeof...(args) > 0)
    {
        return value != input_[offset_] && IsNotAnyOf(args...);
    }
    return value != input_[offset_];
}

template <bool skip_after>
inline bool SelectorLexer::TryConsumeSpaced(const char c)
{
    size_t offset = offset_;
    SkipSpaceCmts();
    if (offset_ >= length_ || input_[offset_] != c)
    {
        offset_ = offset;
        return false;
    }
    offset_++;
    if constexpr (skip_after)
    {
        SkipSpaceCmts();
    }
    return true;
}
