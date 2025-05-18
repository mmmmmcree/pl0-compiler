#include "Definition.h"
using namespace pl0;

const constants::KeywordSet constants::keyword_set = {
    "begin", "call", "const", "do", "end", "if", "else", "odd", "procedure", "read", "then", "var", "while", "write",
};

const constants::DelimiterSet constants::delimiter_set = { '(', ')', '[', ']', '.', ',', ';'};

const constants::OperatorSet constants::operator_set = {'+', '-', '*', '/', '=', '<', '>', ':', '#'};

const constants::TypeMap constants::type_map = {
    // keyword
    {"begin", TokenType::beginsym},
    {"call", TokenType::callsym},
    {"const", TokenType::constsym},
    {"do", TokenType::dosym},
    {"end", TokenType::endsym},
    {"if", TokenType::ifsym},
    {"else", TokenType::elsesym},
    {"odd", TokenType::oddsym}, 
    {"procedure", TokenType::proceduresym},
    {"read", TokenType::readsym},
    {"then", TokenType::thensym},
    {"var", TokenType::varsym},
    {"while", TokenType::whilesym},
    {"write", TokenType::writesym},
    // operator
    {"+", TokenType::plus},
    {"-", TokenType::minus},
    {"*", TokenType::times},
    {"/", TokenType::slash},
    {"=", TokenType::eql},
    {"<", TokenType::lss},
    {">", TokenType::gtr},
    {"<=", TokenType::leq},
    {">=", TokenType::geq},
    {":=", TokenType::becomes},
    {"#", TokenType::neq},
    // delimiter
    {"(", TokenType::lparen},
    {")", TokenType::rparen},
    {".", TokenType::period},
    {",", TokenType::comma},
    {";", TokenType::semicolon},
};

const constants::TokenTypeToStringMap constants::token_type_strings = {
    { TokenType::INVALID, "INVALID" },
    { TokenType::beginsym, "beginsym" },
    { TokenType::callsym, "callsym" },
    { TokenType::constsym, "constsym" },
    { TokenType::dosym, "dosym" },
    { TokenType::endsym, "endsym" },
    { TokenType::ifsym, "ifsym" },
    { TokenType::elsesym, "elsesym" },
    { TokenType::oddsym, "oddsym" },
    { TokenType::proceduresym, "proceduresym" },
    { TokenType::readsym, "readsym" },
    { TokenType::thensym, "thensym" },
    { TokenType::varsym, "varsym" },
    { TokenType::whilesym, "whilesym" },
    { TokenType::writesym, "writesym" },
    { TokenType::ident, "ident" },
    { TokenType::number, "number" },
    { TokenType::plus, "plus" },
    { TokenType::minus, "minus" },
    { TokenType::times, "times" },
    { TokenType::slash, "slash" },
    { TokenType::eql, "eql" },
    { TokenType::neq, "neq" },
    { TokenType::lss, "lss" },
    { TokenType::leq, "leq" },
    { TokenType::gtr, "gtr" },
    { TokenType::geq, "geq" },
    { TokenType::becomes, "becomes" },
    { TokenType::lparen, "lparen" },
    { TokenType::rparen, "rparen" },
    { TokenType::comma, "comma" },
    { TokenType::semicolon, "semicolon" },
    { TokenType::period, "period" }
};

const constants::IdentifierTypeMap constants::identifier_type_strings = {
    { IdentifierType::Const, "const" },
    { IdentifierType::Var, "var" },
    { IdentifierType::Procedure, "procedure" }
};

pl0::TokenType pl0::operator|(TokenType lhs, TokenType rhs)
{
    using T = std::underlying_type_t<TokenType>;
    return static_cast<TokenType>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

pl0::TokenType pl0::operator&(TokenType lhs, TokenType rhs)
{
    using T = std::underlying_type_t<TokenType>;
    return static_cast<TokenType>(static_cast<T>(lhs) & static_cast<T>(rhs));
}

pl0::IdentifierType pl0::operator|(IdentifierType lhs, IdentifierType rhs)
{
    using T = std::underlying_type_t<IdentifierType>;
    return static_cast<IdentifierType>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

pl0::IdentifierType pl0::operator&(IdentifierType lhs, IdentifierType rhs)
{
    using T = std::underlying_type_t<IdentifierType>;
    return static_cast<IdentifierType>(static_cast<T>(lhs) & static_cast<T>(rhs));
}
