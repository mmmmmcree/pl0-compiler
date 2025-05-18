#include "ErrorMessageManager.h"
#include <format>

pl0::ErrorMessageManager::ErrorMessageManager(std::string_view error_source) :
    m_error_source(error_source)
{
}

void pl0::ErrorMessageManager::addErrorMessage(const Position &position, std::string_view message)
{
    std::string error_message = std::format("{}  {:->20}", message, m_error_source);
    m_error_list.emplace_back(position, error_message);
}

void pl0::ErrorMessageManager::addErrorMessage(std::string_view message)
{
    std::string error_message = std::format("{}  {:->20}", message, m_error_source);
    m_error_list.emplace_back(error_message);
}

const std::string &pl0::ErrorMessageManager::getErrorSource() const
{
    return m_error_source;
}

pl0::ErrorMessageListView pl0::ErrorMessageManager::getErrorMessageList() const
{
    return m_error_list;
}

bool pl0::ErrorMessageManager::hasError() const
{
    return not m_error_list.empty();
}

std::ostream &pl0::operator<<(std::ostream &os, const ErrorMessageManager &manager)
{
    if (not manager.hasError()) {
        os << std::format("No error found in {}", manager.getErrorSource());
    }
    for (const auto &error : manager.getErrorMessageList()) {
        os << error << std::endl;
    }
    return os;
}
