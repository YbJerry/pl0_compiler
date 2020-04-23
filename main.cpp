#include <iostream>
#include <fstream>
#include "globals.h"
#include "lexer.h"
#include "util.h"

ifstream fin;

int main(int argc, char **argv){
    Lexer *lexer;
    if(argc > 1){
        fin.open(argv[1]);
        if(!fin){
            cerr << "Target filename don't exist!" << endl;
            return 0;
        }
        lexer = new Lexer(fin);
    }else{
        lexer = new Lexer();
    }

    lexer->printToken();

    return 0;
}