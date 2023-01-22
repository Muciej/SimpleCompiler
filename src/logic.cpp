//
// Created by Maciek Józefkowicz on 10.01.2023.
//

#include <iostream>
#include <fstream>
#include <stack>
#include <utility>
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

    //program structures
    vector<Procedure_obj> defined_procedures;
    vector<Variable> declared_variables;
    stack<Variable*> var_stack;
    stack<Command*> commands_stack;

    //containers
    vector<Procedure_ins> p_ins;
    vector<While> while_ins;
    vector<Repeat> rep_ins;
    vector<If_exp> if_ins;
    vector<If_else> if_else_ins;
    vector<Read> read_ins;
    vector<Write> write_ins;
    vector<Set_exp> set_ins;

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
        for (Procedure_obj& p: defined_procedures){
            if(p.identifier == func_name) proc = &p;
        }
        return proc;
    }

    Variable* var_check(const string& var_name, const string& var_range){
        Variable* var = nullptr;
        for (Variable& v: declared_variables)
            if(v.identifier == var_name && v.range == var_range)
                var = &v;
        return var;
    }



    void handle_proc_head(const string& func_name){
        Procedure_obj* proc = proc_check(func_name);
        if(curr_procedure != nullptr){  //call to function
            if(proc == nullptr){
                string s = "Usage of undeclared procedure " + func_name;
                yyerror(s.c_str());
            } else {
                Procedure_ins ins{proc};
                while(!var_stack.empty()){
                    ins.vars_to_use.push_back(var_stack.top());
                    var_stack.pop();
                }
                p_ins.push_back(std::move(ins));
                commands_stack.push(&p_ins.back());
            }
        } else {    //declaration
            if(proc == nullptr){    //new procedure
                Procedure_obj p{func_name};
                defined_procedures.push_back(std::move(p));
                curr_procedure = proc_check(func_name);
                Variable* var_p = nullptr;
                while(!var_stack.empty()){
                    var_p = var_stack.top();
                    var_stack.pop();
                    var_p->range = curr_procedure->identifier;
                    cout<<"Got "<<var_p->identifier<<" from stack"<<endl;
                }
            } else{
                string s = "Second declaration of procedure " + func_name;
                yyerror(s.c_str());
            }
        }
    }

    void handle_proc_end(){
        curr_procedure = nullptr;
    }

    void handle_var_decl(const string& ident){
        if(curr_procedure == nullptr){
//            cout<<"Var delcaration: "<<ident<<", curr_proc = nullptr, in_def = "<<in_def<<endl;
            Variable var;
            var.identifier = ident;
            var.external = true;
            declared_variables.push_back(std::move(var));
            var_stack.push(var_check(ident, "undef"));
        } else {
//            cout<<"Var delcaration: "<<ident<<" curr_proc: "<<curr_procedure->identifier<< " in_def = "<<in_def<<endl;

            if(in_def) {
                Variable var;
                var.identifier = ident;
                var.external = false;
                var.range = curr_procedure->identifier;
                declared_variables.push_back(std::move(var));
            } else{
                Variable* var = var_check(ident, curr_procedure->identifier);
                if(var == nullptr){
                    string error = "Usage of undeclared variable " + ident;
                    yyerror(error.c_str());
                } else {
//                    var_stack.push(var);
                }
            }
        }
    }

    void start_main(){
        Procedure_obj main{"main"};
        defined_procedures.push_back(std::move(main));
        curr_procedure = proc_check("main");
        while(!var_stack.empty()){
            Variable* v = var_stack.top();
            var_stack.pop();
            v->range = curr_procedure->identifier;
            v->external = false;
        }
    }

    void d_print_program_structures(){
        debugFile<<"Dump of defined_procedures vector:"<<endl;
        for (const Procedure_obj& p : defined_procedures){
            debugFile<<p.identifier<<endl;
        }
        debugFile<<"Dump of declared_variables vector:"<<endl;
        for (const Variable& v : declared_variables){
            debugFile<<v.identifier << " from " << v.range << " ext: " << v.external << endl;
        }
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

    void set_def(bool val){
        debugFile<<"changing in_def to: "<<val<<endl;
        in_def = val;
    }
};