#include "QuaternionCode.h"
#include <format>

pl0::QuaternionCode &pl0::QuaternionCode::clear()
{
    m_operator = {};
    m_lhs_operand = {};
    m_rhs_operand = {};
    m_result = {};
    return *this;
}

pl0::QuaternionCode &pl0::QuaternionCode::clearExpression()
{
    m_operator = {};
    m_lhs_operand = {};
    m_rhs_operand = {};
    return *this;
}

pl0::QuaternionCode &pl0::QuaternionCode::setOperator(const Operator &op)
{
    m_operator = op;
    return *this;
}

pl0::QuaternionCode &pl0::QuaternionCode::setLhsOperand(const Operand &operand)
{
    m_lhs_operand = operand;
    return *this;
}

pl0::QuaternionCode &pl0::QuaternionCode::setRhsOperand(const Operand &operand)
{
    m_rhs_operand = operand;
    return *this;
}

pl0::QuaternionCode &pl0::QuaternionCode::setResult(const Operand &operand)
{
    m_result = operand;
    return *this;
}

const pl0::Operator &pl0::QuaternionCode::getOperator() const
{
    return m_operator;
}

const pl0::Operand & pl0::QuaternionCode::getLhsOperand() const
{
    return m_lhs_operand;
}

const pl0::Operand &pl0::QuaternionCode::getRhsOperand() const
{
    return m_rhs_operand;
}

const pl0::Operand &pl0::QuaternionCode::getResult() const
{
    return m_result;
}

bool pl0::QuaternionCode::isExpressionEqual(const QuaternionCode &other) const
{
    return m_operator == other.m_operator and m_lhs_operand == other.m_lhs_operand and m_rhs_operand == other.m_rhs_operand;
}

std::ostream &pl0::operator<<(std::ostream &os, const QuaternionCode &code)
{
    return os << '(' << code.getOperator() << ", " << code.getLhsOperand() << ", " << code.getRhsOperand() << ", " << code.getResult() << ')';
}
