#include "QuaternionCodeOptimizer.h"
#include <unordered_set>

pl0::QuaternionCodeOptimizer::QuaternionCodeOptimizer(QuaternionCodeListView code_list) :
    m_code_list(code_list.begin(), code_list.end()),
    m_dependent(code_list.size(), false)
{
    this->optimize();
}

const pl0::QuaternionCodeList &pl0::QuaternionCodeOptimizer::getOptimizedResult() const
{
    return m_code_list;
}

void pl0::QuaternionCodeOptimizer::optimize()
{
    for (int i = 0; i < m_code_list.size(); ++i) {
        m_dependent_result_index_map[m_code_list[i].getResult().getName()].push(i);
    }
    for (auto &code : m_code_list) {
        this->evaluateConstantExpression(code);
    }
    this->updateEqualExpression();
    this->judgeDependent();
    this->eliminateRedundant();
    m_dependent.back() = true; // 保证至少有一个语句是有效的
    QuaternionCodeList optimized_code_list;
    for (int i = 0; i < m_code_list.size(); ++i) {
        if (m_dependent[i]) {
            optimized_code_list.push_back(m_code_list[i]);
        }
    }
    m_code_list = optimized_code_list;
}

void pl0::QuaternionCodeOptimizer::evaluateConstantExpression(QuaternionCode &code)
{
    auto [op, arg1, arg2, result] = code;
    bool is_arg1_constant = this->isOperandConstant(arg1);
    bool is_arg2_constant = this->isOperandConstant(arg2);
    Operand constant_operand;
    if (is_arg1_constant and not arg2) {
        int result_value = this->getConstantValue(arg1);
        constant_operand.setValue(result_value);
        code.clearExpression().setOperator(Operator::Assign).setLhsOperand(constant_operand);
        m_constant_map[result.getName()] = result_value;
    } else if (is_arg1_constant and is_arg2_constant) {
        int result_value = this->getConstantValue(arg1);
        switch (op.getType()) {
            case pl0::Operator::Add: { result_value += this->getConstantValue(arg2); } break;
            case pl0::Operator::Subtract: { result_value -= this->getConstantValue(arg2); } break;
            case pl0::Operator::Multiply: { result_value *= this->getConstantValue(arg2); } break;
            case pl0::Operator::Divide: { result_value /= this->getConstantValue(arg2); } break;
            default: break;
        }
        constant_operand.setValue(result_value);
        code.clearExpression().setOperator(Operator::Assign).setLhsOperand(constant_operand);
        m_constant_map[result.getName()] = result_value;
    } else if (is_arg1_constant) {
        int constant_value = this->getConstantValue(arg1);
        constant_operand.setValue(constant_value);
        code.setLhsOperand(constant_operand);
    } else if (is_arg2_constant) {
        int constant_value = this->getConstantValue(arg2);
        constant_operand.setValue(constant_value);
        code.setRhsOperand(constant_operand);
    }
}

bool pl0::QuaternionCodeOptimizer::isOperandConstant(const Operand &operand) const
{
    if (not operand) { return false; }
    if (operand.isLiteral()) { return true; }
    return m_constant_map.contains(operand.getName());
}

int pl0::QuaternionCodeOptimizer::getConstantValue(const Operand &operand) const
{
    if (operand.isLiteral()) { return operand.getValue(); }
    return m_constant_map.at(operand.getName());
}

void pl0::QuaternionCodeOptimizer::updateEqualExpression()
{
    for (int i = 0; i < m_code_list.size(); ++i) {
        auto &code = m_code_list[i];
        auto it = m_equivalence_map.find(code.getLhsOperand().getName());
        if (it != m_equivalence_map.end()) {
            code.setLhsOperand(it->second);
        }
        it = m_equivalence_map.find(code.getRhsOperand().getName());
        if (it != m_equivalence_map.end()) {
            code.setRhsOperand(it->second);
        }
        for (int j = i - 1; j >= 0; --j) {
            auto &prev_code = m_code_list[j];
            if (m_constant_map.contains(prev_code.getResult().getName())) { continue; }
            if (not code.isExpressionEqual(prev_code)) { continue; }
            code.clearExpression().setOperator(Operator::Assign).setLhsOperand(prev_code.getResult());
            m_equivalence_map[code.getResult().getName()] = prev_code.getResult();
            break;
        }
    }
}

void pl0::QuaternionCodeOptimizer::judgeDependent()
{
    for (int i = static_cast<int>(m_code_list.size() - 1); i >= 0; --i) {
        const auto &[op, arg1, arg2, result] = m_code_list[i];
        m_dependent_result_index_map[result.getName()].pop();
        if (not m_dependent[i] and result.isTemporary()) { continue; }
        this->setDependent(arg1);
        this->setDependent(arg2);
    }
}

void pl0::QuaternionCodeOptimizer::setDependent(const Operand &result)
{
    if (not result or result.isLiteral()) { return; }
    std::string name = result.getName();
    if (m_equivalence_map.contains(name)) {
        name = m_equivalence_map.at(name).getName();
    }
    auto it = m_dependent_result_index_map.find(name);
    if (it == m_dependent_result_index_map.end() or it->second.empty()) { return; }
    m_dependent[it->second.top()] = true;
}

void pl0::QuaternionCodeOptimizer::eliminateRedundant()
{
    std::unordered_set<std::string> used_result;
    for (int i = static_cast<int>(m_code_list.size() - 1); i >= 0; --i) {
        const Operand &result = m_code_list[i].getResult();
        if (m_dependent[i] or result.isTemporary()) { continue; }
        if (used_result.contains(result.getName())) { continue; }
        used_result.insert(result.getName());
        m_dependent[i] = true;
    }
}
