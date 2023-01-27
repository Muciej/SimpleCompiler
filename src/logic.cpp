//
// Created by Maciek Józefkowicz on 10.01.2023.
//

#include <iostream>
#include <fstream>
#include <stack>
#include "classes.cpp"

void yyerror(char const *s);

using std::string;
using std::endl;
using std::vector;
using std::stack;

class Logic{
public:
    std::ofstream outFile;
    std::ofstream debugFile;
    bool debug = false;

    //flags
    Procedure_obj* curr_procedure = nullptr;
    bool in_def = false;

    //help variables
    int const_count = 0;

    //program structures
    vector<Procedure_obj*> defined_procedures;
    vector<Variable*> declared_variables;
    stack<Variable*> var_stack;
    stack<vector<Command*>*> comm_containers;
    stack<Condition*> cond_stack;
    Expression* curr_exp = nullptr;

    //containers
//    vector<Procedure_ins*> p_ins;
//    vector<While*> while_ins;
//    vector<Repeat*> rep_ins;
//    vector<If_exp*> if_ins;
//    vector<If_else*> if_else_ins;
//    vector<Read*> read_ins;
//    vector<Write*> write_ins;
//    vector<Set_exp*> set_ins;
//    vector<Condition*> conditions;

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

    Procedure_obj* proc_check(const string& func_name){
        Procedure_obj* proc = nullptr;
        for (Procedure_obj* p: defined_procedures){
            if(p->identifier == func_name) proc = p;
        }
        return proc;
    }

    Variable* var_check(const string& var_name, const string& var_range){
        Variable* var = nullptr;
        for (Variable* v: declared_variables)
            if(v->identifier == var_name && v->range == var_range)
                var = v;
        return var;
    }

    void handle_proc_head(const string& func_name){
        Procedure_obj* proc = proc_check(func_name);
        if(curr_procedure != nullptr){  //call to function
            if(proc == nullptr){
                string s = "Usage of undeclared procedure " + func_name;
                yyerror(s.c_str());
            } else {
                auto* ins = new Procedure_ins{proc};
                while(!var_stack.empty()){
                    auto var = var_stack.top();
                    var->init_status = AMBIGIOUS;
                    ins->vars_to_use.push_back(var);
                    var_stack.pop();
                }
//                p_ins.push_back(ins);
//                comm_container.top()->push_back(p_ins.back());
                  comm_containers.top()->push_back(ins);
            }
        } else {    //declaration
            if(proc == nullptr){    //new procedure
                auto* p = new Procedure_obj{func_name};
                defined_procedures.push_back(p);
                curr_procedure = proc_check(func_name);
                comm_containers.push(&curr_procedure->body);
                Variable* var_p;
                while(!var_stack.empty()){
                    var_p = var_stack.top();
                    var_stack.pop();
                    var_p->range = curr_procedure->identifier;
//                    cout<<"Got "<<var_p->identifier<<" from stack"<<endl;
                }
            } else{
                string s = "Second declaration of procedure " + func_name;
                yyerror(s.c_str());
            }
        }
    }

    void handle_proc_end(){
        curr_procedure = nullptr;
        comm_containers.pop();
    }

    void chech_double_decl(){
        for(auto var : declared_variables){
            for(auto var2 : declared_variables){
                if (var->identifier == var2->identifier && var->range == var2->range && var != var2){
                    string s = "Second declaration of variable " + var->identifier;
                    yyerror(s.c_str());
                }
            }
        }
    }


    void handle_var_decl(const string& ident){
        if(curr_procedure == nullptr){  //we're before next procedure declaration
            auto var = new Variable();
            var->identifier = ident;
            var->external = true;
            var->init_status = AMBIGIOUS;
//            cout<<"Var delcaration: "<<var->identifier<<", curr_proc = nullptr, in_def = "<<in_def<<endl;
            declared_variables.push_back(var);
            var_stack.push(declared_variables.back());
        } else {    //we're before call to a procedure from a different procedure
//            cout<<"Var delcaration: "<<ident<<" curr_proc: "<<curr_procedure->identifier<< " in_def = "<<in_def<<endl;

            if(in_def) {
                auto var = new Variable();
                var->identifier = ident;
                var->external = false;
                var->range = curr_procedure->identifier;
                declared_variables.push_back(var);
            } else{
                auto var = var_check(ident, curr_procedure->identifier);
                if(var == nullptr){
                    string error = "Usage of undeclared variable " + ident;
                    yyerror(error.c_str());
                } else {
                    var_stack.push(var);
                }
            }
        }
        chech_double_decl();
    }


    /**
     * Handles usage of variable in conditions, expressions and writes.
     * Used variable must have been declared and initialized earlier
     * @param ident
     */
    void handle_var_usage(const string& ident, bool can_be_uninit = false){
        if(curr_procedure == nullptr){
            yyerror("Variable usage outside procedure!");
        }
        auto var = var_check(ident, curr_procedure->identifier);
        if (var == nullptr){
            auto s = "Usage of undeclared variable" + ident;
            yyerror(s.c_str());
        } else if (var->init_status == NOT_INITIALIZED && !can_be_uninit){
            auto s = "Usage of uninitialized variable " + var->identifier;
            yyerror(s.c_str());
        } else {
            var_stack.push(var);
        }
    }

    void handle_const_usage(const string& ident){
        if(curr_procedure == nullptr){
            yyerror("Variable usage outside procedure!");
        }
        auto const_v = new Variable();
        const_v->identifier = "CONST_" + std::to_string(const_count);
        const_v->const_val = atoll(ident.c_str());
        const_count++;
        const_v->is_const = true;
        const_v->range = curr_procedure->identifier;
        const_v->init_status = INITIALIZED;
        declared_variables.push_back(const_v);
        var_stack.push(const_v);
    }

    void handle_condition(string op){
        Variable* var1 = var_stack.top();
        var_stack.pop();
        Variable* var2 = var_stack.top();
        var_stack.pop();
        auto c_ins = new Condition(op, var1, var2);
        c_ins->container = new vector<Command*>();
        comm_containers.push(c_ins->container);
        cond_stack.push(c_ins);
    }

    void handle_expression(const string& op){
        Variable* var1 = var_stack.top();
        var_stack.pop();
        Variable *var2 = nullptr;
        if(op != "just_var") {
            var2 = var_stack.top();
            var_stack.pop();
        }
        auto exp_ins = new Expression(op, var1, var2);
        curr_exp = exp_ins;
    }

    void handle_set_comm(){
        Variable* var = var_stack.top();
        var_stack.pop();
        auto set_exp = new Set_exp();
        set_exp->var = var;
        set_exp->exp = curr_exp;
        var->init_status = INITIALIZED;
        comm_containers.top()->push_back(set_exp);
    }

    void handle_write_comm(){
        Variable* var = var_stack.top();
        var_stack.pop();
        auto write_exp = new Write(var);
        comm_containers.top()->push_back(write_exp);
    }

    void handle_read_comm(){
        Variable* var = var_stack.top();
        var_stack.pop();
        var->init_status = INITIALIZED;
        auto read_exp = new Read(var);
        comm_containers.top()->push_back(read_exp);
    }

    void handle_ifelse(){
        vector<Command*>* else_cont = comm_containers.top();
        comm_containers.pop();
        vector<Command*>* if_cont = comm_containers.top();
        comm_containers.pop();
        auto if_ins = new If_else();
        if_ins->cond = cond_stack.top();
        cond_stack.pop();
        if_ins->if_body = if_cont;
        if_ins->else_body = else_cont;
        comm_containers.top()->push_back(if_ins);
    }

    void handle_else(){
        auto cont = new vector<Command*>();
        comm_containers.push(cont);
    }

    void handle_if(){
        vector<Command*>* cont = comm_containers.top();
        comm_containers.pop();
        auto if_ins = new If_exp();
        if_ins->cond = cond_stack.top();
        cond_stack.pop();
        if_ins->body = cont;
        comm_containers.top()->push_back(if_ins);
    }

    void handle_while(){
        vector<Command*>* cont = comm_containers.top();
        comm_containers.pop();
        auto wh = new While();
        wh->cond = cond_stack.top();
        cond_stack.pop();
        wh->body = cont;
        comm_containers.top()->push_back(wh);
    }

    void handle_repeat_end(){
        comm_containers.pop();  //removing empty container created by condition
        vector<Command*>* cont = comm_containers.top();
        comm_containers.pop();  //removing this loop container (now we're back to function)
        auto rep = new Repeat();
        comm_containers.top()->push_back(rep);
        rep->body = cont;
        rep->cond = cond_stack.top();
        cond_stack.pop();
    }

    void handle_repeat_begin(){
        auto cont = new vector<Command*>();
        comm_containers.push(cont);
    }

    void start_main(){
        auto main = new Procedure_obj{"main"};
        defined_procedures.push_back(main);
        curr_procedure = main;
        if(!comm_containers.empty()){
            std::cerr<<"Some other command container left when entering main!"<<endl;
        }
        comm_containers.push(&main->body);
    }

    void d_print_program_structures(){
        debugFile<<endl<<"---------DEBUG------------"<<endl<<endl;
        debugFile<<"Dump of defined_procedures vector:"<<endl;
        for (auto p : defined_procedures){
            debugFile<<p->identifier<<" Start pos: "<<p->start_pos<<" Memory from: "<<p->first_var_address<<" Jump addr: "<<p->jump_cell_address<<endl;
        }
        debugFile<<"Dump of declared_variables vector:"<<endl;
        for (auto v : declared_variables){
            debugFile<<v->identifier << " from " << v->range << " ext: " << v->external << " address: "<<v->addr<<endl;
        }
//        debugFile<<"Dump of proc_ins vector:"<<endl;
//        for (auto ins : p_ins){
//            debugFile<<ins->proc->identifier<<" with variables:"<<endl;
//            for(auto v : ins->vars_to_use){
//                debugFile<<"\t"<<v->identifier<<" from "<<v->range<<endl;
//            }
//        }
    }

    /**
     * CAUTION
     * Usage of this functions empties variables stack
     */
    void d_print_var_stack(){
        debugFile<<"Dump of variable stack (top to bottom):"<<endl;
        while (!var_stack.empty()){
            Variable* v = var_stack.top();
            debugFile << v->identifier << endl;
            var_stack.pop();
        }
    }

    void print_debug() const{
        if(curr_procedure == nullptr)
            cout<<"No current procedure"<<endl;
        else
            cout<<"In "<<curr_procedure->identifier<<endl;
    }

    void print_program_structure() {
        debugFile<<"Program is: "<<endl;
        for(auto c : curr_procedure->body){
            c->debug_print(debugFile);
        }
    }

    void set_def(bool val){
        debugFile<<"changing in_def to: "<<val<<endl;
        in_def = val;
    }

    void give_mem_address(){
        int curr = 5;
        for (auto p : defined_procedures){
            p->first_var_address = curr;
            for(auto var : declared_variables){
                if(var->range == p->identifier){
                    var->addr = curr;
                    curr++;
                }
            }
            p->jump_cell_address = curr;
            curr++;
        }
    }

    void handle_constants(AssemblerCode* coder){
        for(auto v : declared_variables){
            if(v->is_const){
                coder->add_order("SET", v->const_val);
                coder->add_order("STORE", v->addr);
            }
        }
    }

    void to_assembly(){

        auto* coder = new AssemblerCode();
        handle_constants(coder);
        int ind;
        if(defined_procedures.size() > 1) ind = coder->add_part_order("JUMP");
        for( auto p : defined_procedures){
            cout<<"Translating "<<p->identifier<<endl;
            p->get_assembly_code(coder);
        }
        if(defined_procedures.size() > 1) coder->set_part_order(ind, curr_procedure->start_pos);
        coder->print_out_file(outFile);
        outFile.close();
        delete coder;
    }

    ~Logic(){
        //stacks should be empty at this point
        for(auto v : declared_variables)
            delete v;
        for(auto p : defined_procedures){
            delete p;
        }
//        for(auto x : read_ins){
//            delete x;
//        }
//        for(auto x : p_ins){
//            delete x;
//        }
//        for(auto x : if_else_ins){
//            delete x;
//        }
//        for(auto x : if_ins){
//            delete x;
//        }
//        for(auto x : write_ins){
//            delete x;
//        }
//        for(auto x : rep_ins){
//            delete x;
//        }
//        for(auto x : while_ins){
//            delete x;
//        }
//        for(auto x : set_ins){
//            delete x;
//        }
    }
};