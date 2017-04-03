#ifndef DATABASE_H_INCLUDED
#define DATABASE_H_INCLUDED
#include <iostream>


#include <map>
#include "Parser.h"
#include "Relation.h"
#include <fstream>

using namespace std;

class Database{

private:
    map<string,Relation> database_map;
    set<Tuple> query_result_tuple;
    DatalogProgram datalog_prog;
    char* output_file;


public:
    Database(DatalogProgram datalog_prog);
    Database(){};
    void database_evaluation();
    void scheme_evaluation();
    void fact_evaluation();
    void rule_evaluation();
    void database_sift(Rule& tempRule, set<Tuple>&selectResult, Predicate& rulePred1);
    void query_evaluation();
    Scheme convert_vec_param2string_scheme(vector<Parameter> input_parameter);
    Tuple convert_vec_param2string_tuple(vector<Parameter> input_parameter);
    vector<Parameter> convert_scheme2vec_param(Scheme input_scheme);


    Relation database_sel_proj_ren(Predicate query_input, bool verbose);
    set<Tuple> database_proj(set<Tuple> input_set,Predicate input_pred,Relation temp_relation);
    vector<Parameter> database_ren(set<Tuple> input_set,Predicate input_pred,Relation temp_relation, bool verbose);


    void print_scheme_evaluation();
    void print_fact_evaluation();
    void print_rule_evaluation();
    void print_query_evaluation();
    void print_query_evaluation_select(set<Tuple> temp_set);


    vector<Parameter> database_rename(vector<Parameter> query_input);
    Scheme database_project(Scheme relation_scheme,vector<Parameter> query_input);

    bool query_contains_variables(vector<Parameter> query_input);

    int getNumTuples();
};


#endif // DATABASE_H_INCLUDED
