#include "Parameter.h"
#include <sstream>

Parameter::Parameter(string input, bool input_bool)
{
    parameter_name = input;
    is_ID = input_bool;

}

Parameter::Parameter()
{
    isExpression = false;
}

string Parameter:: get_name()
{
    return parameter_name;
}
void Parameter::set_name(string input)
{
    parameter_name = input;
}
bool Parameter:: get_is_ID()
{
    return is_ID;
}
void Parameter:: set_is_ID(bool input)
{
    is_ID = input;
}
string Parameter:: to_string()
{
    return parameter_name;
}

Expression::Expression(Parameter l_param, Parameter r_param, string s_operator)
{
    l_param = l_param;
    r_param = r_param;
    s_operator = s_operator;
    isExpression = true;
}

string Expression::to_string()
{
    stringstream ss;
    ss << "(" << l_param.to_string() << s_operator << r_param.to_string() << ")";
    return ss.str();

}
