#pragma once

#include <string>
#include <string_view>
#include "Definition.h"


namespace pl0 {
    bool is_keyword(const std::string &str);
    bool is_operator(char c);
    bool is_delimiter(char c);
}