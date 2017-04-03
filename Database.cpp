#include "Database.h"
#include <iomanip>
#include <vector>

void print_relation(Scheme sch, set<Tuple> rel)
{
    for(unsigned int i = 0; i < sch.size(); i++)
    {
        cout <<  sch.at(i) << setw(10);
    }
    cout << endl;
    for (set<Tuple>::iterator it=rel.begin(); it!=rel.end(); ++it)
    {
        Tuple temp_tuple = *it;
        Tuple projected_tuple;
        for(unsigned int i = 0; i < temp_tuple.size();i++)
        {
            cout << temp_tuple.at(i) << setw(10);
        }
        cout << endl;
    }
}
Database::Database(DatalogProgram datalog_prog_in)
{
    datalog_prog = datalog_prog_in;
}

void Database::database_evaluation()
{
    scheme_evaluation();
    fact_evaluation();
    rule_evaluation();
    query_evaluation();
}
void Database::scheme_evaluation()
{
    ///Start with an empty Database.
    string relationship_name;
    Scheme relationship_scheme;
    vector<Parameter> relationship_param;
    for(unsigned int i = 0; i < datalog_prog.get_schemes().size(); i++)
    {
        ///For each scheme in the Datalog program, add an empty Relation to the Database.
        ///Use the scheme name as the name of the relation and the attribute list from the scheme as the scheme of the relation.
        relationship_name = datalog_prog.get_schemes().at(i).get_name();
        relationship_param = datalog_prog.get_schemes().at(i).get_content();
        relationship_scheme = convert_vec_param2string_scheme(relationship_param);
        Relation rel_input = Relation(relationship_name);
        rel_input.relation_add_scheme(relationship_scheme);
        database_map.insert(pair<string,Relation>(relationship_name,rel_input));
    }
}
void Database::fact_evaluation()
{
    string relationship_name;
    vector<Parameter> fact_parameters;
    Tuple fact_tuple;
    ///For each fact in the Datalog program, add a Tuple to a Relation.
    for(unsigned int i = 0; i < datalog_prog.get_facts().size();i++)
    {
        relationship_name = datalog_prog.get_facts().at(i).get_name();
        fact_parameters = datalog_prog.get_facts().at(i).get_content();
        fact_tuple = convert_vec_param2string_tuple(fact_parameters);
        database_map.at(relationship_name).relation_add_facts(fact_tuple);
    }
    ///Use the predicate name from the fact to determine the Relation to which the Tuple should be added.
    ///Use the values listed in the fact to provide the values for the Tuple.
    ///print_fact_evaluation();
}
void Database::rule_evaluation()
{
    bool steadyState = false;
    int numIterations = 0;
    int curNumberTuples = 0;
    while(!steadyState)
    {
        numIterations++;
        /// get current number of tuples for steady state algorithm
        curNumberTuples = getNumTuples();
        for(unsigned int i = 0; i < datalog_prog.get_rules().size(); i++)
        {
            Rule tempRule = datalog_prog.get_rules().at(i);

            Predicate rulePred1 = tempRule.get_preds().at(0); ///first predicate on right hand side of rule
            Predicate rulePred2;

            Relation tempRel1;
            Relation tempRel2;

            set<Tuple> selectResult;

            if(tempRule.get_preds().size() > 1) ///more than one pred on right hand side of rule
            {
                tempRel1 = database_map.at(rulePred1.get_name());
                ///selectProjectRename
                tempRel1 = database_sel_proj_ren(rulePred1, false);
                //print_relation(tempRel1.relation_get_scheme(), tempRel1.relation_get_tuple_set());
                //tempRel1.relation_add_scheme(convert_vec_param2string_scheme(rulePred1.get_content()));
                ///join all rules from
                ///join 2nd to first, and then 3rd to the result and etc.
                for(unsigned int j = 1; j < tempRule.get_preds().size(); j++)
                {
                    rulePred2 = tempRule.get_preds().at(j);

                    tempRel2 = database_map.at(rulePred2.get_name());
                    ///selectProjectRename
                    tempRel2 = database_sel_proj_ren(rulePred2, false);
                    //tempRel2.relation_add_scheme(convert_vec_param2string_scheme(rulePred2.get_content()));

                    Relation result = tempRel1.relation_join(tempRel2);
                    selectResult = result.relation_get_tuple_set();

                    rulePred1 = Predicate("temp",convert_scheme2vec_param(result.relation_get_scheme()));

                    tempRel1 = result;
                }
            }
            else /// only one predicate in rule
            {
                ///just grab the relation from map and get its content
                tempRel1 = database_map.at(rulePred1.get_name());
                selectResult = tempRel1.relation_select(rulePred1.get_content());
            }
            ///sift result to match predHead
            ///rulePred1 and selectResult are the join of all the preds in rule
            ///now need to sift so result matches the rule head predicated
            database_sift(tempRule, selectResult, rulePred1);
        }
        if(numIterations > 1) ///stable state algorithm
        {
            //cout << "CURRENT NUM OF TUPLES \t" << curNumberTuples << endl;
            //cout << "Tuples added \t\t" << getNumTuples() << endl;
            if(curNumberTuples == getNumTuples()) /// number of tuples in database has not changed
                steadyState = true;
        }
    }
    cout << "Schemes populated after " << numIterations << " passes through the Rules.\n";
}
void Database::database_sift(Rule& tempRule, set<Tuple>&selectResult, Predicate& rulePred1)
{
    Predicate headPred = tempRule.getHeadPred();
    for (set<Tuple>::iterator it=selectResult.begin(); it!=selectResult.end(); ++it)
    {
        Tuple tempTuple;
        for(unsigned int i = 0; i < headPred.get_content().size(); i++)
        {
            for(unsigned int j = 0; j < rulePred1.get_content().size(); j++)
            {
                if(headPred.get_content().at(i).get_name() == rulePred1.get_content().at(j).get_name())
                {
                    tempTuple.push_back((*it).at(j));
                }
            }
        }
        string relName = headPred.get_name();
        database_map.at(relName).relation_add_facts(tempTuple); /// add sifted result to database
    }
}
int Database::getNumTuples()
{
    //database_map.insert(pair<string,Relation>(relationship_name,rel_input));
    int numTuples = 0;
    typedef map<string, Relation>::iterator it_type;
    for(it_type iterator = database_map.begin(); iterator != database_map.end(); iterator++)
    {
        set<Tuple> tempSet = iterator->second.relation_get_tuple_set();
        for (set<Tuple>::iterator it=tempSet.begin(); it!=tempSet.end(); ++it)
        {
            numTuples++;
        }

    }
    return numTuples;
}

void Database::query_evaluation()
{
    print_query_evaluation();

}

Scheme Database::convert_vec_param2string_scheme(vector<Parameter> input_parameter)
{
    Scheme output_scheme;
    for(unsigned int i = 0; i < input_parameter.size(); i++)
    {
        output_scheme.push_back(input_parameter.at(i).get_name());
    }
    return output_scheme;
}

vector<Parameter> Database::convert_scheme2vec_param(Scheme input_scheme)
{
    vector<Parameter> output_parameter;
    for(unsigned int i = 0; i < input_scheme.size(); i++)
    {
        Parameter tempParam(input_scheme.at(i), true);
        output_parameter.push_back(tempParam);
    }
    return output_parameter;
}

Tuple Database::convert_vec_param2string_tuple(vector<Parameter> input_parameter)
{
    Tuple output_tuple;
    for(unsigned int i = 0; i < input_parameter.size(); i++)
    {
        output_tuple.push_back(input_parameter.at(i).get_name());
    }
    return output_tuple;

}

Relation Database::database_sel_proj_ren(Predicate query_input, bool verbose)
{
    string query_scheme = query_input.get_name();
    Relation temp_relation = database_map.at(query_scheme);
    set<Tuple> temp_set_1 = temp_relation.relation_select(query_input.get_content());
    if(verbose)
    {
        if(temp_set_1.size() != 0)
        {
            cout << "Yes("<< temp_set_1.size() << ")\n";
        }
        else
        {
            cout << "No";
        }
    }

    Scheme schemeResult;
    vector<Parameter> vecParamResult;
    set<Tuple> temp_set_2;
    if(query_contains_variables(query_input.get_content()))
    {
        temp_set_2 = database_proj(temp_set_1,query_input,temp_relation); ///project
        vecParamResult = database_ren(temp_set_2,query_input, temp_relation, verbose); /// rename and print of results
    }
    if(temp_set_1.size() == 0)
        cout << endl;
    schemeResult = convert_vec_param2string_scheme(vecParamResult);
    temp_relation.relation_set_scheme(schemeResult);
    temp_relation.relation_set_tuple_set(temp_set_2);
    return temp_relation;
}
set<Tuple> Database::database_proj(set<Tuple> temp_set_1,Predicate input_pred, Relation temp_relation)
{
    set<Tuple> temp_set_2 = temp_relation.relation_project(temp_set_1,input_pred.get_content());
    return temp_set_2;
}

vector<Parameter> Database::database_ren(set<Tuple> input_set, Predicate input_pred, Relation temp_relation, bool verbose)
{
    vector<Parameter> renamed_query;
    if(input_set.size() != 0)
    {
        renamed_query = database_rename(input_pred.get_content());
        if(!verbose)
            return renamed_query;
        for (set<Tuple>::iterator it=input_set.begin(); it!=input_set.end(); ++it)
        {
            Tuple temp_tuple = *it;

            cout << "  "; ///formating
            for(unsigned int i = 0 ; i < renamed_query.size();i++)
            {
                if(i == (renamed_query.size()-1))
                    cout << renamed_query.at(i).get_name() <<"=" << temp_tuple.at(i);
                else
                    cout << renamed_query.at(i).get_name() <<"=" << temp_tuple.at(i) << ", ";
            }
            cout << "\n";
        }
    }
    return renamed_query;
}

void Database::print_query_evaluation()
{
    for(unsigned int i = 0; i < datalog_prog.get_queries().size();i++)
    {
        cout << datalog_prog.get_queries().at(i).to_string() << "? ";
        database_sel_proj_ren(datalog_prog.get_queries().at(i), true);
    }
}

vector<Parameter> Database::database_rename(vector<Parameter> query_input)
{
    vector<Parameter> projected_query;
    bool duplicate = false;
    for(unsigned int i = 0; i < query_input.size(); i++)
    {
        if(query_input.at(i).get_is_ID())
        {
            for(unsigned int j = 0; j < projected_query.size();j++)
            {
                if(query_input.at(i).get_name() == projected_query.at(j).get_name())
                {
                    duplicate = true;
                }
            }
            if(!duplicate)
            {
                projected_query.push_back(query_input.at(i));
            }
            duplicate = false;
        }
    }
    return projected_query;
}

Scheme Database::database_project(Scheme relation_scheme,vector<Parameter> query_input)
{
    Scheme projected_relation;
    bool duplicate = false;
    for(unsigned int i = 0; i < relation_scheme.size(); i++)
    {
        if(query_input.at(i).get_is_ID())
        {
            /// check for duplicates
            for(unsigned int j = 0 ; j < projected_relation.size(); j++)
            {
                if(query_input.at(j).get_name() == query_input.at(i).get_name())
                {
                    duplicate = true;
                }
            }
            if(!duplicate)
            {
                projected_relation.push_back(relation_scheme.at(i));
            }
            duplicate = false;
        }
    }
    return projected_relation;
}

bool Database::query_contains_variables(vector<Parameter> query_input)
{
    for(unsigned int i = 0; i < query_input.size();i++)
    {
        if(query_input.at(i).get_is_ID())
        {
            return true;
        }
    }
    return false;
}


