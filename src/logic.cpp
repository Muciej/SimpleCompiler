//
// Created by Maciek Józefkowicz on 10.01.2023.
//

#include <iostream>
#include <fstream>
#include <stack>
#include <utility>
#include "classes.cpp"

using std::string;
using std::endl;

class Logic{
public:
    std::ofstream outFile;
    std::stack<string> identStack;
    std::vector<Variable> declared;

    void handleVarDeclaration(const string& ident){
        Variable var{ident};
        declared.push_back(var);
        identStack.push(ident);
    }

    void handleProcHead(const string& ident){
        std::string head = ident;
        while(!identStack.empty()){
//            head identStack.pop();
        }
    }
};