#include "VirtualMachine.h"

pl0::VirtualMachine::VirtualMachine()
{
}

void pl0::VirtualMachine::execute(QuaternionCodeListView code_list)
{
    this->initialize();
    for (int i = 0; i < code_list.size(); ++i) {
        const auto &code = code_list[i];
        if (not code.getOperator().is(Operator::Label)) { continue; }
        m_label_map[code.getResult().getName()] = i;
    }
    int pc = 0;
    while (pc < code_list.size()) {
        const auto &[op, arg1, arg2, result] = code_list[pc];
        const std::string &arg1_name = arg1.getName();
        const std::string &arg2_name = arg2.getName();
        const std::string &result_name = result.getName();
        switch (op.getType()) {
            case Operator::ConstDeclare : {
                m_memory[result_name] = this->getValue(arg1);
            } break;
            case Operator::VarDeclare : {
                m_memory[result_name] = this->getValue(arg1);
            } break;
            case Operator::Assign : {
                m_memory[result_name] = this->getValue(arg1);
            } break;
            case Operator::Add : {
                m_memory[result_name] = this->getValue(arg1) + this->getValue(arg2);
            } break;
            case Operator::Subtract : {
                m_memory[result_name] = this->getValue(arg1) - this->getValue(arg2);
            } break;
            case Operator::Multiply : {
                m_memory[result_name] = this->getValue(arg1) * this->getValue(arg2);
            } break;
            case Operator::Divide : {
                int divisor = this->getValue(arg2);
                if (divisor == 0) {
                    throw std::runtime_error("Division by zero");
                }
                m_memory[result_name] = this->getValue(arg1) / divisor;
            } break;
            case Operator::UnaryMinus : {
                m_memory[result_name] = -this->getValue(arg1);
            } break;
            case Operator::Odd : {
                m_memory[result_name] = this->getValue(arg1) % 2 == 1;
            } break;
            case Operator::Greater : {
                m_memory[result_name] = this->getValue(arg1) > this->getValue(arg2);
            } break;
            case Operator::Less : {
                m_memory[result_name] = this->getValue(arg1) < this->getValue(arg2);
            } break;
            case Operator::GreaterEqual : {
                m_memory[result_name] = this->getValue(arg1) >= this->getValue(arg2);
            } break;
            case Operator::LessEqual : {
                m_memory[result_name] = this->getValue(arg1) <= this->getValue(arg2);
            } break;
            case Operator::Equal : {
                m_memory[result_name] = this->getValue(arg1) == this->getValue(arg2);
            } break;
            case Operator::NotEqual : {
                m_memory[result_name] = this->getValue(arg1) != this->getValue(arg2);
            } break;
            case Operator::Jump : {
                pc = m_label_map[result_name];
                continue;  // 跳过pc++
            } break;
            case Operator::JumpIfZero : {
                if (not this->getValue(arg1)) {
                    pc = m_label_map[result_name];
                    continue;  // 跳过pc++
                }
            } break;
            case Operator::Read : {
                int value;
                std::cout << "Enter value for " << result_name << ": ";
                std::cin >> value;
                m_memory[result_name] = value;
            } break;
            case Operator::Write : {
                std::cout << result_name << '=' << this->getValue(result) << std::endl;
            } break;
            case Operator::Label : {
                // 这些是控制标记，不需要执行操作
            } break;
        }
        ++pc;
    }
}

void pl0::VirtualMachine::initialize()
{
    m_memory.clear();
    m_label_map.clear();
}

int pl0::VirtualMachine::getValue(const Operand &operand)
{
    if (operand.isLiteral()) { return operand.getValue(); }
    return m_memory[operand.getName()];
}
