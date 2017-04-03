#include "Token.h"
#include <sstream>

Token::Token(token_type tok,string val,int num)
{
    token = tok;
    value = val;
    line_num = num;

}
string Token::to_string()
{
    stringstream ss;
    string temp_tok;
    switch (token)
    {
    case COMMA:
        temp_tok = "COMMA";
    break;
    case PERIOD:
        temp_tok = "PERIOD";
    break;
    case Q_MARK:
        temp_tok = "Q_MARK";
    break;
    case L_PAREN:
        temp_tok = "LEFT_PAREN";
    break;
    case R_PAREN:
        temp_tok = "RIGHT_PAREN";
    break;
    case COLON:
        temp_tok = "COLON";
    break;
    case COLON_DASH:
        temp_tok = "COLON_DASH";
    break;
    case SCHEMES:
        temp_tok = "SCHEMES";
    break;
    case FACTS:
        temp_tok = "FACTS";
    break;
    case RULES:
        temp_tok = "RULES";
    break;
    case QUERIES:
        temp_tok = "QUERIES";
    break;
    case ID:
        temp_tok = "ID";
    break;
    case EOFILE:
        temp_tok = "EOF";
    break;
    case STRING:
        temp_tok = "STRING";
    break;
    case UNDEFINED:
        temp_tok = "UNDEFINED";
    break;
    case COMMENT:
        temp_tok = "COMMENT";
    break;
    case ADD:
        temp_tok = "+";
    break;
    case MULTIPLY:
        temp_tok = "*";
    break;
    }
    ss << "(" << temp_tok << ",\"" << value << "\"," << line_num<< ")";
    return ss.str();
}
token_type Token::get_type()
{
    return token;
}
string Token::get_value()
{
    return value;
}
int Token::get_line()
{
    return line_num;
}
