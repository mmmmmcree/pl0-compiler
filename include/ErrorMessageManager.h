#pragma once

#include "ErrorMessage.h"

namespace pl0 {
    class ErrorMessageManager
    {
    public:
        ErrorMessageManager(std::string_view error_source);
        void addErrorMessage(const Position &position, std::string_view message);
        void addErrorMessage(std::string_view message);
        const std::string &getErrorSource() const;
        ErrorMessageListView getErrorMessageList() const;
        bool hasError() const;
    private:
        std::string m_error_source;
        ErrorMessageList m_error_list;
    };

    std::ostream &operator<<(std::ostream &os, const ErrorMessageManager &manager);
}