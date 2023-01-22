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
    bool external = false;
    string identifier;
    string range = "undef";

//    Variable() = default;
//
//    Variable(const Variable& other){
//        is_const = other.is_const;
//        external = other.external;
//        identifier = other.identifier;
//        range = other.range;
//    }
//
//    Variable(Variable&& other) noexcept {
//        is_const = other.is_const;
//        external = other.external;
//        identifier = other.identifier;
//        range = other.range;
//    }
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
    Variable* left;
    Variable* right;

    Condition(){
        l_op = EQ_enum;
        left = right = nullptr;
    }
    Condition(LogicOp op, Variable* l, Variable* r) : l_op(op), left(l), right(r) {}
};

class Procedure_obj{
public:
    string identifier;
    vector<Command*> body;

    explicit Procedure_obj(string ident) : identifier(std::move(ident)) {}

    ~Procedure_obj() {
      for (auto x: body) {
          delete x;
      }
    }
};

class Procedure_ins : public Command{
public:
    Procedure_obj* proc;

    explicit Procedure_ins(Procedure_obj* p) : proc(p) {};

    vector<Variable*> vars_to_use;

    string debug_str() override{
        return "";
    }
};

class While : public Command{
public:
    Condition cond;
    vector<Command*> body;

    string debug_str() override {
        return "";
    }

    ~While() override{
        for(auto x : body){
            delete x;
        }
    }
};

class Repeat : public Command{
public:
    Condition cond;
    vector<Command*> body;

    string debug_str() override {
        return "";
    }

    ~Repeat() override{
        for(auto x : body){
            delete x;
        }
    }
};

class If_exp : public Command{
public:
    Condition cond;
    vector<Command*> body;

    string debug_str() override {
        return "";
    }

    ~If_exp() override{
        for(auto x : body){
            delete x;
        }
    }
};

class If_else : public Command{
public:
    Condition cond;
    vector<Command*> if_body;
    vector<Command*> else_body;

    string debug_str() override {
        return "";
    }

    ~If_else() override{
        for(auto x : if_body){
            delete x;
        }
        for(auto x : else_body){
            delete x;
        }
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


