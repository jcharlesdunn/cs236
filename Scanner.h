#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED
#include "Token.h"
#include <vector>
#include <fstream>
using namespace std;

class Scanner{
    private:

    vector<Token> token_vector;
    ifstream in_file;
    char* input_string;
    char curr_char;
    int curr_line_num;
    int num_tokens;
    bool scan_error;
    int error_line;

    public:

    Scanner(char* input);
    void read_file();
    vector<Token> scan_token();// what kind of token
    void scan_ident();
    void scan_string();
    void scanString2(string m_in, bool eofFound);
    void scan_space();
    void scan_new_line();
    void print_tokens();
    void scan_comment();
    void scanLineComment(string m_in);
    void scancomment2(string m_in, bool eofFound, int currLineNumberCatchUp);
    void write_file();
    void scan_other();

};

#endif // SCANNER_H_INCLUDED
