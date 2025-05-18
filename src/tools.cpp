#include "tools.h"

bool pl0::is_keyword(const std::string &str)
{
    return constants::keyword_set.contains(str);
}

bool pl0::is_operator(char c)
{
    return constants::operator_set.contains(c);
}

bool pl0::is_delimiter(char c)
{
    return constants::delimiter_set.contains(c);
}