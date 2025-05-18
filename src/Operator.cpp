#include "Operator.h"
#include <unordered_map>

pl0::Operator::Operator(Type type) : m_type(type)
{
}

pl0::Operator::Operator(const Token &token)
{
    switch (token.getType()) {
        case TokenType::constsym : { m_type = ConstDeclare; } break;
        case TokenType::varsym : { m_type = VarDeclare; } break;
        case TokenType::proceduresym : { m_type = ProcedureDeclare; } break;
        case TokenType::plus : { m_type = Add; } break;
        case TokenType::minus : { m_type = Subtract; } break;
        case TokenType::times : { m_type = Multiply; } break;
        case TokenType::slash : { m_type = Divide; } break;
        case TokenType::oddsym : { m_type = Odd; } break;
        case TokenType::gtr : { m_type = Greater; } break;
        case TokenType::lss : { m_type = Less; } break;
        case TokenType::geq : { m_type = GreaterEqual; } break;
        case TokenType::leq : { m_type = LessEqual; } break;
        case TokenType::eql : { m_type = Equal; } break;
        case TokenType::neq : { m_type = NotEqual; } break;
        case TokenType::becomes : { m_type = Assign; } break;
        case TokenType::ifsym : { m_type = Jump; } break;
        case TokenType::readsym : { m_type = Read; } break;
        case TokenType::writesym : { m_type = Write; } break;
    }
}

pl0::Operator::operator bool() const
{
    return not this->is(None);
}

pl0::Operator::Type pl0::Operator::getType() const
{
    return m_type;
}

std::string_view pl0::Operator::getTypeAsString() const
{
    static const std::unordered_map<Type, std::string> s_type_to_string_map {
        {None, "None"}, 
        {ConstDeclare, "ConstDeclare"}, {VarDeclare, "VarDeclare"}, {ProcedureDeclare, "ProcedureDeclare"},
        {Add, "Add"}, {Subtract, "Substract"}, {Multiply, "Multiply"}, {Divide, "Divide"},
        {UnaryMinus, "UnaryMinus"}, {Odd, "Odd"},
        {Greater, "Greater"}, {Less, "Less"}, {GreaterEqual, "GreaterEqual"}, {LessEqual, "LessEqual"}, {Equal, "Equal"},
        {Assign, "Assign"},
        {Jump, "Jump"}, {JumpIfZero, "JumpIfZero"},
        {Read, "Read"}, {Write, "Write"}, 
        {Label, "Label"}
    };
    return s_type_to_string_map.at(m_type);
}

bool pl0::Operator::is(Type type) const
{
    return m_type == type;
}

std::ostream &pl0::operator<<(std::ostream &os, const Operator &op)
{
    return os << op.getTypeAsString();
}
