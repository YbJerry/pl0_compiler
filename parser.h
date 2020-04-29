#ifndef _PARSER_H_
#define _PARSER_H_

#include <iostream>
#include "globals.h"
#include "lexer.h"

class Parser : public Lexer{
public:
    Parser():Lexer(){};
    Parser(const istream& fin):Lexer(fin){};
    ~Parser(){};

    void parseProgram();

private:
    void parseSubProgram();
    void parseConstDeclaration();
    void parseVarDeclaration();
    void parseStatement();
    void parseCondition();
    void parseExpression();
    void parseTerm();
    void parseFactor();

    void matchToken(const Token &token);

    Token token = Token::NUL;
};

#endif