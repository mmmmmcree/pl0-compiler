#include "Operand.h"
#include <format>

pl0::Operand::Operand(const Token &token)
{
    if (token.is(TokenType::number)) {
        m_value = std::stoi(token.getValue());
    }
    if (token.is(TokenType::ident)) {
        m_name = token.getValue();
    }
}

pl0::Operand::operator bool() const
{
    return this->isValid();
}

pl0::Operand &pl0::Operand::operator=(const Operand &other)
{
    m_is_temporary = other.m_is_temporary;
    m_value = other.m_value;
    m_name = other.m_name;
    return *this;
}

bool pl0::Operand::operator==(const Operand &other) const
{
    return m_value == other.m_value and m_name == other.m_name and m_is_temporary == other.m_is_temporary;
}

pl0::Operand & pl0::Operand::setName(std::string_view name)
{
    m_name = name;
    return *this;
}

pl0::Operand & pl0::Operand::setValue(int value)
{
    m_value = value;
    return *this;
}

pl0::Operand & pl0::Operand::setTemporary(bool is_temporary)
{
    m_is_temporary = is_temporary;
    return *this;
}

bool pl0::Operand::isValid() const
{
    return m_value != std::numeric_limits<int>::max() or not m_name.empty();
}

bool pl0::Operand::isTemporary() const
{
    return m_is_temporary;
}

bool pl0::Operand::isIdentifier() const
{
    return not m_name.empty();
}

bool pl0::Operand::isLiteral() const
{
    return m_name.empty() and not this->isTemporary();
}

const std::string &pl0::Operand::getName() const
{
    return m_name;
}

int pl0::Operand::getValue() const
{
    return m_value;
}

std::ostream &pl0::operator<<(std::ostream &os, const Operand &operand)
{
    if (operand.isIdentifier()) {
        return os << std::format("{}({})", operand.getName(), operand.isTemporary() ? "temporary" : "permanent");
    }
    if (operand) {
        return os << std::format("{}({})", operand.getValue(), "literal");
    }
    return os << "null";
}
