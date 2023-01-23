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

enum InitStatus{
    NOT_INITIALIZED,
    INITIALIZED,
    AMBIGIOUS
};

class Variable{
public:
    bool is_const = false;
    bool external = false;
    string identifier;
    string range = "undef";
    InitStatus init_status = NOT_INITIALIZED;

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
    virtual void debug_print(std::ostream &out) = 0;
    virtual ~Command() = default;
    //jakieś funkcje, które będą charakteryzować wszystkie komedny
    // może np. to_assembly albo coś takiego?
};

class Condition{
public:
    string l_op;
    Variable* left;
    Variable* right;
    vector<Command*>* container = nullptr;

    Condition(){
        l_op = "==";
        left = right = nullptr;
    }
    Condition(string op, Variable* l, Variable* r) : l_op(op), left(l), right(r) {}

    [[nodiscard]] string to_str() const{
        string str;
        str += left->identifier;
        str += l_op;
        str += right->identifier;
        return str;
    }
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

    void debug_print(std::ostream &out) override{
        out<<proc->identifier<<endl;
        for(auto c : proc->body)
            c->debug_print(out);
    }
};

class While : public Command{
public:
    Condition* cond;
    vector<Command*>* body;

    void debug_print(std::ostream &out) override {
    out<<"While ( "<<cond->to_str()<<" )"<<endl;
        for (auto c : *body){
            c->debug_print(out);
        }
    }

    ~While() override{
        for(auto x : *body){
            delete x;
        }
    }
};

class Repeat : public Command{
public:
    Condition* cond;
    vector<Command*>* body;

    void debug_print(std::ostream &out) override {
        out<<"Do:"<<endl;
        for (auto c : *body){
            c->debug_print(out);
        }
        out<<"Until  ( "<<cond->to_str()<<" )"<<endl;
    }

    ~Repeat() override{
        for(auto x : *body){
            delete x;
        }
    }
};

class If_exp : public Command{
public:
    Condition* cond;
    vector<Command*>* body;

    void debug_print(std::ostream &out) override {
        out<<"If ( "<<cond->to_str()<<" ) then:"<<endl;
        for (auto c : *body){
            c->debug_print(out);
        }
        delete body;
    }

    ~If_exp() override{
        for(auto x : *body){
            delete x;
        }
        delete body;
    }
};

class If_else : public Command{
public:
    Condition* cond;
    vector<Command*>* if_body;
    vector<Command*>* else_body;

    void debug_print(std::ostream &out) override {
        out<<"If ( "<<cond->to_str()<<" ) then:"<<endl;
        for (auto c : *if_body){
            c->debug_print(out);
        }
        out<<"else:"<<endl;
        for (auto c : *else_body){
            c->debug_print(out);
        }
    }

    ~If_else() override{
        for(auto x : *if_body){
            delete x;
        }
        delete if_body;
        for(auto x : *else_body){
            delete x;
        }
        delete else_body;
    }
};

class Read : public Command{
public:
    Variable* var;

    explicit Read(Variable* v) : var(v) {}

    void debug_print(std::ostream &out) override {
        out<<"Read: "<<var->identifier<<endl;
    }
};

class Write : public Command{
public:
    Variable* var;

    explicit Write(Variable* v) : var(v) {}

    void debug_print(std::ostream &out) override {
        out<<"Write: "<<var->identifier<<endl;
    }
};

class Expression {
public:
    Variable* left = nullptr;
    Variable* right = nullptr;
    string math_op;

    Expression() = default;

    Expression(string op, Variable* l, Variable* r) : math_op(op), left(l), right(r) {}

    [[nodiscard]] string to_str() const{
        string s;
        s += left->identifier;
        if(math_op != "just_var") {
            s += math_op;
            s += right->identifier;
        }
        return s;
    }
};


class Set_exp : public Command{
public:
    Variable* var;
    Expression* exp;

    void debug_print(std::ostream &out) override {
        out<<"Set "<<var->identifier<< " to "<<exp->to_str()<<endl;
    }
};
