#pragma once

#include "Token.h"
#include <iostream>

namespace pl0 {
    class Operator
    {
    public:
        enum Type
        {
            None,
            ConstDeclare,
            VarDeclare,
            ProcedureDeclare,
            Add,
            Subtract,
            Multiply,
            Divide,
            UnaryMinus,
            Odd,
            Greater,
            Less,
            GreaterEqual,
            LessEqual,
            Equal,
            NotEqual,
            Assign,
            Jump,
            JumpIfZero,
            Read,
            Write,
            Label,
        };
        Operator() = default;
        Operator(Type type);
        Operator(const Token &token);
        operator bool() const;
        bool operator==(const Operator &other) const = default;
        Type getType() const;
        std::string_view getTypeAsString() const;
        bool is(Type type) const;
    private:
        Type m_type = None;
    };

    std::ostream &operator<<(std::ostream &os, const Operator &op);
}