#include "Relation.h"

bool is_first_pass = true;

Relation::Relation(string relation_name_input)
{
    relation_name = relation_name_input;
}

Relation::Relation()
{}

void Relation::relation_add_scheme(Scheme relation_scheme_input)
{
    relation_scheme = relation_scheme_input;
}
void Relation::relation_add_facts(Tuple fact_tuple)
{
    tuple_set.insert(fact_tuple);
}
set<Tuple> Relation::relation_select(vector<Parameter> query_input)
{
    set<Tuple> select_tuple_set;
    set<Tuple> temp_tuple_set;
    temp_tuple_set = tuple_set; ///start with all tuples

    for(unsigned int i = 0; i < query_input.size(); i++)
    {
        if(query_input.at(i).get_is_ID())
        {
            //cout << "ID" << endl;
            int indexDup = findDuplicateID(query_input,i);
            if(indexDup != -1)
            {
                temp_tuple_set = relation_select_ID(temp_tuple_set,query_input,i, indexDup);
            }
        }
        else
        {
            //cout << "STRING CONST" << endl;
            temp_tuple_set = relation_select_string_constant(temp_tuple_set,query_input,i);
        }
        /// add temp_tuple_set to
        select_tuple_set = temp_tuple_set;
    }

    return select_tuple_set;
}
set<Tuple> Relation::relation_select_string_constant(set<Tuple>& temp_set,vector<Parameter>& query_input,int index)
{
    set<Tuple> set_from_select;
    for (set<Tuple>::iterator it=temp_set.begin(); it!=temp_set.end(); ++it)
    {
        Tuple m_Tuple = (*it);
        if(m_Tuple.at(index) == query_input.at(index).get_name())
        {
            set_from_select.insert(m_Tuple);
            //cout << "STRING MATCH" << endl;
        }
    }
    return set_from_select;
}

set<Tuple> Relation::relation_select_ID(set<Tuple>& temp_set,vector<Parameter>& query_input,int index,int indexDup)
{
    set<Tuple> set_from_select;
    for (set<Tuple>::iterator it=temp_set.begin(); it!=temp_set.end(); ++it)
    {
        Tuple m_Tuple = (*it);
        if(m_Tuple.at(index) == m_Tuple.at(indexDup))
        {
            set_from_select.insert(m_Tuple);
            //cout << "STRING MATCH" << endl;
        }
    }
    return set_from_select;
}

set<Tuple> Relation::relation_project(set<Tuple> set_from_select,vector<Parameter> query_input)
{
    set<Tuple> projected_set;
    bool duplicate = false;
    for (set<Tuple>::iterator it=set_from_select.begin(); it!=set_from_select.end(); ++it)
    {
        Tuple temp_tuple = *it;
        Tuple projected_tuple;
        for(unsigned int i = 0; i < query_input.size();i++)
        {
            if(query_input.at(i).get_is_ID())
            {
                for(unsigned int j = 0; j < projected_tuple.size();j++)
                {
                    if(query_input.at(i).get_name() == query_input.at(j).get_name())
                    {
                        duplicate = true;
                    }
                }
                if(!duplicate)
                {
                    projected_tuple.push_back(temp_tuple.at(i));
                }
                duplicate = false;
            }
        }
        projected_set.insert(projected_tuple);
    }
    return projected_set;
}
void Relation::relation_rename(vector<Parameter> query_input)
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
}
string Relation::relation_get_name()
{
    return relation_name;
}
Scheme Relation::relation_get_scheme()
{
    return relation_scheme;
}

set<Tuple> Relation::relation_get_tuple_set()
{
    return tuple_set;
}

void Relation::relation_set_tuple_set(set<Tuple> i_set)
{
    tuple_set = i_set;
}

int Relation::findDuplicateID(vector<Parameter> query_input, unsigned int start_index)
{
    //cout << "Index to match: " << start_index << endl;
    for(unsigned int i = start_index + 1; i < query_input.size(); i++)
    {
        if(query_input.at(start_index).get_name() == query_input.at(i).get_name())
        {
            if(query_input.at(start_index).get_is_ID() && query_input.at(i).get_is_ID())
            {
                //cout << "EXACT match" << endl;
                return i;
            }
        }
    }
    return -1;
}

Relation Relation::relation_union(Relation& rel_2)
{
    Relation union_relation;
    Scheme union_scheme = relation_scheme;
    set<Tuple> union_set;
    for(unsigned int i = 0; i < rel_2.relation_get_scheme().size(); i++)
    {
        union_scheme.push_back(rel_2.relation_get_scheme().at(i));
    }
    union_relation.relation_set_scheme(union_scheme);

    set<Tuple> tuple_set2 = rel_2.relation_get_tuple_set();
    for (set<Tuple>::iterator it=tuple_set.begin(); it!=tuple_set.end(); ++it)
    {
        for (set<Tuple>::iterator it2=tuple_set2.begin(); it2!=tuple_set2.end(); ++it2)
        {
            ///a.insert(a.end(), b.begin(), b.end());
            Tuple temp_tuple = *it;
            Tuple temp_tuple2 = *it2;
            for(unsigned int i = 0; i < temp_tuple2.size(); i++)
            {
                temp_tuple.push_back(temp_tuple2.at(i));
            }
            union_set.insert(temp_tuple);
        }
    }
    union_relation.relation_set_tuple_set(union_set);
    return union_relation;
}

Scheme Relation::relation_join_scheme(Relation& rel_2)
{
    Scheme join_scheme = relation_scheme;
    bool match = false;
    //join schemes
    for(unsigned int i = 0; i < rel_2.relation_get_scheme().size(); i++)
    {
        match = false;
        for(unsigned int j = 0; j < join_scheme.size(); j++)
        {
            if(rel_2.relation_get_scheme().at(i) == join_scheme.at(j))
            {
                match = true;
            }
        }
        if(!match)
        {
            join_scheme.push_back(rel_2.relation_get_scheme().at(i));
        }
    }
    return join_scheme;
}

set<Tuple> Relation::relation_join_tuples(Relation& rel_2)
{
    set<Tuple> join_set;
    Scheme join_scheme = relation_scheme;
    set<Tuple> tuple_set2 = rel_2.relation_get_tuple_set();
    Tuple tempTuple;
	bool match = false;
	bool error = false;
	for (set<Tuple>::iterator it=tuple_set.begin(); it!=tuple_set.end(); ++it) // all tuples from rel_1
	{
		for (set<Tuple>::iterator it2=tuple_set2.begin(); it2!=tuple_set2.end(); ++it2) // all tuples from rel_2
		{
			tempTuple = *it;
			error = false;
			for(unsigned int j = 0; j < rel_2.relation_get_scheme().size(); j++) // all schemes in rel_2
			{
				match = false;

				for(unsigned int i = 0; i < join_scheme.size() ; i++) // all schemes in rel_1
				{
					if(join_scheme.at(i) == rel_2.relation_get_scheme().at(j))
					{
						match = true;
						if(tempTuple.at(i) != (*it2).at(j))
						{
							error = true;
						}
					}
				}
				if(!match)
				{
						tempTuple.push_back((*it2).at(j));
				}
			}
			if(!error)
				join_set.insert(tempTuple);
		}
	}
	return join_set;
}

Relation Relation::relation_join(Relation& rel_2)
{
    Relation join_relation;


    //join schemes
    Scheme join_scheme = relation_join_scheme(rel_2);


    join_relation.relation_set_scheme(join_scheme);

	//join tuples
	set<Tuple> join_set = relation_join_tuples(rel_2);

    join_relation.relation_set_tuple_set(join_set);

    return join_relation;
}

void Relation::relation_set_scheme(Scheme i_scheme)
{
    relation_scheme = i_scheme;
}





