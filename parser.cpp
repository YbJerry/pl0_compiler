#include "parser.h"

void Parser::parseProgram(){
    LLState = NoTerminal::PROG;
    matchToken(Token::NUL);
    parseSubProgram();
    matchToken(Token::PERIOD);
    cGen.genComment();
    cGen.commit();

    cGen.optimizeGoto();
    cGen.buildDAG();
    cGen.outputOptimizedCode();
}

void Parser::parseSubProgram(){
    LLState = NoTerminal::SUBPROG;
    if(token == Token::CONSTSYM){
        matchToken(Token::CONSTSYM);
        parseConstDeclaration();
        while(token == Token::COMMA){
            matchToken(Token::COMMA);
            parseConstDeclaration();
        }
        matchToken(Token::SEMICOLON);
        cGen.genComment();
    }

    if(token == Token::VARSYM){
        matchToken(Token::VARSYM);
        parseVarDeclaration();
        while(token == Token::COMMA){
            matchToken(Token::COMMA);
            parseVarDeclaration();
        }
        matchToken(Token::SEMICOLON);
        cGen.genComment();
    }

    while(token == Token::PROCSYM){
        matchToken(Token::PROCSYM);
        sTable.insert(tokenString, Symtab::Kind::PROC, level, 0, 0);
        string proc = tokenString;
        matchToken(Token::IDENT);
        cGen.genComment();
        cGen.genFuncLabel(proc);
        matchToken(Token::SEMICOLON);
        parseSubProgram();
        matchToken(Token::SEMICOLON);
        cGen.genReturn();
        cGen.genComment();
        cGen.commit();
    }

    parseStatement();
}

void Parser::parseConstDeclaration(){
    LLState = NoTerminal::CONSTDEC;
    string name = tokenString;
    matchToken(Token::IDENT);
    matchToken(Token::EQL);
    int val = stoi(tokenString);
    matchToken(Token::NUMBER);
    sTable.insert(name, Symtab::Kind::CONST, level, val);
}

void Parser::parseVarDeclaration(){
    LLState = NoTerminal::VARDEC;
    sTable.insert(tokenString, Symtab::Kind::VAR, level, 0, DXAddr++);
    matchToken(Token::IDENT);
}
     
void Parser::parseStatement(){
    LLState = NoTerminal::STATEMENT;
    switch (token){
    case Token::IDENT:{
        string temp = tokenString;
        matchToken(Token::IDENT);
        auto *p = sTable.findNonProc(temp);
        if(!p){
            cout << "\033[41;36m ERROR " << lineNo << ":" << charNo << "\33[0m" << ": Var or Const " << temp << " is not defined." << endl;
        }else if(p->kind == Symtab::Kind::CONST){
            cout << "\033[41;36m ERROR " << lineNo << ":" << charNo << "\33[0m" << ": " << temp << " is the type of Const." << endl;
        }
        matchToken(Token::BECOMES);
        parseExpression();
        cGen.genAssign(cGen.getTemp(), temp);

        cGen.genComment();
        cGen.commit();
        break;
    }
    case Token::IFSYM:{
    //  If t1 goto L1
    //  goto L2
    //  L1:
    //  ...
    //  statement
    //  ...
    //  L2:
        matchToken(Token::IFSYM);
        parseCondition();
        string L1 = cGen.getLabel();
        cGen.genIf(cGen.getTemp(), L1);
        cGen.addLabelNum();
        string L2 = cGen.getLabel();
        cGen.genGoto(L2);
        cGen.addLabelNum();
        cGen.genLabel(L1);
        matchToken(Token::THENSYM);
        cGen.genComment();
        cGen.commit();
        parseStatement();
        cGen.genLabel(L2);

        cGen.commit();
        break;
    }
    case Token::WHILESYM:{
    //  L0:
    //  If t1 goto L1
    //  goto L2
    //  L1:
    //  ...
    //  statement
    //  ...
    //  goto L0
    //  L2:
        string L0 = cGen.getLabel();
        cGen.genLabel(L0);
        cGen.addLabelNum();
        matchToken(Token::WHILESYM);
        parseCondition();
        string L1 = cGen.getLabel();
        cGen.addLabelNum();
        cGen.genIf(cGen.getTemp(), L1);
        cGen.addTempNum();
        string L2 = cGen.getLabel();
        cGen.addLabelNum();
        cGen.genGoto(L2);
        cGen.genLabel(L1);
        matchToken(Token::DOSYM);
        parseStatement();
        cGen.genComment();
        cGen.commit();
        cGen.genGoto(L0);
        cGen.genLabel(L2);
        
        cGen.commit();
        break;
    }
    case Token::CALLSYM:{
        matchToken(Token::CALLSYM);
        string temp = tokenString;
        matchToken(Token::IDENT);
        if(!sTable.findProc(temp)){
            cout << "\033[41;36m ERROR " << lineNo << ":" << charNo << "\33[0m" << ": Proc " << temp << " is not defined." << endl;
        }
        cGen.genCall(temp);
        cGen.genComment();
        cGen.commit();
        break;
    }
    case Token::READSYM:{
        matchToken(Token::READSYM);
        matchToken(Token::LPAREN);
        string temp = tokenString;
        matchToken(Token::IDENT);
        if(!sTable.findNonProc(temp)){
            cout << "\033[41;36m ERROR " << lineNo << ":" << charNo << "\33[0m" << ": Var or Const " << temp << " is not defined." << endl;
        }
        cGen.genRead(temp);
        while(token == Token::COMMA){
            matchToken(Token::COMMA);
            temp = tokenString;
            matchToken(Token::IDENT);
            if(!sTable.findNonProc(temp)){
                cout << "\033[41;36m ERROR " << lineNo << ":" << charNo << "\33[0m" << ": Var or Const " << temp << " is not defined." << endl;
            }
            cGen.genRead(temp);
        }
        matchToken(Token::RPAREN);
        cGen.genComment();
        cGen.commit();
        break;
    }
    case Token::WRITESYM:
        matchToken(Token::WRITESYM);
        matchToken(Token::LPAREN);
        parseExpression();
        cGen.genWrite(cGen.getTemp());
        cGen.addTempNum();
        while(token == Token::COMMA){
            matchToken(Token::COMMA);
            parseExpression();
            cGen.genWrite(cGen.getTemp());
            cGen.addTempNum();
        }
        matchToken(Token::RPAREN);

        cGen.genComment();
        cGen.commit();
        break;
    case Token::BEGINSYM:
        matchToken(Token::BEGINSYM);
        cGen.genComment();
        ++level;
        DXAddr = 0;
        parseStatement();
        while(token == Token::SEMICOLON){
            matchToken(Token::SEMICOLON);
            parseStatement();
        }
        --level;
        sTable.print();
        matchToken(Token::ENDSYM);
        // cGen.genComment();
        break;
    // if statement is empty, its follow set is . and ; and end
    case Token::ENDSYM:
    case Token::SEMICOLON:
    case Token::PERIOD:
        break;
    default:
        matchToken(Token::ERROR);
        break;
    }
}

void Parser::parseCondition(){
    string arg1, arg2, op, out;
    LLState = NoTerminal::CONDITION;
    switch(token){
    case Token::ODDSYM:
        matchToken(Token::ODDSYM);
        parseExpression();
        arg1 = cGen.getTemp();
        cGen.addTempNum();
        cGen.genOp("odd", arg1, cGen.getTemp());
        break;
    // condition's first set
    case Token::PLUS:
    case Token::MINUS:
    case Token::LPAREN:
    case Token::NUMBER:
    case Token::IDENT:
        parseExpression();
        arg1 = cGen.getTemp();
        cGen.addTempNum();
        switch (token)
        {
        case Token::EQL:
        case Token::NEQ:
        case Token::LSS:
        case Token::LEQ:
        case Token::GTR:
        case Token::GEQ:
            op = transToken(token).second;
            matchToken(token);
            break;
        default:
            matchToken(Token::ERROR);
            break;
        }
        parseExpression();
        arg2 = cGen.getTemp();
        cGen.addTempNum();
        cGen.genOp(op, arg1, arg2, cGen.getTemp());
        break;
    default:
        matchToken(Token::ERROR);
        break;
    }
}

void Parser::parseExpression(){
    // - a + b + c - d
    // t1 := -a
    // t2 := t1 + b
    // t3 := t2 + c
    // t4 := t3 - d

    string op, arg1, arg2, out;
    LLState = NoTerminal::EXPRESSION;

    if(token == Token::MINUS){
        matchToken(token);
        parseTerm();
        arg1 = cGen.getTemp();
        cGen.addTempNum();
        out = cGen.getTemp();
        cGen.genOp("-", arg1, out);
        arg1 = out;
    }else if(token == Token::PLUS){
        matchToken(token);
        parseTerm();
        arg1 = cGen.getTemp();
    }else{
        parseTerm();
        arg1 = cGen.getTemp();
    }
    
    while(token == Token::PLUS || token == Token::MINUS){
        op = transToken(token).second;
        matchToken(token);
        parseTerm();
        arg2 = cGen.getTemp();
        cGen.addTempNum();
        out = cGen.getTemp();
        cGen.genOp(op, arg1, arg2, out);
        arg1 = out;
    }
}

void Parser::parseTerm(){
    bool isSingle = true;
    string op, arg1, arg2, out;
    LLState = NoTerminal::TERM;
    parseFactor();
    arg1 = cGen.getTemp();
    while(token == Token::TIMES || token == Token::SLASH){
        op = transToken(token).second;
        isSingle = false;
        matchToken(token);
        parseFactor();
        arg2 = cGen.getTemp();
        cGen.addTempNum();
        out = cGen.getTemp();
        cGen.genOp(op, arg1, arg2, out);
        arg1 = out;
    }
}

void Parser::parseFactor(){
    LLState = NoTerminal::FACTOR;
    switch (token)
    {
    case Token::IDENT:{
        string temp = tokenString;
        matchToken(token);
        if(!sTable.findNonProc(temp)){
            cout << "\033[41;36m ERROR " << lineNo << ":" << charNo << "\33[0m" << ": Var or Const " << temp << " is not defined." << endl;
        }
        cGen.addTempNum();
        cGen.genAssign(temp, cGen.getTemp());
        // cGen.genComment();
        // cGen.commit();
        break;
    }
    case Token::NUMBER:{
        string temp = tokenString;
        matchToken(token);
        cGen.addTempNum();
        cGen.genAssign(temp, cGen.getTemp());
        // cGen.genComment();
        // cGen.commit();
        break;
    }
    case Token::LPAREN:{
        matchToken(Token::LPAREN);
        parseExpression();
        matchToken(Token::RPAREN);
        string arg = cGen.getTemp();
        // cGen.addTempNum();
        // cGen.genAssign(arg, cGen.getTemp());
        // cGen.genComment();
        // cGen.commit();
        break;
    }
    default:
        matchToken(Token::ERROR);
        break;
    }
}

bool Parser::matchToken(const Token &token){
    if(token != this->token){
        cout << "\033[41;36m ERROR " <<lineNo << ":" << charNo << "\33[0m" << ": expect a \"" << transToken(token).second << "\"" << endl;
        errorRecovery(LLState);
        if(token == this->token){
            this->token = getToken();
        }
        return false;
    }
    auto prev = this->token;
    if(token == Token::IDENT || token == Token::NUMBER)
        cGen.putComment(tokenString);
    else if(token != Token::NUL){
        cGen.putComment(transToken(token).second);
    }
    this->token = getToken();
    if(prev == Token::PERIOD && this->token == Token::NUL){
        return true;
    }
    if(this->token == Token::NUL){
        cout << "\033[41;36m ERROR " <<lineNo << ":" << charNo << "\33[0m" << ": code ends without '.'" << endl;
        this->token = Token::PERIOD;
        return false;
    }
    if(this->token == Token::ERROR){
        cout << "\033[41;36m ERROR " <<lineNo << ":" << charNo << "\33[0m" << ": an unexpected token" << endl;
        do{
            this->token = getToken();
        }while(this->token == Token::ERROR);
        return false;
    }
    return true;
}

void Parser::errorRecovery(const NoTerminal &nt){
    const vector<Token> &followSet = followSets.at(nt);
    do{
        token = getToken();
    } while (find(followSet.begin(), followSet.end(), token) == followSet.end());
}