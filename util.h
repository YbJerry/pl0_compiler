#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <utility>
#include "lexer.h"

using namespace std;

void countIdent();
void printToken();
pair<string, string> transToken(Token token);

#endif