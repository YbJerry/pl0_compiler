#include "symtab.h"

void Symtab::insert(string name, Kind kind, int level, int val, int addr, int size)
{
    items.push_back(Item{name, kind, level, val, addr, size});
}

Symtab::Item* Symtab::find(string name, Kind kind)
{
    auto iter = find_if(items.rbegin(), items.rend(), [&name, &kind](const Item &item){
        return (item.name == name && item.kind == kind);
    });

    if(iter == items.rend())
        return nullptr;
    return &*iter;
}

Symtab::Item* Symtab::findNonProc(string name)
{
    auto iter = find_if(items.rbegin(), items.rend(), [&name](const Item &item){
        return (item.name == name && item.kind != Kind::PROC);
    });

    if(iter == items.rend())
        return nullptr;
    return &*iter;
}

Symtab::Item* Symtab::findProc(string name)
{
    return find(name, Kind::PROC);
}

Symtab::Item* Symtab::findConst(string name)
{
    return find(name, Kind::CONST);
}

Symtab::Item* Symtab::findVar(string name)
{
    return find(name, Kind::VAR);
}

void Symtab::erase(string name)
{
    items.erase(find_if(items.rbegin(), items.rend(), [&name](const Item &item){
        return item.name == name;
    }).base());
}

void Symtab::eraseLevel(int level)
{
    items.erase(remove_if(items.begin(), items.end(), [&level](const Item &item){
        return item.level == level;
    }), items.end());
}

void Symtab::print(void)
{
    cout << setw(10) << "name"
        << setw(10) << "kind"
        << setw(10) << "level"
        << setw(10) << "val"
        << setw(10) << "addr"
        << setw(10) << "size"
        << endl;

    for(auto it:items){
        string kind;
        switch(it.kind){
        case Kind::CONST:
            kind = "CONST";
            break;
        case Kind::VAR:
            kind = "VAR";
            break;
        case Kind::PROC:
            kind = "PROC";
            break;
        }
        cout << setw(10) << it.name
            << setw(10) << kind
            << setw(10) << it.level;
        if(it.kind != Kind::CONST){
            cout << setw(10) << " "
                << setw(10) << "DX+" << it.addr;
            if(it.kind == Kind::PROC)
                cout << setw(10) << it.size;
        }else{
            cout << setw(10) << it.val;
        }
        cout << endl;
    }
}