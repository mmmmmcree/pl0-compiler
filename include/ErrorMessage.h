#pragma once

#include "Definition.h"
#include <iostream>

namespace pl0 {
    class ErrorMessage
    {
    public:
        ErrorMessage(Position position, std::string_view message);
        ErrorMessage(std::string_view message);
        const Position &getPosition() const;
        const std::string &getMessage() const;
    private:
        Position m_position;
        std::string m_message;
    };

    using ErrorMessageList = List<ErrorMessage>;
    using ErrorMessageListView = ListView<ErrorMessage>;

    std::ostream& operator<<(std::ostream& os, const ErrorMessage& message);

}