#ifndef TOKEN_H_INCLUDED
#define TOKEN_H_INCLUDED
#include <string>
#include <iostream>
using namespace std;
typedef enum token_type{
    COMMA, // 1
    PERIOD, // 2
    Q_MARK, // 3
    L_PAREN, // 4
    R_PAREN, // 5
    COLON, // 6
    COLON_DASH, // 7
    SCHEMES, // 8
    FACTS, // 9
    RULES, // 10
    QUERIES, // 11
    ID, // 12
    EOFILE, // 13
    STRING, // 14
    UNDEFINED,
    COMMENT,
    ADD,
    MULTIPLY
} token_type;



class Token {
    private:
    string value;
    int line_num;
    token_type token;

    public:
    Token(token_type token, string value, int line_num);
    Token(){}
    string to_string();
    token_type get_type();
    string get_value();
    int get_line();

};

#endif // TOKEN_H_INCLUDED
