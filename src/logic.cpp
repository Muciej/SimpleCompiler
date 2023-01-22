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
    //program structures
    vector<Procedure_obj> defined_procedures;
    vector<Variable> defined_variables;
    stack<Variable> var_stack;
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

    bool var_check(const string& var_name){
        Variable* var = nullptr;
        for (Variable& v: defined_variables)
            if(v.identifier == var_name)
                var = &v;
        return var;
    }

    void handle_proc_head(const string& func_name){
        Procedure_obj* proc = proc_check(func_name);
        if(curr_procedure != nullptr){
            if(proc == nullptr){
                string s = "Usage of undeclared procedure " + func_name;
                yyerror(s.c_str());
            } else {
                Procedure_ins ins{proc};
                p_ins.push_back(ins);
                commands_stack.push(&ins);
            }
        } else {
            if(proc == nullptr){    //new procedure
                Procedure_obj p{func_name};
                defined_procedures.push_back(std::move(p));
                curr_procedure = proc_check(func_name);
            } else{
                string s = "Second declaration of procedure " + func_name;
                yyerror(s.c_str());
            }
        }
    }

    void handle_proc_end(){
        curr_procedure = nullptr;
    }

    void handle_var_ident(){

    }

    void start_main(){
        Procedure_obj main{"main"};
        defined_procedures.push_back(std::move(main));
        curr_procedure = proc_check("main");
    }

    void d_print_program_structures(){
        debugFile<<"Dump of defined_procedures vector:"<<endl;
        for (const Procedure_obj& p : defined_procedures){
            debugFile<<p.identifier<<endl;
        }
        debugFile<<"Dump of defined_variables vector:"<<endl;
        for (const Variable& v : defined_variables){
            debugFile<<v.identifier<<endl;
        }
    }

    /**
     * CAUTION
     * Usage of this functions empties variables stack
     */
    void d_print_var_stack(){
        debugFile<<"Dump of variable stack (top to bottom):"<<endl;
        while (!var_stack.empty()){
            Variable v = var_stack.top();
            debugFile<<v.identifier<<endl;
            var_stack.pop();
        }
    }

    void print_debug() const{
        if(curr_procedure == nullptr)
            cout<<"No current procedure"<<endl;
        else
            cout<<"In "<<curr_procedure->identifier<<endl;
    }
};