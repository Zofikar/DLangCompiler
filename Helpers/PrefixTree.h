#include <optional>
#include <unordered_map>
#include "LexerTypes.h"

struct PrefixTree
{
    std::optional<TokenType> oType{std::nullopt};
    std::unordered_map<char, PrefixTree> children{};

    void insert(std::string_view const& str, TokenType type)
    {
        if (str.empty())
        {
            oType = type;
            return;
        }
        auto it = children.find(str.front());
        if (it == children.end())
        {
            children[str.front()] = PrefixTree{};
        }
        children[str.front()].insert(str.substr(1), type);
    }

    template<size_t N>
    void insert(std::array<std::string_view, N> const& strs, TokenType type)
    {
        for(auto const& str : strs)
        {
            insert(str, type);
        }
    }

    std::optional<PrefixTree> find(std::string_view const& str) const
    {
        if (str.empty())
        {
            return *this;
        }
        return find(str.front())->find(str.substr(1));
    }

    std::optional<PrefixTree> find(const char c) const
    {
        auto it = children.find(c);
        if (it == children.end())
        {
            return std::nullopt;
        }
        return it->second;
    }

    bool isLeaf() const
    {
        return children.empty();
    }
};