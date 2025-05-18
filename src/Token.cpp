#include "Token.h"
#include "tools.h"
#include <format>

pl0::Token::Token(std::string_view value, const Position &position) :
    m_value(value),
    m_position(position)
{
    if (value.empty()) {
        m_type = TokenType::INVALID;
        return;
    }
    if (constants::type_map.contains(m_value)) {
        m_type = constants::type_map.at(m_value);
        return;
    }
    char first = m_value.front();
    if (isdigit(first)) { m_type = TokenType::number; }
    else if (isalpha(first)) { m_type = TokenType::ident; }
    else {
        m_type = TokenType::INVALID;
    }
}

pl0::Token::operator bool() const
{
    return this->isValid();
}

bool pl0::Token::isValid() const
{
    return m_type != TokenType::INVALID;
}

pl0::TokenType pl0::Token::getType() const
{
    return m_type;
}

const std::string &pl0::Token::getValue() const
{
    return m_value;
}

bool pl0::Token::is(TokenType type) const
{
    return static_cast<bool>(m_type & type);
}

std::string pl0::Token::getTypeAsString() const
{
    return constants::token_type_strings.at(this->getType());
}

const pl0::Position &pl0::Token::getPosition() const
{
    return m_position;
}

std::ostream &pl0::operator<<(std::ostream &os, const Token &token)
{
    auto [line, column] = token.getPosition();
    return os << std::format("Token at ({},{}) {} {}", line, column, token.getTypeAsString(), token.getValue());
}
