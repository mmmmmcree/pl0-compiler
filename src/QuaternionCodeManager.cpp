#include "QuaternionCodeManager.h"
#include "QuaternionCodeOptimizer.h"
#include <iomanip>

void pl0::QuaternionCodeManager::emitCode(const QuaternionCode &code)
{
    m_code_list.emplace_back(code);
}

const pl0::Operand &pl0::QuaternionCodeManager::emitCodeWithTempResult(const QuaternionCode &code)
{
    Operand temp_result;
    temp_result.setName("t" + std::to_string(m_temp_result_count++)).setTemporary(true);
    QuaternionCode new_code = code;
    new_code.setResult(temp_result);
    return m_code_list.emplace_back(new_code).getResult();
}

const pl0::Operand &pl0::QuaternionCodeManager::emitCodeWithNewLabel(const QuaternionCode &code)
{
    Operand label;
    label.setName("L" + std::to_string(m_label_count++)).setTemporary(true);
    QuaternionCode new_code = code;
    new_code.setResult(label);
    return m_code_list.emplace_back(new_code).getResult();
}

pl0::QuaternionCodeListView pl0::QuaternionCodeManager::getCodeListView() const
{
    return m_code_list;
}

void pl0::QuaternionCodeManager::optimize()
{
    std::cout << "Before optimization: \n" << *this << std::endl;
    QuaternionCodeList optimized_code_list;
    QuaternionCodeList temp_code_list;
    for (const auto &code : m_code_list) {
        const Operator &op = code.getOperator();
        if (op.is(Operator::Assign) or op.is(Operator::Add) or
            op.is(Operator::Subtract) or op.is(Operator::Multiply) or op.is(Operator::Divide)) {
            temp_code_list.emplace_back(code);
        } else {
            temp_code_list = QuaternionCodeOptimizer(temp_code_list).getOptimizedResult();
            optimized_code_list.insert(optimized_code_list.end(), temp_code_list.begin(), temp_code_list.end());
            optimized_code_list.emplace_back(code);
            temp_code_list.clear();
        }
    }
    m_code_list = optimized_code_list;
    std::cout << "After optimization: \n" << *this << std::endl;
}

std::ostream &pl0::operator<<(std::ostream &os, const QuaternionCodeManager &manager)
{
    auto code_list_view = manager.getCodeListView();
    os << "QuaternionCodeManager" << std::endl;
    for (int i = 0; i < code_list_view.size(); ++i) {
        os << i << ": \t"<< code_list_view[i] << std::endl;
    }
    return os;
}
