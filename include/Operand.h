#pragma once
#include <string>
#include "Token.h"
#include <limits>
#include <iostream>

namespace pl0 {
    class Operand
    {
    public:
        Operand() = default;
        Operand(const Token &token);
        operator bool() const;
        Operand &operator=(const Operand &other);
        bool operator==(const Operand &other) const;
        Operand & setName(std::string_view name);
        Operand & setValue(int value);
        Operand & setTemporary(bool is_temporary);
        bool isValid() const;
        bool isTemporary() const;
        bool isIdentifier() const;
        bool isLiteral() const;
        const std::string &getName() const;
        int getValue() const;
    private:
        bool m_is_temporary = false;
        std::string m_name;
        int m_value = std::numeric_limits<int>::max();
    };

    struct OperandHashByName
    {
        size_t operator()(const Operand &operand) const
        {
            return std::hash<std::string>{}(operand.getName());
        }
    };

    struct OperandEqualByName
    {
        bool operator()(const Operand &lhs, const Operand &rhs) const
        {
            return lhs.getName() == rhs.getName();
        }
    };

    std::ostream &operator<<(std::ostream &os, const Operand &operand);
}