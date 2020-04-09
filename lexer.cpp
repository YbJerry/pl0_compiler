#include "lexer.h"

string tokenString;
int lineNo = 1, charNo = 1;
extern ifstream fin;
int commentLevel = 0;

// 有限自动机状态
enum class DFAStatus{
    DONE = 0, START, BLANK, NEWLINE, NONBLANK,
    NUMBER, ALNUM, OP, COMMENT
};

// 保留关键字
static unordered_map<string, Token> reservedWords = {
    {"begin",   Token::BEGINSYM},
    {"end",     Token::ENDSYM},
    {"if",      Token::IFSYM},
    {"then",    Token::THENSYM},
    {"while",   Token::WHILESYM},
    {"write",   Token::WRITESYM},
    {"read",    Token::READSYM},
    {"do",      Token::DOSYM},
    {"call",    Token::CALLSYM},
    {"const",   Token::CONSTSYM},
    {"var",     Token::VARSYM},
    {"proc",    Token::PROCSYM},
};

char getCh(){
    return cin.get();
}

void unGetCh(char ch){
    cin.unget();
}

Token getToken(){
    if(fin){
        cin.rdbuf(fin.rdbuf());
    }
    DFAStatus status = DFAStatus::START;
    Token res;
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
            case '\r':
                ch = getCh();
                if(ch != '\n')
                    unGetCh(ch);
            // 此处无break
            case '\n':
                status = DFAStatus::DONE;
                charNo = 1;
                ++lineNo;
                break;
            case EOF:
                status = DFAStatus::DONE;
                res = Token::NUL;
                break;
            default:
                unGetCh(ch);
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
                tokenString.append(1, tolower(ch));
            }while(ch = getCh());
            unGetCh(ch);
            if(reservedWords.find(tokenString) != reservedWords.end())
                res = reservedWords[tokenString];
            else
                res = Token::IDENT;
            status = DFAStatus::DONE;
            break;
        case DFAStatus::NUMBER:
            do{
                if(!isdigit(ch))
                    break;
                tokenString.append(1, ch);
            }while(ch = getCh());
            unGetCh(ch);
            res = Token::NUMBER;
            status = DFAStatus::DONE;
            break;
        case DFAStatus::OP:
            switch (ch)
            {
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
                if((ch = getCh()) == '='){
                    res = Token::LEQ;
                    break;
                }
                unGetCh(ch);
                res = Token::LSS;
                break;
            case '>':
                if((ch = getCh()) == '='){
                    res = Token::GEQ;
                    break;
                }
                unGetCh(ch);
                res = Token::GTR;
                break;
            case ':':
                if((ch = getCh()) == '='){
                    res = Token::GEQ;
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
            status = DFAStatus::DONE;
            if(ch == '{'){
                ++commentLevel;
                status = DFAStatus::COMMENT;
            }
            break;
        case DFAStatus::COMMENT:
            do{
                if(ch == '}'){
                    --commentLevel;
                }else if(ch == '{'){
                    ++commentLevel;
                }
                ch = getCh();
            }while(commentLevel);
            unGetCh(ch);
            status = DFAStatus::START;
            break;
        default:
            cerr << "Unknown lexical parser status!" << endl;
            break;
        }
    }
    return res;
}