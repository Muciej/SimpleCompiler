//
// Created by Maciek Józefkowicz on 10.01.2023.
//

#include <iostream>
#include <fstream>
#include "classes.cpp"

using std::string;
using std::endl;

class Logic{
public:
    std::ofstream outFile;
    std::ofstream debugFile;
    bool debug = false;

    void close(){
        outFile.close();
    }

    void println_debug(const string& mess){
        if(debug) debugFile << mess << endl;
    }

    void print_debug(const string& mess){
        if(debug) debugFile << mess;
    }

    void print_debug(const int& num){
        if(debug) debugFile << num;
    }
};