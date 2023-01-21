//
// Created by Maciek Józefkowicz on 15.01.2023.
//
#include <iostream>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;

class Variable{
    bool is_const;
    string identifier;
};

class Command{
    virtual string debug_str() = 0;
    //jakieś funkcje, które będą charakteryzować wszystkie komedny
    // może np. to_assembly albo coś takiego?
};

class Condition{
    string logic_operator;
};

class Procedure : public Command{
  string identifier;
  Condition cond;
  vector<Command> body;

  string debug_str() override {

  }
};

class While : public Command{
    Condition cond;
    vector<Command> body;
};

class Repeat : public Command{
    Condition cond;
    vector<Command> body;
};

class If_exp : public Command{
    Condition cond;
    vector<Command> body;
};

class If_else : public Command{
    Condition cond;
    vector<Command> if_body;
    vector<Command> else_body;
};

class Read : public Command{
    Variable var;
};

class Write : public Command{
    Variable var;
};

class Expression {
    Variable var1, var2;
    string math_operator;
};


class Set_exp : public Command{
    Variable var;
    Expression exp;
};


