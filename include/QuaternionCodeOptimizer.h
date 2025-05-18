#pragma once

#include "QuaternionCode.h"
#include <unordered_map>
#include <stack>

namespace pl0 {
    class QuaternionCodeOptimizer
    {
        using ConstantMap = std::unordered_map<std::string, int>;
        using EquivalenceMap = std::unordered_map<std::string, Operand>;
        using DependentResultIndexMap = std::unordered_map<std::string, std::stack<int>>;
    public:
        QuaternionCodeOptimizer(QuaternionCodeListView code_list);
        const QuaternionCodeList &getOptimizedResult() const;
    private:
        void optimize();
        void evaluateConstantExpression(QuaternionCode& code);
        bool isOperandConstant(const Operand& operand) const;
        int getConstantValue(const Operand& operand) const;
        void updateEqualExpression();
        void judgeDependent();
        void setDependent(const Operand &result);
        void eliminateRedundant();
    private:
        ConstantMap m_constant_map;
        EquivalenceMap m_equivalence_map;
        DependentResultIndexMap m_dependent_result_index_map;
        std::string m_dependent;
        QuaternionCodeList m_code_list;
    };
}