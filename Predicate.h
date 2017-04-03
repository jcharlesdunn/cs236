#ifndef PREDICATE_H_INCLUDED
#define PREDICATE_H_INCLUDED
#include <iostream>
#include <vector>
#include "Token.h"
#include "Parameter.h"

using namespace std;

class Predicate {

private:

    string pred_name;
    vector<Parameter> content;

public:
    Predicate(string pred_name, vector<Parameter> content);
    Predicate();
    string to_string();
    string get_name();
    vector<Parameter> get_content();
};


#endif // PREDICATE_H_INCLUDED
