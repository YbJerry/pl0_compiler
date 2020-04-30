#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream>
#include <vector>
#include "globals.h"
#include "lexer.h"
#include <algorithm>

class Parser : public Lexer{
public:
    Parser():Lexer(){};
    Parser(const istream& fin):Lexer(fin){};
    ~Parser(){};

    void parseProgram();

private:
    enum class NoTerminal{
        PROG, SUBPROG, CONSTDEC, VARDEC, STATEMENT, CONDITION, EXPRESSION, TERM, FACTOR
    };


    void parseSubProgram();
    void parseConstDeclaration();
    void parseVarDeclaration();
    void parseStatement();
    void parseCondition();
    void parseExpression();
    void parseTerm();
    void parseFactor();
    
    bool matchToken(const Token &token);
    void errorRecovery(const NoTerminal &nt);

    Token token = Token::NUL;
    NoTerminal LLState = NoTerminal::PROG;

    const unordered_map<NoTerminal, vector<Token>> firstSets =
        {
            {NoTerminal::SUBPROG, vector<Token>{Token::CONSTSYM, Token::VARSYM, Token::PROCSYM, Token::IDENT, Token::IFSYM, Token::CALLSYM, Token::BEGINSYM, Token::WHILESYM, Token::READSYM, Token::WRITESYM}},
            {NoTerminal::STATEMENT, vector<Token>{Token::IDENT, Token::CALLSYM, Token::BEGINSYM, Token::IFSYM, Token::WHILESYM, Token::READSYM, Token::WRITESYM}},
            {NoTerminal::CONDITION, vector<Token>{Token::ODDSYM, Token::PLUS, Token::MINUS, Token::LPAREN, Token::IDENT, Token::NUMBER}},
            {NoTerminal::EXPRESSION, vector<Token>{Token::PLUS, Token::MINUS, Token::LPAREN, Token::IDENT, Token::NUMBER}},
            {NoTerminal::TERM, vector<Token>{Token::IDENT, Token::NUMBER, Token::LPAREN}},
            {NoTerminal::FACTOR, vector<Token>{Token::IDENT, Token::NUMBER, Token::LPAREN}}
        };

    const unordered_map<NoTerminal, vector<Token>> followSets =
        {
            {NoTerminal::SUBPROG, vector<Token>{Token::PERIOD, Token::SEMICOLON}},
            {NoTerminal::STATEMENT, vector<Token>{Token::PERIOD, Token::SEMICOLON, Token::ENDSYM}},
            {NoTerminal::CONDITION, vector<Token>{Token::THENSYM, Token::DOSYM}},
            {NoTerminal::EXPRESSION, vector<Token>{Token::PERIOD, Token::SEMICOLON, Token::COMMA, Token::RPAREN, Token::EQL, Token::NEQ, Token::LSS, Token::LEQ, Token::GTR, Token::GEQ, Token::ENDSYM, Token::THENSYM, Token::DOSYM}},
            {NoTerminal::TERM, vector<Token>{Token::PERIOD, Token::SEMICOLON, Token::COMMA, Token::RPAREN, Token::EQL, Token::NEQ, Token::LSS, Token::LEQ, Token::GTR, Token::GEQ, Token::PLUS, Token::MINUS, Token::ENDSYM, Token::THENSYM, Token::DOSYM}},
            {NoTerminal::FACTOR, vector<Token>{Token::PERIOD, Token::SEMICOLON, Token::COMMA, Token::RPAREN, Token::EQL, Token::NEQ, Token::LSS, Token::LEQ, Token::GTR, Token::GEQ, Token::PLUS, Token::MINUS, Token::TIMES, Token::SLASH, Token::ENDSYM, Token::THENSYM, Token::DOSYM}},
            {NoTerminal::CONSTDEC, vector<Token>{Token::COMMA, Token::SEMICOLON}},
            {NoTerminal::VARDEC, vector<Token>{Token::COMMA, Token::SEMICOLON}}
        };
};

#endif