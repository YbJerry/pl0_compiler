#ifndef _CODEOPT_H_
#define _CODEOPT_H_

#include <iostream>
#include <fstream>
#include <sstream>
#include "codegen.h"

using namespace std;

class CodeOpt: public CodeGen{
public:
    CodeOpt():CodeGen(), outputOptimizedFile(fstream("outputOptimizedFile.txt", ios::out)){ };
    ~CodeOpt(){outputOptimizedFile.flush();};

    void optimizeGoto(){
        if(codes.size() < 2)
            return;

        for(auto iter = codes.begin(); iter != codes.end()-1; ++iter){
            auto &codeType = iter->first;
            auto &codeContent = iter->second;

            auto &nextCodeType = iter->first;
            auto &nextCodeContent = iter->second;

            if(codeType == TACType::LABEL){
                if(nextCodeType == TACType::LABEL){
                    
                }
            }
        }
    }

private:
    fstream outputOptimizedFile;
};

#endif