#include <iostream>
#include <fstream>
#include <sstream>
#include "globals.h"
#include "lexer.h"
#include "parser.h"

int main(int argc, char **argv){
    ifstream fin;
    stringstream sharedIO;
    Parser *parser;

    if(argc > 1){
        fin.open(argv[1]);
        if(!fin){
            cerr << "Target filename don't exist!" << endl;
            return 0;
        }
        parser = new Parser(fin);
    }else{
        parser = new Parser();
    }
    parser->parseProgram();
    return 0;
}