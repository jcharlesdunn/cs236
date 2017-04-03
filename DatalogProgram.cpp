#include "DatalogProgram.h"

DatalogProgram::DatalogProgram()
{}

void DatalogProgram::add_schemes(vector<Predicate>schemes_in)
{
    data_schemes = schemes_in;
}

void DatalogProgram::add_facts(vector<Predicate>facts_in)
{
    data_facts = facts_in;
}

void DatalogProgram::add_rules(vector<Rule>rules_in)
{
    data_rules = rules_in;
}

void DatalogProgram::add_queries(vector<Predicate>queries_in)
{
    data_queries = queries_in;
}

vector<Predicate> DatalogProgram::get_schemes()
{
    return data_schemes;
}
vector<Predicate> DatalogProgram::get_facts()
{
    return data_facts;
}
vector<Rule> DatalogProgram::get_rules()
{
    return data_rules;
}
vector<Predicate> DatalogProgram::get_queries()
{
    return data_queries;
}
