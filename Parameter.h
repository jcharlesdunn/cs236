#ifndef PARAMETER_H_INCLUDED
#define PARAMETER_H_INCLUDED

#include <string>
#include <iostream>
using namespace std;
class Parameter{

private:
    string parameter_name;
    bool is_ID;
    bool isExpression;



public:
    Parameter(string parameter_name, bool is_ID);

    Parameter();
    string get_name();
    void set_name(string name);
    bool get_is_ID();
    void set_is_ID(bool ID);
    string to_string();
};

class Expression: public Parameter{
private:
    Parameter l_param;
    Parameter r_param;
    string s_operator;
    bool isExpression;
public:
    Expression(Parameter l_param, Parameter r_param, string s_operator);
    string to_string();


};







#endif // PARAMETER_H_INCLUDED
