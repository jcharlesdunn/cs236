#ifndef RULE_H_INCLUDED
#define RULE_H_INCLUDED
#include "Predicate.h"
#include <vector>
using namespace std;

class Rule{

private:
    Predicate head_pred;
    vector<Predicate> pred_vector;
public:
    Rule(Predicate head, vector<Predicate> pred_vector);
    string to_string();
    vector<Predicate> get_preds();
    Predicate getHeadPred();


};

#endif // RULE_H_INCLUDED
