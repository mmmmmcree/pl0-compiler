#include "Lexer.h"
#include "tools.h"
#include <fstream>
#include <iostream>
#include <format>

pl0::Lexer::operator bool() const
{
    return this->isValid();
}

bool pl0::Lexer::isValid() const
{
    return not m_err_manager.hasError();
}

bool pl0::Lexer::readFile(const char *path)
{
    std::ifstream file(path);
    if (not file.is_open()) {
        m_err_manager.addErrorMessage(std::format("Cannot open file: {}", path));
        return false;
    }
	std::string str;
    bool success = true;
	while (std::getline(file, str)) {
        ++m_current_pos.first;
        success &= this->append(str);
    }
    return success;
}

pl0::TokenListView pl0::Lexer::getTokenListView() const
{
    return m_token_list;
}

const pl0::ErrorMessageManager &pl0::Lexer::getErrorMessageManager() const
{
    return m_err_manager;
}

bool pl0::Lexer::append(std::string_view text)
{
    for (int i = 0; i < text.size();) {
        m_current_pos.second = i;
        char c = text[i];
        if (isspace(c)) { ++i; continue; }
        if (is_delimiter(c)) { i = this->processToken(text, i, [](char) { return false; }); }
        else if (is_operator(c)) { i = this->processToken(text, i, is_operator); }
        else if (isalpha(c)) { i = this->processToken(text, i, isalnum); }
        else if (isdigit(c)) { i = this->processToken(text, i, isdigit); }
        else {
            m_err_manager.addErrorMessage(m_current_pos, std::format("Invalid character {}.", c));
            return false;
        } // error
    }
    return true;
}

int pl0::Lexer::processToken(std::string_view text, int start, std::function<bool(char)> pred)
{
    std::string token_value;
    int i = start;
    do {
        token_value.push_back(std::tolower(text[i++]));
    } while (i < text.size() and pred(text[i]));
    m_token_list.emplace_back(token_value, m_current_pos);
    if (not m_token_list.back()) {
        m_err_manager.addErrorMessage(m_current_pos, std::format("Invalid token: {}", token_value));
    }
    return i;
}
