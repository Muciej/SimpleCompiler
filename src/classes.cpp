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

#define ACC 0

class AssemblyIns{
public:
    string ins;
    long long cell;

    AssemblyIns(string i, long long c) : ins(i), cell(c) {}

    [[nodiscard]] string to_str() const{
        string s = ins;
        s += " ";
        s += std::to_string(cell);
        return s;
    }
};

class AssemblerCode {
public:
    int order_pos = 0;

    vector<AssemblyIns> orders;

    void add_order(string ins, long long cell){
        AssemblyIns order{ins, cell};
        order_pos++;
    }

    void add_order(AssemblyIns ins){
        orders.push_back(ins);
        order_pos++;
    }

    int add_part_order(string ins){
        AssemblyIns order{ins, -1};
        orders.push_back(order);
        order_pos++;
        return order_pos-1;
    }

    void set_part_order(int pos, int to_set){
        orders.at(pos).cell = to_set;
    }

    [[nodiscard]] int get_order_pos() const{
        return order_pos;
    }

    void print_out_file(std::ostream &out){
        for (const auto& ins : orders){
            out<<ins.to_str()<<endl;
        }
    }
};

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
    long long const_val;
    int addr = -1;

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
    virtual void get_assembly_code(AssemblerCode* coder) = 0;
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
    vector<AssemblyIns> assembler_ins;

    Condition(){
        l_op = "==";
        left = right = nullptr;
    }
    Condition(string op, Variable* l, Variable* r) : l_op(op), left(l), right(r) {}

    void to_assembler(){
        //TODO

    }

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
    long long start_address = -1;
    long long jump_cell_address = -1;

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

    void get_assembly_code(AssemblerCode* coder) override{

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

    void get_assembly_code(AssemblerCode* coder) override{
        int start_ord_no = coder->get_order_pos();
        cond->to_assembler();
        for( const auto& ins : cond->assembler_ins){
            coder->add_order(ins);
        }
        int jump_pos = coder->add_part_order("JPOS");
        for (auto comm : *body){
            comm->get_assembly_code(coder);
        }
        coder->add_order("JUMP", start_ord_no);
        coder->set_part_order(jump_pos, coder->get_order_pos());
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

    void get_assembly_code(AssemblerCode* coder) override{
        int start_ord_pos = coder->get_order_pos();
        for (auto comm : *body){
            comm->get_assembly_code(coder);
        }
        cond->to_assembler();
        for(const auto& ins : cond->assembler_ins){
            coder->add_order(ins);
        }
        coder->add_order("JZERO", start_ord_pos);
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

    void get_assembly_code(AssemblerCode* coder) override{
        cond->to_assembler();
        for(const auto& ins : cond->assembler_ins){
            coder->add_order(ins);
        }
        int jump_pos = coder->add_part_order("JPOS");
        for(const auto& ins : *body){
            ins->get_assembly_code(coder);
        }
        coder->set_part_order(jump_pos, coder->order_pos);
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

    void get_assembly_code(AssemblerCode* coder) override{
        cond->to_assembler();
        for(const auto& ins : cond->assembler_ins){
            coder->add_order(ins);
        }
        int else_jump_pos = coder->add_part_order("JPOS");
        for(const auto& ins : *if_body){
            ins->get_assembly_code(coder);
        }
        int after_if_jump_pos = coder->add_part_order("JUMP");
        coder->set_part_order(else_jump_pos, coder->get_order_pos());
        for(const auto& ins : *else_body){
            ins->get_assembly_code(coder);
        }
        coder->set_part_order(after_if_jump_pos, coder->get_order_pos());
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

    void get_assembly_code(AssemblerCode* coder) override{
        if(var->external){
            coder->add_order("GET", ACC);
            coder->add_order("STOREI", var->addr);
        } else {
            coder->add_order("GET", var->addr);
        }
    }
};

class Write : public Command{
public:
    Variable* var;

    explicit Write(Variable* v) : var(v) {}

    void debug_print(std::ostream &out) override {
        out<<"Write: "<<var->identifier<<endl;
    }

    void get_assembly_code(AssemblerCode* coder) override{
        if(var->external){
            coder->add_order("LOADI", var->addr);
            coder->add_order("PUT", ACC);
        } else {
            coder->add_order("PUT", var->addr);
        }
    }
};

class Expression {
public:
    Variable* left = nullptr;
    Variable* right = nullptr;
    string math_op;
    vector<AssemblyIns> assembler_ins;

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

    void to_assembler(){

    }
};


class Set_exp : public Command{
public:
    Variable* var;
    Expression* exp;

    void debug_print(std::ostream &out) override {
        out<<"Set "<<var->identifier<< " to "<<exp->to_str()<<endl;
    }

    void get_assembly_code(AssemblerCode* coder) override{
        exp->to_assembler();
        for(const auto& ins : exp->assembler_ins){
            coder->add_order(ins);
        }
        if(var->external){
            coder->add_order("STOREI", var->addr);
        } else {
            coder->add_order("STORE", var->addr);
        }
    }
};
