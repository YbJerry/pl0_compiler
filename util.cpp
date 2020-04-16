#include "util.h"

extern string tokenString;
extern int lineNo, charNo;

void countIdent(){
    unordered_map<string, int> identSum;
    Token token;
    while((token = getToken()) != Token::NUL){
        if(token == Token::IDENT){
            if(identSum.find(tokenString) != identSum.end()){
                ++identSum[tokenString];
            }else{
                identSum[tokenString] = 1;
            }
        }
    }

    for(auto m:identSum){
        cout << "(" << m.first << ": " << m.second << ")" <<endl;
    }
}

void printToken(){
    Token token;
    pair<string, string> res;
    while((token = getToken()) != Token::NUL){
        res = transToken(token);
        if(res.first == "" && res.second == "")
            continue;
        cout << "( " << setw(10) << res.first << ":" << setw(10) << res.second << " )" << endl;
    }
}

inline pair<string, string> resTrans(const string &s1, const string &s2){
    return make_pair(s1, s2);
}

pair<string, string> transToken(Token token){
    switch (token){
    case Token::BEGINSYM:
        return resTrans("BEGINSYM", "begin");
    case Token::ENDSYM:
        return resTrans("ENDSYM", "end");
    case Token::IFSYM:
        return resTrans("IFSYM", "if");
    case Token::THENSYM:
        return resTrans("THENSYM", "then");
    case Token::WHILESYM:
        return resTrans("WHILESYM", "while");
    case Token::WRITESYM:
        return resTrans("WRITESYM", "write");
    case Token::READSYM:
        return resTrans("READSYM", "read");
    case Token::DOSYM:
        return resTrans("DOSYM", "do");
    case Token::CALLSYM:
        return resTrans("CALLSYM", "call");
    case Token::CONSTSYM:
        return resTrans("CONSTSYM", "const");
    case Token::VARSYM:
        return resTrans("VARSYM", "var");
    case Token::PROCSYM:
        return resTrans("PROCSYM", "procedure");
    case Token::ODDSYM:
        return resTrans("ODDSYM", "odd");
    case Token::IDENT:
        return resTrans("IDENT", tokenString);
    case Token::NUMBER:
        return resTrans("NUMBER", tokenString);
    case Token::PLUS:
        return resTrans("PLUS", "+");
    case Token::MINUS:
        return resTrans("MINUS", "-");
    case Token::TIMES:
        return resTrans("TIMES", "*");
    case Token::SLASH:
        return resTrans("SLASH", "/");
    case Token::EQL:
        return resTrans("EQL", "=");
    case Token::NEQ:
        return resTrans("NEQ", "#");
    case Token::LSS:
        return resTrans("LSS", "<");
    case Token::LEQ:
        return resTrans("LEQ", "<=");
    case Token::GTR:
        return resTrans("GTR", ">");
    case Token::GEQ:
        return resTrans("GEQ", ">=");
    case Token::BECOMES:
        return resTrans("BECOMES", ":=");
    case Token::LPAREN:
        return resTrans("LPAREN", "(");
    case Token::RPAREN:
        return resTrans("RPAREN", ")");
    case Token::COMMA:
        return resTrans("COMMA", ",");
    case Token::SEMICOLON:
        return resTrans("SEMICOLON", ";");
    case Token::PERIOD:
        return resTrans("PERIOD", ".");
    case Token::ERROR:{
        string errorInfo;
        errorInfo += to_string(lineNo);
        errorInfo += ":";
        errorInfo += to_string(charNo);
        return resTrans("\033[41;36m ERROR \33[0m", errorInfo);
    }
        
    default:
        break;
    }
}

