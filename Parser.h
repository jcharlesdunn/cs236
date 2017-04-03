#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "Token.h"
#include "DatalogProgram.h"
#include "Predicate.h"
#include "Rule.h"
#include <vector>
#include <stack>
#include <iostream>
#include <sstream>

class Parser{

enum current_list{
    CURR_SCHEME,
    CURR_FACT,
    CURR_RULE,
    CURR_QUERIES
};

private:
    ///vector<Token> token_vector;
    DatalogProgram datalog_prog;
    stack<Token> token_stack;
    vector<Predicate> scheme_vector;
    vector<Predicate> facts_vector;
    vector<Predicate> queries_vector;
    vector<Rule> rule_vector;
    vector<Parameter> domain;
    vector<string> domain_string;

    current_list curr_list;
    char* output_file;
    bool parse_error;


public:
    Parser(vector<Token> token_vector);

    DatalogProgram datalogProgram();
    void scheme();
    void schemeList();
    void idList();
    void fact();
    void factList();
    void rule();
    void ruleList();
    void headPredicate();
    Predicate predicate();
    vector<Predicate> predicateList();
    Parameter parameter();
    vector<Parameter> parameterList();
    void expression();
    void operator_f();
    void query();
    void queryList();
    void stringList();


    void match(token_type token);
    bool bool_match(token_type token);
    void compute_domain();


    void print_program();
    void write_program();

};



#endif // PARSER_H_INCLUDED
