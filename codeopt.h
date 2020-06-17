#ifndef _CODEOPT_H_
#define _CODEOPT_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include "codegen.h"

using namespace std;

class CodeOpt: public CodeGen{
public:
    CodeOpt():CodeGen(), outputOptimizedFile(fstream("outputOptimizedFile.txt", ios::out)){ };
    ~CodeOpt(){outputOptimizedFile.flush();};

    void outputOptimizedCode(){
        for(auto &it:codes){
            auto &inst = it.second;

            switch (it.first)
            {
            case TACType::OP:{
                outputOptimizedIndent(4);
                if(inst.size() == 4){
                    outputOptimizedFile << inst[3] << " := " << inst[1] << " " << inst[0] << " " << inst[2] << endl;
                }else if(inst.size() == 3){
                    outputOptimizedFile << inst[2] << " := " << inst[0] << " " << inst[1] << endl;
                }
                break;
            }
            case TACType::ASSIGN:{
                outputOptimizedIndent(4);
                outputOptimizedFile << inst[1] << " := " << inst[0] << endl;
                break;
            }
            case TACType::GOTO:{
                outputOptimizedIndent(4);
                outputOptimizedFile << "goto " << inst[0] << endl;
                break;
            }
            case TACType::IF:{
                outputOptimizedIndent(4);
                outputOptimizedFile << "if " << inst[0] << " goto " << inst[1] << endl;
                break;
            }
            case TACType::LABEL:{
                outputOptimizedIndent(2);
                outputOptimizedFile << inst[0] << ":" << endl;
                break;
            }
            default:
                break;
            }
        }
    }

    void optimizeGoto(){
        if(codes.size() < 2)
            return;

        for(auto iter = codes.begin()+1; iter != codes.end();++iter){
            auto &prevCodeType = (iter-1)->first;
            auto &prevCodeContent = (iter-1)->second;

            auto &codeType = (iter+1)->first;
            auto &codeContent = (iter+1)->second;

            if(prevCodeType == TACType::LABEL){
                if(codeType == TACType::LABEL){
                    labelReplace(*(prevCodeContent.begin()), *(codeContent.begin()));
                    codes.erase(iter-1);
                }else if(codeType == TACType::GOTO){
                    labelReplace(*(prevCodeContent.begin()), *(codeContent.begin()));
                    codes.erase(iter-1);
                }
            }
        }
    }

    void buildDAG(){
        set<int> DAGIndex;
        DAGIndex.insert(0);
        for(int i = 0; i < codes.size(); ++i){
            if(codes[i].first == TACType::LABEL){
                DAGIndex.insert(i);
            }else if(codes[i].first == TACType::IF){
                DAGIndex.insert(i+1);
            }
        }
        DAGIndex.insert(codes.size());

        auto iter1 = DAGIndex.begin();
        auto iter2 = iter1;
        ++iter2;
        for(; iter2 != DAGIndex.end(); ++iter1, ++iter2){
            DAG.push_back({*iter1, *iter2});
        }
    }

    void optimizeDAG(){
        for(auto it:DAG){
            int iBegin = it.first, iEnd = it.second;
            for(; iBegin < iEnd; ++iBegin){
                
            }
        }
    }

private:
    // replace Label l1 to l2
    void labelReplace(string l1, string l2){
        for(auto &it:codes){
            for(auto &arg:it.second){
                if(arg == l1){
                    arg = l2;
                }
            }
        }
    }

    void outputOptimizedIndent(int num){
        for(int i = 0; i < num; ++i)
            outputOptimizedFile << " ";
    }

    fstream outputOptimizedFile;
    vector<pair<int, int>> DAG;
};

#endif