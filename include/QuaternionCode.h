#pragma once
#include <iostream>
#include "Operand.h"
#include "Operator.h"

namespace pl0 {
    class QuaternionCode
    {
    public:
        QuaternionCode() = default;
        QuaternionCode &clear();
        QuaternionCode &clearExpression();
        QuaternionCode &setOperator(const Operator &op);
        QuaternionCode &setLhsOperand(const Operand &operand);
        QuaternionCode &setRhsOperand(const Operand &operand);
        QuaternionCode &setResult(const Operand &operand);
        const Operator &getOperator() const;
        const Operand &getLhsOperand() const;
        const Operand &getRhsOperand() const;
        const Operand &getResult() const;
        template<std::size_t N> auto get() const;
        bool isExpressionEqual(const QuaternionCode &other) const;
    private:
        Operator m_operator;
        Operand m_lhs_operand;
        Operand m_rhs_operand;
        Operand m_result;
    };

    using QuaternionCodeList = std::vector<QuaternionCode>;
    using QuaternionCodeListView = std::span<const QuaternionCode>;

    std::ostream &operator<<(std::ostream &os, const QuaternionCode &code);
}

template<std::size_t N>
auto pl0::QuaternionCode::get() const
{
    if constexpr (N == 0) return m_operator;
    else if constexpr (N == 1) return m_lhs_operand;
    else if constexpr (N == 2) return m_rhs_operand;
    else if constexpr (N == 3) return m_result;
}

namespace std {
    template<> struct tuple_size<pl0::QuaternionCode> : std::integral_constant<std::size_t, 4> {};
    template<std::size_t N> struct tuple_element<N, pl0::QuaternionCode> { using type = pl0::Operand; };
    template <> struct tuple_element<0, pl0::QuaternionCode> { using type = pl0::Operator; };
}