#pragma once

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <span>
#include <functional>

namespace pl0 {
    enum class TokenType : int64_t
    {
        INVALID = 0, 
        plus = 1,
        minus = 1 << 1,
        SignOperator = plus | minus,
        times = 1 << 2,
        slash = 1 << 3,
        eql = 1 << 4,
        neq = 1 << 5,
        lss = 1 << 6,
        leq = 1 << 7,
        gtr = 1 << 8,
        geq = 1 << 9,
        becomes = 1 << 10,
        UnaryOperator = SignOperator,
        BinaryRelationOperator = eql | neq | lss | leq | gtr | geq,
        Operator = plus | minus | times | slash | eql | neq | lss | leq | gtr | geq | becomes,
        lparen = 1 << 11,
        rparen = 1 << 12,
        comma = 1 << 13,
        semicolon = 1 << 14,
        period = 1 << 15,
        delimiters = lparen | rparen | comma | semicolon | period,
        // keywords
        beginsym = 1 << 16,
        callsym = 1 << 17,
        constsym = 1 << 18,
        dosym = 1 << 19,
        endsym = 1 << 20,
        ifsym = 1 << 21,
        elsesym = 1 << 22,
        oddsym = 1 << 23,
        proceduresym = 1 << 24,
        readsym = 1 << 25,
        thensym = 1 << 26,
        varsym = 1 << 27,
        whilesym = 1 << 28,
        writesym = 1 << 29,
        Keyword = beginsym | callsym | constsym | dosym | endsym | ifsym | elsesym | oddsym | proceduresym | readsym | thensym | varsym | whilesym | writesym,
        number = 1 << 30, // number literal
        ident = 1 << 31, // identifier
    };
    TokenType operator|(TokenType lhs, TokenType rhs);
    TokenType operator&(TokenType lhs, TokenType rhs);

    enum class IdentifierType
    {
        Const = 1,
        Var = 1 << 1,
        Procedure = 1 << 2,
    };
    IdentifierType operator|(IdentifierType lhs, IdentifierType rhs);
    IdentifierType operator&(IdentifierType lhs, IdentifierType rhs);

}

namespace pl0::constants {
    using KeywordSet = std::unordered_set<std::string>;

    using DelimiterSet = std::unordered_set<char>;

    using OperatorSet = std::unordered_set<char>;

    using CodeMap = std::vector<std::unordered_map<std::string, TokenType>>;

    using TokenTypeToStringMap = std::unordered_map<TokenType, std::string>;

    using IdentifierTypeMap = std::unordered_map<IdentifierType, std::string>;

    using TypeMap = std::unordered_map<std::string, TokenType>;

    extern const TypeMap type_map;

    extern const TokenTypeToStringMap token_type_strings;

    extern const IdentifierTypeMap identifier_type_strings;

    extern const KeywordSet keyword_set;

    extern const DelimiterSet delimiter_set;

    extern const OperatorSet operator_set;
}

//common types
namespace pl0 {
    using Position = std::pair<int, int>;
    template <typename T> using List = std::vector<T>;
    template <typename T> using ListView = std::span<const std::decay_t<T>>;
    using PaserTable = std::unordered_map<TokenType, std::function<void()>>;
}