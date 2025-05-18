#pragma once
#include "Definition.h"
#include <string>
#include <iostream>

namespace pl0 {
    class Token
    {
    public:
        Token(std::string_view value, const Position &position = {-1, -1});
        Token(const Token &other) = default;
        Token &operator=(const Token &other) = default;
        ~Token() = default;
        operator bool() const;
        bool isValid() const;
        TokenType getType() const;
        const std::string& getValue() const;
        bool is(TokenType type) const;
        std::string getTypeAsString() const; // for debugging purposes
        const Position &getPosition() const;
    private:
        TokenType m_type;
        std::string m_value;
        Position m_position;
    };

    using TokenList = List<Token>;
    using TokenListView = ListView<Token>;

    std::ostream &operator<<(std::ostream &os, const Token &tokens);
}

