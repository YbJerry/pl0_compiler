#include <iostream>
#include <fstream>
#include "globals.h"
#include "lexer.h"
#include "util.h"

ifstream fin;

int main(int argc, char **argv){
    if(argc > 1){
        fin.open(argv[1]);
        if(!fin){
            cerr << "Target filename don't exist!" << endl;
            return 0;
        }
    }

    countIdent();

    return 0;
}