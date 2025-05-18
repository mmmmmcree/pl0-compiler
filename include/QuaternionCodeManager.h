#pragma once
#include "QuaternionCode.h"
#include <vector>
#include <span>
#include <iostream>

namespace pl0 {
    class QuaternionCodeManager
    {
    public:
        QuaternionCodeManager() = default;
        void emitCode(const QuaternionCode &code);
        const Operand &emitCodeWithTempResult(const QuaternionCode &code);
        const Operand &emitCodeWithNewLabel(const QuaternionCode &code);
        QuaternionCodeListView getCodeListView() const;
        void optimize();
    private:
        QuaternionCodeList m_code_list;
        int m_temp_result_count = 0;
        int m_label_count = 0;
    };

    std::ostream& operator<<(std::ostream& os, const QuaternionCodeManager& manager);
}