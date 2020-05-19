#ifndef _SYMTAB_H_
#define _SYMTAB_H_

#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

class Symtab{
    friend class Parser;

    enum class Kind{
        CONST, VAR, PROC
    };

    struct Item{
        string name;
        Kind kind;
        int level;
        int val;
        int addr;
        int size;
    };
public:
    Symtab(){};
    ~Symtab(){};

    void insert(string name, Kind kind, int level, int val = 0, int addr = 0, int size = 0);
    Item* findNonProc(string name);
    Item* findProc(string name);
    Item* findConst(string name);
    Item* findVar(string name);
    void erase(string name);
    void eraseLevel(int level);
    void print();

private:
    Item* find(string name, Kind kind);
    vector<Item> items;
};

#endif