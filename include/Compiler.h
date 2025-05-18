#pragma once

#include "Definition.h"
#include "Token.h"
#include "ErrorMessageManager.h"
#include "ForwardIterator.h"
#include "Lexer.h"
#include "Identifier.h"
#include "QuaternionCodeManager.h"
#include <string>

namespace pl0 {
    class Compiler
    {
    public:
        Compiler(TokenListView token_list);
        bool compile();
        const ErrorMessageManager &getErrorMessageManager() const;
        QuaternionCodeListView getQuaternionCodeListView() const;
    private:
        bool expect(TokenType type, bool error_if_failed = true, bool skip = true);
        bool storeIdentifier(IdentifierType type, bool error_if_failed = true, bool skip = true);
        bool expectIdentifier(IdentifierType type, bool error_if_failed = true, bool skip = true);
        void skip(TokenType type);
        void skipAll(TokenType type);
        void parseProgram();
        void parseBlock();
        void parseConstDeclaration();
        void parseVarDeclaration();
        void parseProcedureDeclaration();
        void parseStatement();
        void parseAssignmentStatement();
        void parseCallStatement();
        void parseBeginStatement();
        void parseIfStatement();
        void parseWhileStatement();
        void parseReadStatement();
        void parseWriteStatement();
        Operand parseCondition();
        Operand parseExpression();
        Operand parseTerm();
        Operand parseFactor();
    private:
        ForwardIterator<TokenListView::iterator> m_cur_token;
        IdentifierMap m_identifiers;
        ErrorMessageManager m_err_manager{"Compiler"};
        QuaternionCodeManager m_code_manager;
    };
}