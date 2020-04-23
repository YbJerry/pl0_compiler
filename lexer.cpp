#include "lexer.h"

char Lexer::getCh()
{
    ++charNo;
    return lin.get();
}

void Lexer::unGetCh(char ch)
{
    --charNo;
    lin.unget();
}

const Token Lexer::getToken() const{
    return getToken();
}

Token Lexer::getToken()
{
    DFAStates state = DFAStates::START;
    Token res;
    tokenString.clear();
    while (state != DFAStates::DONE) {
        int ch = getCh();
        switch (state) {
        case DFAStates::START:
        case DFAStates::BLANK:
            switch (ch) {
            case ' ':
                state = DFAStates::BLANK;
                break;
            case '\t':
                charNo = ((charNo) / TABDISTANCE + 1) * TABDISTANCE + 1;
                state = DFAStates::BLANK;
                break;
            // 可以处理多种系统下换行标志
            case '\r':
                ch = getCh();
                if (ch != '\n')
                    unGetCh(ch);
            // 此处无break
            case '\n':
                state = DFAStates::BLANK;
                charNo = 1;
                ++lineNo;
                break;
            case EOF:
                state = DFAStates::DONE;
                res = Token::NUL;
                break;
            default:
                unGetCh(ch);
                state = DFAStates::NONBLANK;
                break;
            }
            break;
        case DFAStates::NONBLANK:
            if (isalpha(ch)) {
                state = DFAStates::ALNUM;
                unGetCh(ch);
                break;
            } else if (isdigit(ch)) {
                state = DFAStates::NUMBER;
                unGetCh(ch);
                break;
            } else {
                state = DFAStates::OP;
                unGetCh(ch);
                break;
            }
            break;
        case DFAStates::ALNUM:
            do {
                if (!isalnum(ch))
                    break;
                tokenString.append(1, tolower(ch));
            } while (ch = getCh());
            unGetCh(ch);
            if (reservedWords.find(tokenString) != reservedWords.end())
                res = reservedWords.at(tokenString);
            else
                res = Token::IDENT;
            state = DFAStates::DONE;
            break;
        case DFAStates::NUMBER:
            do {
                if (!isdigit(ch))
                    break;
                tokenString.append(1, ch);
            } while (ch = getCh());
            unGetCh(ch);
            res = Token::NUMBER;
            state = DFAStates::DONE;
            break;
        case DFAStates::OP:
            switch (ch) {
            case '+':
                res = Token::PLUS;
                break;
            case '-':
                res = Token::MINUS;
                break;
            case '*':
                res = Token::TIMES;
                break;
            case '/':
                res = Token::SLASH;
                break;
            case '=':
                res = Token::EQL;
                break;
            case '#':
                res = Token::NEQ;
                break;
            case '<':
                if ((ch = getCh()) == '=') {
                    res = Token::LEQ;
                    break;
                }
                unGetCh(ch);
                res = Token::LSS;
                break;
            case '>':
                if ((ch = getCh()) == '=') {
                    res = Token::GEQ;
                    break;
                }
                unGetCh(ch);
                res = Token::GTR;
                break;
            case ':':
                if ((ch = getCh()) == '=') {
                    res = Token::BECOMES;
                    break;
                }
                unGetCh(ch);
                res = Token::ERROR;
                break;
            case '(':
                res = Token::LPAREN;
                break;
            case ')':
                res = Token::RPAREN;
                break;
            case ',':
                res = Token::COMMA;
                break;
            case ';':
                res = Token::SEMICOLON;
                break;
            case '.':
                res = Token::PERIOD;
                break;
            default:
                res = Token::ERROR;
                break;
            }
            state = DFAStates::DONE;
            if (ch == '{') {
                ++commentLevel;
                state = DFAStates::COMMENT;
            }
            break;
        case DFAStates::COMMENT:
            do {
                if (ch == '}') {
                    --commentLevel;
                } else if (ch == '{') {
                    ++commentLevel;
                }
                ch = getCh();
            } while (commentLevel);
            unGetCh(ch);
            state = DFAStates::START;
            break;
        default:
            cerr << "Unknown lexical parser status!" << endl;
            break;
        }
    }
    return res;
}

void Lexer::countIdent(){
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

void Lexer::printToken(){
    Token token;
    pair<string, string> res;
    while((token = getToken()) != Token::NUL){
        res = transToken(token);
        if(res.first == "" && res.second == "")
            continue;
        cout << "( " << setw(10) << res.first << ":" << setw(10) << res.second << " )" << endl;
    }
}

pair<string, string> Lexer::transToken(Token token){
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