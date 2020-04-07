#include "lexer.h"

string tokenString;
int lineNo = 1, charNo = 1;

// 有限自动机状态
enum class DFAStatus{
    DONE = 0, START, BLANK, NEWLINE, NONBLANK,
    NUMBER, ALNUM, OP
};

// 保留关键字
static unordered_map<string, token> reservedWords = {
    {"begin",   token::BEGINSYM},
    {"end",     token::ENDSYM},
    {"if",      token::IFSYM},
    {"then",    token::THENSYM},
    {"while",   token::WHILESYM},
    {"write",   token::WRITESYM},
    {"read",    token::READSYM},
    {"do",      token::DOSYM},
    {"call",    token::CALLSYM},
    {"const",   token::CONSTSYM},
    {"var",     token::VARSYM},
    {"proc",    token::PROCSYM},
};

char getCh(){
    return cin.get();
}

void unGetCh(char ch){
    cin.unget();
}

token getToken(){
    DFAStatus status = DFAStatus::START;
    token res;
    tokenString.clear();
    while(status != DFAStatus::DONE){
        int ch = getCh();
        switch (status)
        {
        case DFAStatus::START:
        case DFAStatus::BLANK:
            switch (ch)
            {
            case ' ':
                ++charNo;
                status = DFAStatus::BLANK;
                break;
            case '\t':
                charNo = (charNo / TABDISTANCE + 1) * TABDISTANCE;
                status = DFAStatus::BLANK;
                break;
            // 可以处理多种系统下换行标志
            case '\n':
                ch = getCh();
                if(ch != '\r')
                    unGetCh(ch);
            // 此处无break
            case '\r':
                status = DFAStatus::DONE;
                charNo = 1;
                ++lineNo;
                break;
            case EOF:
                status = DFAStatus::DONE;
                res = token::NUL;
            default:
                status = DFAStatus::NONBLANK;
                break;
            }
            break;
        case DFAStatus::NONBLANK:
            if(isalpha(ch)){
                status = DFAStatus::ALNUM;
                unGetCh(ch);
                break;
            }else if(isdigit(ch)){
                status = DFAStatus::NUMBER;
                unGetCh(ch);
                break;
            }else{
                status = DFAStatus::OP;
                unGetCh(ch);
                break;
            }
            break;
        case DFAStatus::ALNUM:
            do{
                if(!isalnum(ch))
                    break;
                tokenString.append(1, ch);
            }while(ch = getCh());
            unGetCh(ch);
            if(reservedWords.find(tokenString) != reservedWords.end())
                res = reservedWords[tokenString];
            else
                res = token::IDENT;
            status = DFAStatus::DONE;
            break;
        case DFAStatus::NUMBER:
            do{
                if(!isdigit(ch))
                    break;
                tokenString.append(1, ch);
            }while(ch = getCh());
            unGetCh(ch);
            res = token::NUMBER;
            status = DFAStatus::DONE;
            break;
        case DFAStatus::OP:
            switch (ch)
            {
            case '+':
                res = token::PLUS;
                break;
            case '-':
                res = token::MINUS;
                break;
            case '*':
                res = token::TIMES;
                break;
            case '/':
                res = token::SLASH;
                break;
            case '=':
                res = token::EQL;
                break;
            case '#':
                res = token::NEQ;
                break;
            case '<':
                if((ch = getCh()) == '='){
                    res = token::LEQ;
                    break;
                }
                unGetCh(ch);
                res = token::LSS;
                break;
            case '>':
                if((ch = getCh()) == '='){
                    res = token::GEQ;
                    break;
                }
                unGetCh(ch);
                res = token::GTR;
                break;
            case ':':
                if((ch = getCh()) == '='){
                    res = token::GEQ;
                    break;
                }
                unGetCh(ch);
                res = token::ERROR;
                break;
            case '(':
                res = token::LPAREN;
                break;
            case ')':
                res = token::RPAREN;
                break;
            case ',':
                res = token::COMMA;
                break;
            case ';':
                res = token::SEMICOLON;
                break;
            case '.':
                res = token::PERIOD;
                break;
            default:
                res = token::ERROR;
                break;
            }
            status = DFAStatus::DONE;
            break;
        default:
            cerr << "Unknown lexical parser status!" << endl;
            break;
        }
    }
    return res;
}