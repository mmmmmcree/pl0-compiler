#include "ErrorMessage.h"
#include <format>

std::ostream &pl0::operator<<(std::ostream &os, const ErrorMessage &message)
{
    auto [line, column] = message.getPosition();
    if (line == -1 and column == -1) {
        return os << message.getMessage();
    }
    return os << std::format("Error at line {}, column {}: {}", line, column, message.getMessage());
}

pl0::ErrorMessage::ErrorMessage(Position position, std::string_view message) :
    m_position(position), m_message(message)
{
}

pl0::ErrorMessage::ErrorMessage(std::string_view message) :
    m_position(-1, -1), m_message(message)
{
}

const pl0::Position &pl0::ErrorMessage::getPosition() const
{
    return m_position;
}

const std::string &pl0::ErrorMessage::getMessage() const
{
    return m_message;
}
