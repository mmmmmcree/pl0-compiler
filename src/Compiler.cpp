#include "Compiler.h"
#include <format>
#include <functional>

pl0::Compiler::Compiler(TokenListView token_list) :
    m_cur_token(token_list)
{
    if (token_list.empty()) {
        m_err_manager.addErrorMessage("empty token list");
    }
}

bool pl0::Compiler::compile()
{
    this->parseProgram();
    m_code_manager.optimize();
    return m_err_manager.hasError();
}

const pl0::ErrorMessageManager &pl0::Compiler::getErrorMessageManager() const
{
    return m_err_manager;
}

pl0::QuaternionCodeListView pl0::Compiler::getQuaternionCodeListView() const
{
    return m_code_manager.getCodeListView();
}

bool pl0::Compiler::expect(TokenType type, bool error_if_failed, bool skip)
{
    if (not m_cur_token->is(type)) {
        if (error_if_failed) {
            m_err_manager.addErrorMessage(m_cur_token->getPosition(),
                std::format("expected {}, but got {}", constants::token_type_strings.at(type), m_cur_token->getValue()));
        }
        return false;
    }
    if (skip) { this->skip(type); }
    return true;
}

bool pl0::Compiler::storeIdentifier(IdentifierType type, bool error_if_failed, bool skip)
{
    m_identifiers[m_cur_token->getValue()] = Identifier(type, m_cur_token->getValue());
    return this->expect(TokenType::ident, error_if_failed, skip);
}

bool pl0::Compiler::expectIdentifier(IdentifierType type, bool error_if_failed, bool skip)
{
    bool result = true;
    auto it = m_identifiers.find(m_cur_token->getValue());
    if (it == m_identifiers.end()) {
        m_err_manager.addErrorMessage(m_cur_token->getPosition(),
            std::format("undefined identifier {}", m_cur_token->getValue()));
        result = false;
    } else if (not it->second.is(type)) {
        m_err_manager.addErrorMessage(m_cur_token->getPosition(),
            std::format("expected {}, but got {}", constants::identifier_type_strings.at(type), m_cur_token->getValue()));
        result = false;
    }
    return result &= this->expect(TokenType::ident, error_if_failed, skip);
}

void pl0::Compiler::skip(TokenType type)
{
    if (m_cur_token and m_cur_token->is(type)) { ++m_cur_token; }
}

void pl0::Compiler::skipAll(TokenType type)
{
    while (m_cur_token and m_cur_token->is(type)) { ++m_cur_token; }
}

//- program = block {";"} "." ;
void pl0::Compiler::parseProgram()
{
    this->parseBlock();
    this->skipAll(TokenType::semicolon); // allow semicolon at the end of program
    this->expect(TokenType::period, true, false);
}

/*
- block = [ "const" ident "=" number {"," ident "=" number} ";"]
-             [ "var" ident {"," ident} ";"]
-             { "procedure" ident ";" block ";" } statement ;
*/
void pl0::Compiler::parseBlock()
{
    if (m_cur_token->is(TokenType::constsym)) { this->parseConstDeclaration(); }
    if (m_cur_token->is(TokenType::varsym)) { this->parseVarDeclaration(); }
    while (m_cur_token->is(TokenType::proceduresym)) { this->parseProcedureDeclaration(); }
    this->parseStatement();
}

//- "const" ident "=" number {"," ident "=" number} ";"
void pl0::Compiler::parseConstDeclaration()
{
    this->expect(TokenType::constsym);
    do {
        Operand constant(*m_cur_token);
        QuaternionCode code;
        code.setOperator(Operator::ConstDeclare).setResult(constant);
        m_code_manager.emitCode(code);
        this->storeIdentifier(IdentifierType::Const);
        this->expect(TokenType::eql);
        this->skip(TokenType::SignOperator);
        this->expect(TokenType::number);
    } while (this->expect(TokenType::comma, false));
    this->expect(TokenType::semicolon);
}

//- "var" ident {"," ident} ";"
void pl0::Compiler::parseVarDeclaration()
{

    this->expect(TokenType::varsym);
    bool loop_continue = true;
    do {
        Operand var(*m_cur_token);
        QuaternionCode code;
        code.setOperator(Operator::VarDeclare).setResult(var);
        m_code_manager.emitCode(code); 
        this->storeIdentifier(IdentifierType::Var);
    } while (this->expect(TokenType::comma, false));
    this->expect(TokenType::semicolon);
}

//- "procedure" ident ";" block ";" 
void pl0::Compiler::parseProcedureDeclaration()
{
    this->expect(TokenType::proceduresym);
    this->storeIdentifier(IdentifierType::Procedure);
    this->expect(TokenType::semicolon);
    this->parseBlock();
    this->expect(TokenType::semicolon);
}

/*
- statement = [ ident ":=" expression | "call" ident 
-                    | "?" ident | "!" expression 
-                    | "begin" statement {";" statement } "end" 
-                    | "if" condition "then" statement 
-                    | "while" condition "do" statement
-                    | "read" '(' ident {',' ident} ')'
-                    | "write" '(' expression {',' expression} ')' ];
*/
void pl0::Compiler::parseStatement()
{
    static const PaserTable statement_parsers = {
        {TokenType::ident, std::bind(&Compiler::parseAssignmentStatement, this)},
        {TokenType::callsym, std::bind(&Compiler::parseCallStatement, this)},
        {TokenType::beginsym, std::bind(&Compiler::parseBeginStatement, this)},
        {TokenType::ifsym, std::bind(&Compiler::parseIfStatement, this)},
        {TokenType::whilesym, std::bind(&Compiler::parseWhileStatement, this)},
        {TokenType::readsym, std::bind(&Compiler::parseReadStatement, this)},
        {TokenType::writesym, std::bind(&Compiler::parseWriteStatement, this)},
    };
    auto it = statement_parsers.find(m_cur_token->getType());
    if (it != statement_parsers.end()) { it->second(); }
}

//- "ident":= expression
void pl0::Compiler::parseAssignmentStatement()
{
    Operand target(*m_cur_token);
    this->expectIdentifier(IdentifierType::Var);
    this->expect(TokenType::becomes);
    Operand value = this->parseExpression();
    QuaternionCode code;
    code.setOperator(Operator::Assign).setLhsOperand(value).setResult(target);
    m_code_manager.emitCode(code);
}

//- "call" ident
void pl0::Compiler::parseCallStatement()
{
    this->expect(TokenType::callsym);
    this->expectIdentifier(IdentifierType::Procedure);
}

//-  "begin" statement {";" statement } "end"
void pl0::Compiler::parseBeginStatement()
{
    this->expect(TokenType::beginsym);
    this->parseStatement();
    while (m_cur_token->is(TokenType::semicolon)) {
        this->skip(TokenType::semicolon);
        this->parseStatement();
    }
    this->expect(TokenType::endsym);
}

// - "if" condition "then" statement ["else" statement] ;
void pl0::Compiler::parseIfStatement()
{
    this->expect(TokenType::ifsym);
    Operand condition_result = this->parseCondition();
    this->expect(TokenType::thensym);

    QuaternionCode code;
    code.setOperator(Operator::JumpIfZero).setLhsOperand(condition_result);
    Operand end_label = m_code_manager.emitCodeWithNewLabel(code);

    this->parseStatement();
    this->skipAll(TokenType::semicolon);
    if (not m_cur_token->is(TokenType::elsesym)) {
        code.setOperator(Operator::Label).setResult(end_label);
        m_code_manager.emitCode(code);
        return;
    }
    this->skip(TokenType::elsesym);
    Operand else_label = end_label;
    code.clear().setOperator(Operator::Jump);
    end_label = m_code_manager.emitCodeWithNewLabel(code);
    code.setOperator(Operator::Label).setResult(else_label);
    m_code_manager.emitCode(code);
    this->parseStatement();
    code.setResult(end_label);
    m_code_manager.emitCode(code);
}

//- "while" condition "do" statement
void pl0::Compiler::parseWhileStatement()
{
    this->expect(TokenType::whilesym);
    QuaternionCode code;
    code.setOperator(Operator::Label);
    Operand start_label = m_code_manager.emitCodeWithNewLabel(code);
    Operand condition_result = this->parseCondition();
    code.setOperator(Operator::JumpIfZero).setLhsOperand(condition_result);
    Operand end_label = m_code_manager.emitCodeWithNewLabel(code);
    this->expect(TokenType::dosym);
    this->parseStatement();
    code.clear().setOperator(Operator::Jump).setResult(start_label);
    m_code_manager.emitCode(code);
    code.setOperator(Operator::Label).setResult(end_label);
    m_code_manager.emitCode(code);
}

//-"read" '(' ident {',' ident} ')'
void pl0::Compiler::parseReadStatement()
{
    this->expect(TokenType::readsym);
    this->expect(TokenType::lparen);
    do {
        Operand target;
        target.setName(m_cur_token->getValue());
        this->expectIdentifier(IdentifierType::Var);
        QuaternionCode code;
        code.setOperator(Operator::Read).setResult(target);
        m_code_manager.emitCode(code);
    } while (this->expect(TokenType::comma, false));
    this->expect(TokenType::rparen);
}

//- "write" '(' expression {',' expression} ')'
void pl0::Compiler::parseWriteStatement()
{
    this->expect(TokenType::writesym);
    this->expect(TokenType::lparen);
    do {
        Operand target = this->parseExpression();
        QuaternionCode code;
        code.setOperator(Operator::Write).setResult(target);
        m_code_manager.emitCode(code);
    } while (this->expect(TokenType::comma, false));
    this->expect(TokenType::rparen);
}

//- condition = "odd" expression | expression ("="|"#"|"<"|"<="|">"|">=") expression ;
pl0::Operand pl0::Compiler::parseCondition()
{
    if (m_cur_token->is(TokenType::oddsym)) {
        this->skip(TokenType::oddsym);
        Operand operand = this->parseExpression();
        QuaternionCode code;
        code.setOperator(Operator::Odd).setLhsOperand(operand);
        return m_code_manager.emitCodeWithTempResult(code);
    }
    Operand lhs_operand = this->parseExpression();
    Operator op(*m_cur_token);
    this->expect(TokenType::BinaryRelationOperator);
    Operand rhs_operand = this->parseExpression();
    QuaternionCode code;
    code.setLhsOperand(lhs_operand).setOperator(op).setRhsOperand(rhs_operand);
    return m_code_manager.emitCodeWithTempResult(code);
}

//- expression = term { ("+"|"-") term};
pl0::Operand pl0::Compiler::parseExpression()
{
    Operand lhs_operand = this->parseTerm();
    while (m_cur_token->is(TokenType::plus | TokenType::minus)) {
        Operator op(*m_cur_token);
        this->skip(m_cur_token->getType());
        QuaternionCode code;
        code.setOperator(op).setLhsOperand(lhs_operand).setRhsOperand(this->parseTerm());
        lhs_operand = m_code_manager.emitCodeWithTempResult(code);
    }
    return lhs_operand;
}

//- term = factor {("*"|"/") factor};
pl0::Operand pl0::Compiler::parseTerm()
{
    Operand lhs_operand = this->parseFactor();
    while (m_cur_token->is(TokenType::times | TokenType::slash)) {
        Operator op(*m_cur_token);
        this->skip(m_cur_token->getType());
        QuaternionCode code;
        code.setOperator(op).setLhsOperand(lhs_operand).setRhsOperand(this->parseFactor());
        lhs_operand = m_code_manager.emitCodeWithTempResult(code);
    }
    return lhs_operand;
}

//- factor = [ "+"|"-"] number | [ "+"|"-"]  ident | [ "+"|"-"]  "(" expression ")";
pl0::Operand pl0::Compiler::parseFactor()
{
    Operand result;
    const PaserTable factor_parsers = {
        {TokenType::ident, [&] {
            result = Operand(*m_cur_token);
            this->expectIdentifier(IdentifierType::Var | IdentifierType::Const);
        }},
        {TokenType::number, [&] {
            result = Operand(*m_cur_token);
            this->expect(TokenType::number);
        }},
        {TokenType::lparen, [&] {
            this->expect(TokenType::lparen);
            result = this->parseExpression();
            this->expect(TokenType::rparen);
        }},
    };
    Operator op;
    auto it = factor_parsers.find(m_cur_token->getType());
    if (m_cur_token->is(TokenType::minus)) {
        op = Operator(*m_cur_token);
    }
    this->skip(TokenType::SignOperator);
    if (it != factor_parsers.end()) { it->second(); }
    if (op) {
        QuaternionCode code;
        code.setOperator(op).setLhsOperand(result);
        result = m_code_manager.emitCodeWithTempResult(code);
    }
    return result;
}
