//
// Created by Maciek Józefkowicz on 15.01.2023.
//
#include <iostream>
#include <utility>
#include <vector>
#include <string>

class Variable{
public:
    std::string identifier;
    int rel_add_offset = -1;
    int owner_id;
    bool initialized = false;
    Variable(std::string ident, int offset, int own_id) : identifier(std::move(ident)), rel_add_offset(offset), owner_id(own_id) {}
    explicit Variable(std::string ident) : identifier(std::move(ident)) {
        owner_id = 0;
        rel_add_offset = -1;
        initialized = false;
    }

    void setInit(bool state){
        initialized = state;
    }
};

class Procedure{
    int id = -1;
    int varCount = 0;

};

class Instruction{
    virtual void print(std::ofstream out) = 0;
};

class While_loop : Instruction{
    //condition
    //instructions
};


class BasicBlock : public Instruction {
    //instructions
    //exitBlocks
};
