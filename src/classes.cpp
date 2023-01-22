//
// Created by Maciek Józefkowicz on 15.01.2023.
//
#include <iostream>
#include <utility>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;

enum LogicOp{
    EQ_enum,
    UNEQ_enum,
    LTEQ_enum,
    LT_enum,
    GT_enum,
    GTEQ_enum
};

enum MathOp{
    PLUS_enum,
    MINUS_enum,
    TIMES_enum,
    DIV_enum,
    MOD_enum
};

class Variable{
public:
    bool is_const = false;
    string identifier;
    string range = "undef";
};

class Command{
public:
    virtual string debug_str() = 0;
    virtual ~Command() = default;
    //jakieś funkcje, które będą charakteryzować wszystkie komedny
    // może np. to_assembly albo coś takiego?
};

class Condition{
public:
    LogicOp l_op;
    Variable left;
    Variable right;

    Condition(){
        l_op = EQ_enum;
    }
    Condition(LogicOp op, Variable l, Variable r) : l_op(op), left(std::move(l)), right(std::move(r)) {}
};

class Procedure_obj{
public:
  string identifier;
  vector<Command> body;

  explicit Procedure_obj(string ident) : identifier(std::move(ident)) {}


};

class Procedure_ins : public Command{
public:
    Procedure_obj* proc;

    explicit Procedure_ins(Procedure_obj* p) : proc(p) {};

    string debug_str() override{
        return "";
    }
};

class While : public Command{
public:
    Condition cond;
    vector<Command> body;

    string debug_str() override {
        return "";
    }
};

class Repeat : public Command{
public:
    Condition cond;
    vector<Command> body;

    string debug_str() override {
        return "";
    }
};

class If_exp : public Command{
public:
    Condition cond;
    vector<Command> body;

    string debug_str() override {
        return "";
    }
};

class If_else : public Command{
public:
    Condition cond;
    vector<Command> if_body;
    vector<Command> else_body;

    string debug_str() override {
        return "";
    }
};

class Read : public Command{
public:
    Variable var;

    string debug_str() override {
        return "";
    }
};

class Write : public Command{
public:
    Variable var;

    string debug_str() override {
        return "";
    }
};

class Expression {
public:
    Variable var1, var2;
    MathOp math_op;
};


class Set_exp : public Command{
public:
    Variable var;
    Expression exp;

    string debug_str() override {
        return "";
    }
};


