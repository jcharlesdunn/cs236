#ifndef DATALOGPROGRAM_H_INCLUDED
#define DATALOGPROGRAM_H_INCLUDED

#include <vector>
#include "Predicate.h"
#include "Rule.h"

using namespace std;

class DatalogProgram{

private:
    vector<Predicate> data_schemes;
    vector<Predicate> data_facts;
    vector<Rule> data_rules;
    vector<Predicate> data_queries;



public:
    DatalogProgram();
    void add_schemes(vector<Predicate> schemes_in);
    void add_facts(vector<Predicate> facts_in);
    void add_rules(vector<Rule> rules_in);
    void add_queries(vector<Predicate> queries_in);

    vector<Predicate> get_schemes();
    vector<Predicate> get_facts();
    vector<Rule> get_rules();
    vector<Predicate> get_queries();

};


#endif // DATALOGPROGRAM_H_INCLUDED
