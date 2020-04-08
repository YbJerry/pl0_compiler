#include "util.h"

extern string tokenString;

void countIdent(){
    unordered_map<string, int> identSum;
    Token token;
    while((token = getToken()) != Token::NUL){
        if(token == Token::IDENT){
            if(identSum.find(tokenString) != identSum.end()){
                ++identSum[tokenString];
            }else{
                identSum[tokenString] = 1;
            }
        }
    }

    for(auto m:identSum){
        cout << "(" << m.first << ": " << m.second << ")" <<endl;
    }
}