#include "Identifier.h"


pl0::Identifier::Identifier(IdentifierType type, std::string_view name) :
    m_type(type),
    m_name(name),
    m_value(0)
{
}

bool pl0::Identifier::is(IdentifierType type) const
{
    return static_cast<bool>(m_type & type);
}

const std::string &pl0::Identifier::getName() const
{
    return m_name;
}

int pl0::Identifier::getValue() const
{
    return m_value;
}

bool pl0::Identifier::initialize(int value)
{
    if (this->is(IdentifierType::Procedure)) { return false; }
    m_value = value;
    return true;
}

bool pl0::Identifier::initialize(std::string_view value)
{
    if (this->is(IdentifierType::Procedure)) { return false; }
    for (char c : value) {
        if (not std::isdigit(c)) { return false; }
    }
    int val = std::stoi(std::string(value));
    m_value = val;
    return true;
}

bool pl0::Identifier::assign(int value)
{
    if (not this->is(IdentifierType::Var)) { return false; }
    m_value = value;
    return true;
}

bool pl0::Identifier::assign(std::string_view value)
{
    if (not this->is(IdentifierType::Var)) { return false; }
    for (char c : value) {
        if (not std::isdigit(c)) { return false; }
    }
    int val = std::stoi(std::string(value));
    m_value = val;
    return true;
}
