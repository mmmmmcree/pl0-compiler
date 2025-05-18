#pragma once

#include "Token.h"
#include <functional>
#include "ErrorMessageManager.h"

namespace pl0 {
    class Lexer
    {
    public:
        Lexer() = default;
        operator bool() const;
        bool isValid() const;
        bool readFile(const char *path);
        TokenListView getTokenListView() const;
        const ErrorMessageManager &getErrorMessageManager() const;
    private:
        bool append(std::string_view text);
        int processToken(std::string_view text, int start, std::function<bool(char)> pred);
    private:
        Position m_current_pos;
        TokenList m_token_list;
        ErrorMessageManager m_err_manager{"Lexer"};    
    };
}