#include "Lexer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <format>
#include "Compiler.h"
#include "VirtualMachine.h"
#include "QuaternionCodeOptimizer.h"

int main() {
    std::vector<std::string> paths = {
        "./test/test2.txt",
    };
    for (const auto &path : paths) {
        pl0::Lexer lexer;
        std::cout << std::format("{:*^20}\nTesting {} ... ", "", path) << std::endl;
        lexer.readFile(path.c_str());
        std::cout << lexer.getErrorMessageManager() << std::endl;
        pl0::Compiler compiler(lexer.getTokenListView());
        compiler.compile();
        std::cout << compiler.getErrorMessageManager() << std::endl;
        pl0::VirtualMachine virtual_machine;        
        virtual_machine.execute(compiler.getQuaternionCodeListView());
    }
    return 0;
}

