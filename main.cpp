#include <iostream>
#include <fstream>
#include "globals.h"
#include "lexer.h"

int main(int argc, char **argv){
    if(argc > 1){
        ifstream fin(argv[1]);
        if(fin)
            cin.rdbuf(ifstream(argv[1]).rdbuf());
        else{
            cerr << "Target filename don't exist!" << endl;
            return 0;
        }
    }

    
    
    return 0;
}