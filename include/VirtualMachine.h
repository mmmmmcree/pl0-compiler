#pragma once

#include "QuaternionCode.h"
#include "Operator.h"
#include "Operand.h"
#include <unordered_set>
#include <unordered_map>

namespace pl0 {
    class VirtualMachine
    {
    public:
        using Memory = std::unordered_map<std::string, int>;
        using LabelMap = std::unordered_map<std::string, int>;
        VirtualMachine();
        void execute(QuaternionCodeListView code_list);
    private:
        void initialize();
        int getValue(const Operand &operand);
    private:
        Memory m_memory;
        LabelMap m_label_map;
    };
}