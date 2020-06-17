#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <sstream>

using namespace std;

class CodeGen{
public:
    enum Kind{
        IF, RETURN, CALL, 
    };

    enum class TACType{
        OP, ASSIGN, GOTO, IF, LABEL, CALL, RETURN, READ, WRITE, FUNCLABEL
    };

    CodeGen():outputFile(fstream("outputCode.txt", ios::out)){ };
    ~CodeGen(){outputFile.flush();};

    void genOp(string op, string arg1, string arg2, string out){
        commitIndent(4);
        buffer << out << " := " << arg1 << " " << op << " " << arg2 << endl;
        codes.push_back(make_pair(TACType::OP, vector<string>{op, arg1, arg2, out}));
    }

    void genOp(string op, string arg, string out){
        commitIndent(4);
        buffer << out << " := " << op << " " << arg << endl;
        codes.push_back(make_pair(TACType::OP, vector<string>{op, arg, out}));
    }

    void genAssign(string arg, string out){
        commitIndent(4);
        buffer << out << " := " << arg << endl;
        codes.push_back(make_pair(TACType::ASSIGN, vector<string>{arg, out}));
    }

    void genGoto(string label){
        commitIndent(4);
        buffer << "goto " << label << endl;
        codes.push_back(make_pair(TACType::GOTO, vector<string>{label}));
    }

    void genIf(string arg, string label){
        commitIndent(4);
        buffer << "if " << arg << " goto " << label << endl;
        codes.push_back(make_pair(TACType::IF, vector<string>{arg, label}));
    }

    void genLabel(string label){
        commitIndent(2);
        buffer << label << ":" << endl;
        codes.push_back(make_pair(TACType::LABEL, vector<string>{label}));
    }

    void genCall(string func){
        commitIndent(4);
        buffer << "call __" << func << endl;
        codes.push_back(make_pair(TACType::CALL, vector<string>{func}));
    }

    void genReturn(){
        commitIndent(4);
        buffer << "return" << endl;
        codes.push_back(make_pair(TACType::RETURN, vector<string>{}));
    }

    void genRead(string arg){
        commitIndent(4);
        buffer << "read " << arg << endl;
        codes.push_back(make_pair(TACType::READ, vector<string>{arg}));
    }

    void genWrite(string arg){
        commitIndent(4);
        buffer << "write " << arg << endl;
        codes.push_back(make_pair(TACType::WRITE, vector<string>{arg}));
    }

    void genFuncLabel(string func){
        commitIndent(2);
        buffer << "__" << func << ":" << endl;
        codes.push_back(make_pair(TACType::FUNCLABEL, vector<string>{func}));
    }

    void genComment(){
        outputFile << "; " << comment << endl;
        comment.clear();
    }

    void commit(){
        string str;
        outputFile << buffer.str();
        buffer.clear();
        buffer.str("");
    }

    void putComment(const string &comment){
        this->comment += comment + " ";
    }

    void addLabelNum(){
        ++labelNum;
    }

    void addTempNum(){
        ++tempNum;
    }

    int getLabelNum(){
        return labelNum;
    }

    string getLabel(){
        string t = "_L";
        t += to_string(labelNum);
        return t;
    }

    int getTempNum(){
        return tempNum;
    }

    string getTemp(){
        string t = "_t";
        t += to_string(tempNum);
        return t;
    }

protected:
    void commitIndent(int n){
        for(int i = 0; i < n; ++i){
            buffer << " ";
        }
    }

    vector<pair<TACType, vector<string>>> codes;

    array<int ,3> a;

    string comment;
    stringstream buffer;
    fstream outputFile;
    int labelNum = 0;
    int tempNum = 0;
};

#endif