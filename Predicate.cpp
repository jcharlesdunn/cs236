#include "Predicate.h"
#include <sstream>

Predicate::Predicate(string name_input,vector<Parameter> in_content)
{
    pred_name = name_input;
    content = in_content;
}
Predicate::Predicate()
{}

string Predicate::to_string()
{
    stringstream ss;
    ss<< pred_name << "(" << content.at(0).to_string();
    for(unsigned int i = 1; i < content.size();i++)
    {
        ss<<"," <<content.at(i).to_string();
    }
    ///ss.unget();
    ss<< ")";
    return ss.str();
}
string Predicate::get_name()
{
    return pred_name;
}

vector<Parameter> Predicate::get_content()
{
    return content;
}
