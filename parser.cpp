#include "parser.h"

void Parser::parseProgram(){
    matchToken(Token::NUL);
    parseSubProgram();
    matchToken(Token::PERIOD);
}

void Parser::parseSubProgram(){
    if(token == Token::CONSTSYM){
        matchToken(Token::CONSTSYM);
        parseConstDeclaration();
        while(token == Token::COMMA){
            matchToken(Token::COMMA);
            parseConstDeclaration();
        }
        matchToken(Token::SEMICOLON);
    }

    if(token == Token::VARSYM){
        matchToken(Token::VARSYM);
        parseVarDeclaration();
        while(token == Token::COMMA){
            matchToken(Token::COMMA);
            parseVarDeclaration();
        }
        matchToken(Token::SEMICOLON);
    }

    while(token == Token::PROCSYM){
        matchToken(Token::PROCSYM);
        matchToken(Token::IDENT);
        matchToken(Token::SEMICOLON);
        parseSubProgram();
        matchToken(Token::SEMICOLON);
    }

    parseStatement();
}

void Parser::parseConstDeclaration(){
    matchToken(Token::IDENT);
    matchToken(Token::EQL);
    matchToken(Token::NUMBER);
}

void Parser::parseVarDeclaration(){
    matchToken(Token::IDENT);
}
     
void Parser::parseStatement(){
    switch (token)
    {
    case Token::IDENT:
        matchToken(Token::IDENT);
        matchToken(Token::BECOMES);
        parseExpression();
        break;
    case Token::IFSYM:
        matchToken(Token::IFSYM);
        parseCondition();
        matchToken(Token::THENSYM);
        parseStatement();
        break;
    case Token::WHILESYM:
        matchToken(Token::WHILESYM);
        parseCondition();
        matchToken(Token::DOSYM);
        parseStatement();
        break;
    case Token::CALLSYM:
        matchToken(Token::CALLSYM);
        matchToken(Token::IDENT);
        break;
    case Token::READSYM:
        matchToken(Token::READSYM);
        matchToken(Token::LPAREN);
        matchToken(Token::IDENT);
        while(token == Token::COMMA){
            matchToken(Token::COMMA);
            matchToken(Token::IDENT);
        }
        matchToken(Token::RPAREN);
        break;
    case Token::WRITESYM:
        matchToken(Token::WRITESYM);
        matchToken(Token::LPAREN);
        parseExpression();
        while(token == Token::COMMA){
            matchToken(Token::COMMA);
            parseExpression();
        }
        matchToken(Token::RPAREN);
        break;
    case Token::BEGINSYM:
        matchToken(Token::BEGINSYM);
        parseStatement();
        while(token == Token::SEMICOLON){
            matchToken(Token::SEMICOLON);
            parseStatement();
        }
        matchToken(Token::ENDSYM);
        break;
    // if statement is empty, its follow set is . and end
    case Token::ENDSYM:
    case Token::PERIOD:
        break;
    default:
        break;
    }
}

void Parser::parseCondition(){
    switch(token){
    case Token::ODDSYM:
        matchToken(Token::ODDSYM);
        parseExpression();
        break;
    // condition's first set
    case Token::PLUS:
    case Token::MINUS:
    case Token::LPAREN:
    case Token::NUMBER:
    case Token::IDENT:
        parseExpression();
        switch (token)
        {
        case Token::EQL:
        case Token::NEQ:
        case Token::LSS:
        case Token::LEQ:
        case Token::GTR:
        case Token::GEQ:
            matchToken(token);
            break;
        default:
            matchToken(Token::ERROR);
            break;
        }
        parseExpression();
        break;
    default:
        matchToken(Token::ERROR);
        break;
    }
}

void Parser::parseExpression(){
    if(token == Token::PLUS || token == Token::MINUS)
        matchToken(token);
    parseFactor();
    while(token == Token::TIMES || token == Token::SLASH){
        matchToken(token);
        parseFactor();
    }
}

void Parser::parseFactor(){
    switch (token)
    {
    case Token::IDENT:
    case Token::NUMBER:
        matchToken(token);
        break;
    case Token::LPAREN:
        matchToken(Token::LPAREN);
        parseExpression();
        matchToken(Token::RPAREN);
    default:
        matchToken(Token::ERROR);
        break;
    }
}

void Parser::matchToken(const Token &token){
    if(token != this->token){
        cout << "\033[41;36m ERROR " <<lineNo << ":" << charNo << "\33[0m" << ": syntax error!" << endl;
        exit(0);
    }else{
        this->token = getToken();
    }
}