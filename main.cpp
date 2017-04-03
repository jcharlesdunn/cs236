#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
//#include "token.h"
#include "Scanner.h"
#include "Parser.h"
#include "Database.h"

using namespace std;

/*void print_relation(Scheme sch, set<Tuple> rel)
{
    for(unsigned int i = 0; i < sch.size(); i++)
    {
        cout << setw(3) << sch.at(i);
    }
    cout << endl;
    for (set<Tuple>::iterator it=rel.begin(); it!=rel.end(); ++it)
    {
        Tuple temp_tuple = *it;
        Tuple projected_tuple;
        for(unsigned int i = 0; i < temp_tuple.size();i++)
        {
            cout << setw(3) << temp_tuple.at(i);
        }
        cout << endl;
    }

}*/

int main(int argc, char** argv)
{

    //Lab 1
    Scanner scan(argv[1]);
    scan.read_file();
    //scan.scan_token();
    //scan.print_tokens();

    //Lab_2
    //Parser parse(scan.scan_token());
    //parse.datalogProgram();
    //parse.print_program();

    //Lab 3b 4b
    Parser parse(scan.scan_token());
    Database m_database(parse.datalogProgram());
    m_database.database_evaluation();






    return 0;
}
