#include "Rule.h"
#include <sstream>
#include <string>


Rule:: Rule(Predicate head, vector<Predicate> vector_in)
{
    head_pred =head;
    pred_vector = vector_in;
}

string Rule::to_string()
{
    stringstream ss;
    ss<< head_pred.to_string();
    ss<< " :- " <<pred_vector.at(0).to_string();
    for(unsigned int i = 1; i<pred_vector.size(); i++)
    {
        ss<<","<<pred_vector.at(i).to_string();
    }
    return ss.str();
}

vector<Predicate> Rule::get_preds()
{
    return pred_vector;
}
Predicate Rule::getHeadPred()
{
    return head_pred;
}
