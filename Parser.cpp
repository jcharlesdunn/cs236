#include "Parser.h"
#include <fstream>
#include <algorithm>


typedef enum parse_state{
    SCHEMES_ST,
    FACTS_ST,
    RULES_ST,
    QUERIES_ST
} parse_st;

parse_st curr_st = SCHEMES_ST;


Parser::Parser(vector<Token> token_vector)
{
    while(!token_vector.empty())
    {
        if(COMMENT != token_vector.back().get_type())
            token_stack.push(token_vector.back());
        token_vector.pop_back();
    }
    parse_error = false;
}

DatalogProgram Parser::datalogProgram()
{
    try{

    match(SCHEMES);match(COLON);scheme();schemeList();
    curr_st = FACTS_ST;
    match(FACTS);match(COLON);factList();
    curr_st = RULES_ST;
    match(RULES);match(COLON);ruleList();
    curr_st = QUERIES_ST;
    match(QUERIES);match(COLON);query();queryList();
    match(EOFILE);
    datalog_prog.add_schemes(scheme_vector);
    datalog_prog.add_facts(facts_vector);
    datalog_prog.add_rules(rule_vector);
    datalog_prog.add_queries(queries_vector);
    }
    catch(Token tok)
    {
        parse_error = true;
        cout << "Failure!\n  " << tok.to_string()<<'\n';
    }
    return datalog_prog;
}

void Parser::schemeList()
{
    if(ID ==token_stack.top().get_type())
    {
        scheme();schemeList();
    }
}

void Parser::factList()
{

    if(ID ==token_stack.top().get_type())
    {
        fact();factList();
    }
}

void Parser::ruleList()
{
    if(ID == token_stack.top().get_type())
    {
        rule();ruleList();
    }
}

void Parser::queryList()
{
    if(ID == token_stack.top().get_type())
    {
        query();queryList();
    }
}

void Parser::scheme()
{
    scheme_vector.push_back(predicate());
}

void Parser::fact()
{
    facts_vector.push_back(predicate());
    match(PERIOD);
}

void Parser::rule()
{
    Predicate rule_head = predicate();
    match(COLON_DASH);
    vector<Predicate> pred_list_1;
    vector<Predicate> pred_list_2;
    pred_list_1.push_back(predicate());
    pred_list_2 = predicateList();

    pred_list_1.insert(pred_list_1.end(),pred_list_2.begin(),pred_list_2.end());
    rule_vector.push_back(Rule(rule_head, pred_list_1));
    match(PERIOD);
}

void Parser::query()
{
    queries_vector.push_back(predicate());
    match(Q_MARK);
}

vector<Predicate> Parser::predicateList()
{
    vector<Predicate> pred_vector;
    while(token_stack.top().get_type() == COMMA)
    {
        match(COMMA);
        pred_vector.push_back(predicate());
    }
    return pred_vector;
}

Predicate Parser::predicate()
{
    string pred_ID;
    if(token_stack.top().get_type() == ID)
    {
        pred_ID = token_stack.top().get_value();
        match(ID);
    }
    else
    {
        match(ID); /// catches error
    }
    match(L_PAREN);
    vector<Parameter> param_list_1;
    vector<Parameter> param_list_2;
    param_list_1.push_back(parameter());
    param_list_2 = parameterList();

    param_list_1.insert(param_list_1.end(),param_list_2.begin(),param_list_2.end());
    match(R_PAREN);

    return Predicate(pred_ID,param_list_1);
}

vector<Parameter> Parser::parameterList()
{
    vector<Parameter> param_vector;
    while(token_stack.top().get_type() == COMMA)
    {
        match(COMMA);
        param_vector.push_back(parameter());
    }
    return param_vector;
}

Parameter Parser::parameter()
{
    if(curr_st == SCHEMES_ST)
    {
        ///no STRINGS
        Parameter param = Parameter(token_stack.top().get_value(),true);
        match(ID);
        return param;
    }
    else if(curr_st == FACTS_ST)
    {
        /// no ID
        Parameter param = Parameter(token_stack.top().get_value(),false);
        domain.push_back(param);
        match(STRING);
        return param;
    }
    else
    {
        if(ID == token_stack.top().get_type())
        {
            Parameter param = Parameter(token_stack.top().get_value(),true);
            match(ID);
            return param;
        }
        else if (STRING == token_stack.top().get_type())
        {
            Parameter param = Parameter(token_stack.top().get_value(),false);
            match(STRING);
            return param;
        }
        else ///expression
        {
            match(L_PAREN);
            Parameter param_l = parameter();
            string s_operator = token_stack.top().get_value();
            if(ADD == token_stack.top().get_type())
            {
                match(ADD);
            }
            else
            {
                match(MULTIPLY);
            }
            Parameter param_r = parameter();
            match(R_PAREN);

            Expression expr = Expression(param_l,  param_r, s_operator);
            return expr;
        }
    }
}

void Parser::match(token_type token)
{
    if(token == token_stack.top().get_type())
        token_stack.pop();
    else
        throw token_stack.top();
}



void Parser::print_program()
{
    if(!parse_error)
    {
        cout << "Success!\n";
        cout << "Schemes("<< scheme_vector.size()<<"):\n";
        for(unsigned int i= 0; i<scheme_vector.size();i++)
        {
            cout << "  " << scheme_vector.at(i).to_string() << "\n";
        }
        cout << "Facts("<< facts_vector.size()<<"):\n";
        for(unsigned int i= 0; i<facts_vector.size();i++)
        {
            cout << "  "<< facts_vector.at(i).to_string() << ".\n";
        }
        cout << "Rules("<< rule_vector.size()<<"):\n";
        for(unsigned int i= 0; i<rule_vector.size();i++)
        {
            cout << "  "<< rule_vector.at(i).to_string() << ".\n";
        }
        cout << "Queries("<< queries_vector.size()<<"):\n";
        for(unsigned int i= 0; i<queries_vector.size();i++)
        {
            cout << "  "<< queries_vector.at(i).to_string() << "?\n";
        }
        compute_domain();
        cout << "Domain(" << domain_string.size() << "):\n";
        for (unsigned int i = 0; i<domain_string.size();i++)
        {
            cout <<"  "<< domain_string.at(i) << "\n";
        }



    }
}


void Parser::compute_domain()
{
    /*
    for(unsigned int i= 0; i<facts_vector.size();i++)
    {
        vector<Parameter> param_list_1 = facts_vector.at(i).get_content();
        domain.insert(domain.end(),param_list_1.begin(),param_list_1.end());
    }

    for(unsigned int i= 0; i<rule_vector.size();i++)
    {
        for(unsigned int j =0; j < rule_vector.at(i).get_preds().size(); j++)
        {
            vector<Parameter> param_list_1 = rule_vector.at(i).get_preds().at(j).get_content();
            domain.insert(domain.end(),param_list_1.begin(),param_list_1.end());
        }
    }

    for(unsigned int i= 0; i<queries_vector.size();i++)
    {
        vector<Parameter> param_list_1 = queries_vector.at(i).get_content();
        domain.insert(domain.end(),param_list_1.begin(),param_list_1.end());
    }*/
    for(unsigned int i = 0 ; i <domain.size();i++)
    {
        if(domain[i].to_string().at(0) != '\'')
        {
            domain.erase(domain.begin()+ i);
        }
    }

    for(unsigned int i = 0; i < domain.size();i++)
    {
        if(find(domain_string.begin(),domain_string.end(),domain[i].to_string()) == domain_string.end())
        {
            domain_string.push_back(domain.at(i).to_string());

        }
    }
    sort(domain_string.begin(),domain_string.end());
}

