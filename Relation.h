#ifndef RELATION_H_INCLUDED
#define RELATION_H_INCLUDED
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "Tuple.h"
#include "Scheme.h"
#include "Predicate.h"

using namespace std;

class Relation{

private:
    string relation_name;
    Scheme relation_scheme;
    set<Tuple> tuple_set;
    Scheme scheme_from_project;
    Scheme scheme_from_rename;



public:
    Relation(string relation_name_input);
    Relation();
    void relation_add_scheme(Scheme relation_scheme_input);
    void relation_add_facts(Tuple fact_tuple);

    set<Tuple> relation_select(vector<Parameter> query_input);
    set<Tuple> relation_select_string_constant(set<Tuple>& temp_set,vector<Parameter>& query_input,int index);
    set<Tuple> relation_select_ID(set<Tuple>& temp_set,vector<Parameter>& query_input,int index,int dupIndex);

    set<Tuple> relation_project(set<Tuple> set_from_selct,vector<Parameter> query_input);

    void relation_rename(vector<Parameter> query_input);

    Relation relation_join(Relation& rel_2);
    Scheme relation_join_scheme(Relation& rel_2);
    set<Tuple> relation_join_tuples(Relation& rel_2);

    Relation relation_union(Relation& rel_2);

    string relation_get_name();
    Scheme relation_get_scheme();
    void relation_set_scheme(Scheme i_scheme);
    set<Tuple> relation_get_tuple_set();
    void relation_set_tuple_set(set<Tuple> i_set);


    int findDuplicateID(vector<Parameter> query_input, unsigned int start_index);

};


#endif // RELATION_H_INCLUDED
