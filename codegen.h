#ifndef _CODEGEN_H_
#define _CODEGEN_H_

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class CodeGen{
public:
    enum Kind{
        IF, RETURN, CALL, 
    };

    CodeGen():outputFile(fstream("outputCode.txt", ios::out)){ };
    ~CodeGen(){outputFile.flush();};

    void genOp(string op, string arg1, string arg2, string out){
        commitIndent(4);
        buffer << out << " := " << arg1 << " " << op << " " << arg2 << endl;
    }

    void genOp(string op, string arg, string out){
        commitIndent(4);
        buffer << out << " := " << op << " " << arg << endl;
    }

    void genAssign(string arg, string out){
        commitIndent(4);
        buffer << out << " := " << arg << endl;
    }

    void genGoto(string label){
        commitIndent(4);
        buffer << "goto " << label << endl;
    }

    void genIf(string arg, string label){
        commitIndent(4);
        buffer << "if " << arg << " goto " << label << endl;
    }

    void genLabel(string label){
        commitIndent(2);
        buffer << label << ":" << endl;
    }

    void genCall(string func){
        commitIndent(4);
        buffer << "call __" << func << endl;
    }

    void genReturn(){
        commitIndent(4);
        buffer << "return" << endl;
    }

    void genRead(string arg){
        commitIndent(4);
        buffer << "read " << arg << endl;
    }

    void genWrite(string arg){
        commitIndent(4);
        buffer << "write " << arg << endl;
    }

    void genFuncLabel(string func){
        commitIndent(2);
        buffer << "__" << func << ":" << endl;
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

private:
    void commitIndent(int n){
        for(int i = 0; i < n; ++i){
            buffer << " ";
        }
    }

    string comment;
    stringstream buffer;
    fstream outputFile;
    int labelNum = 0;
    int tempNum = 0;
};

#endif