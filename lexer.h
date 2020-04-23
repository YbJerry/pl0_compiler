#ifndef _LEXER_H_
#define _LEXER_H_

#include "globals.h"
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <iomanip>

using namespace std;

class Lexer {
public:
    // Learn Lemon's habits. Use English to write comment.
    // I'm so noob that I forget that C++'s initialization rules.
    // A data member which is a class,
    Lexer()
        : lin(cin.rdbuf()) {};
    Lexer(const istream& fin)
        : lin(fin.rdbuf()) {};
    ~Lexer(){};

    Token getToken();
    const Token getToken() const;
    void countIdent();
    void printToken();
    pair<string, string> transToken(Token token);

    inline pair<string, string> resTrans(const string &s1, const string &s2){
        return make_pair(s1, s2);
    }

private:
    // DFA's States
    enum class DFAStates {
        DONE = 0,
        START,
        BLANK,
        NEWLINE,
        NONBLANK,
        NUMBER,
        ALNUM,
        OP,
        COMMENT
    };

    // reserved keywords
    const unordered_map<string, Token> reservedWords = {
        { "begin", Token::BEGINSYM },
        { "end", Token::ENDSYM },
        { "if", Token::IFSYM },
        { "then", Token::THENSYM },
        { "while", Token::WHILESYM },
        { "write", Token::WRITESYM },
        { "read", Token::READSYM },
        { "do", Token::DOSYM },
        { "call", Token::CALLSYM },
        { "const", Token::CONSTSYM },
        { "var", Token::VARSYM },
        { "odd", Token::ODDSYM },
        { "procedure", Token::PROCSYM },
    };

    char getCh();
    void unGetCh(char ch);

    // lexer's input
    istream lin;
    string tokenString;
    int lineNo = 1, charNo = 1;
    int commentLevel = 0;
};

#endif