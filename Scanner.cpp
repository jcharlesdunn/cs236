#include "Scanner.h"
#include <fstream>




Scanner::Scanner(char* input)
{
    input_string = input;
    curr_line_num = 1;
    num_tokens = 0;
    scan_error = false;
    error_line = 1;
}
void Scanner::read_file()
{
    in_file.open(input_string);
    //in_file.open("in0.txt");
}

bool stringIsEmpty(string m_in)
{
  	bool empty = true;
  	for(unsigned int i = 0; i < m_in.size(); i++)
  	{
  		if(!isspace(m_in.at(i)))
  		{
  			empty = false;
  			break;
  		}
  	}

  	return empty;
}



vector<Token> Scanner::scan_token()// return vector of tokens
{

    while(!in_file.eof() && !scan_error)/// or error
    {
        curr_char = in_file.get();
         /// SKIP NEW LINES AND THEN SKIP WHITE SPACE
        scan_new_line();
        scan_space();

        switch(curr_char)
        {
            case ',':
            {
                token_vector.push_back(Token(COMMA,",",curr_line_num));
            }
            break;

            case '.':
            {
                Token temp_tok(PERIOD, ".", curr_line_num);
                token_vector.push_back(temp_tok);
            }
            break;

            case '?':
            {
                Token temp_tok(Q_MARK, "?", curr_line_num);
                token_vector.push_back(temp_tok);
            }
            break;

            case '(':
            {
                Token temp_tok(L_PAREN, "(", curr_line_num);
                token_vector.push_back(temp_tok);
            }
            break;

            case ')':
            {
                Token temp_tok(R_PAREN, ")", curr_line_num);
                token_vector.push_back(temp_tok);
            }
            break;

            case '+':
            {
                Token temp_tok(ADD, "+", curr_line_num);
                token_vector.push_back(temp_tok);
            }break;

            case '*':
            {
                Token temp_tok(MULTIPLY, "*", curr_line_num);
                token_vector.push_back(temp_tok);
            }break;

            case ':':
            {
                if(in_file.peek() == '-')
                {
                    in_file.get();
                    Token temp_tok(COLON_DASH, ":-", curr_line_num);
                    token_vector.push_back(temp_tok);
                }
                else
                {
                    Token temp_tok(COLON, ":", curr_line_num);
                    token_vector.push_back(temp_tok);
                }
            }
            break;

            case '\'':
            {
                scan_string();
            }
            break;

            case '#':
            {
                scan_comment();
            }break;

            default: // read in an identifier
            {
                scan_other();
            }
            break;
        }
    }
    if (!scan_error)/// if no error, must have reached end of file
    {
        if(token_vector.size() == 0)
        {
            curr_line_num = 1;
        }
        Token temp_tok(EOFILE, "", curr_line_num);
        token_vector.push_back(temp_tok);
    }
    return token_vector;
}
void Scanner::scan_ident()
{
    /// check for comments
    /// keywords
    /// identifiers

    string temp_string = "";
    while(isalnum(in_file.peek()))
    {
        temp_string += curr_char;
        curr_char = in_file.get();
    }
    temp_string += curr_char;
    if(temp_string == "Schemes")
    {
        Token temp_tok(SCHEMES, temp_string, curr_line_num);
        token_vector.push_back(temp_tok);
    }
    else if (temp_string == "Facts")
    {
        Token temp_tok(FACTS, temp_string, curr_line_num);
        token_vector.push_back(temp_tok);
    }
    else if (temp_string == "Rules")
    {
        Token temp_tok(RULES, temp_string, curr_line_num);
        token_vector.push_back(temp_tok);
    }
    else if (temp_string == "Queries")
    {
        Token temp_tok(QUERIES, temp_string, curr_line_num);
        token_vector.push_back(temp_tok);
    }
    else
    {
        if(isdigit(temp_string.at(0)))
        {
              Token temp_tok(UNDEFINED, temp_string, curr_line_num);
              token_vector.push_back(temp_tok);
        }
        else
        {
            Token temp_tok(ID, temp_string, curr_line_num);
            token_vector.push_back(temp_tok);
        }
    }

}

void Scanner::scanString2(string m_in, bool eofFound)
{
    if(!eofFound)
        token_vector.push_back(Token(STRING, m_in, curr_line_num));
    else if(!stringIsEmpty(m_in))
    {
      token_vector.push_back(Token(UNDEFINED,m_in, curr_line_num));
    }

}

void Scanner::scan_string()
{
      string m_in = "\'";
      bool stringScan = true, eofFound = false;
      int currLineNumberCatchUp = 0;
      string m_temp = "";
      while(stringScan)
      {
          if(in_file.peek() == '\n')
          {
              currLineNumberCatchUp++;
          }
          if(in_file.eof())
          {
              stringScan = false; //end of file reached
              eofFound = true;
          }
          else
          {
              m_temp = in_file.get();
              if(m_temp == "\'")
              {
                  if(in_file.peek() == '\'' ) // get the apostraphe
                  {
                      m_in += m_temp;
                      m_temp = in_file.get();
                      m_in += m_temp;
                  }
                  else // im done
                  {
                      stringScan = false;
                      m_in += m_temp;
                  }
              }
              else
              {
                  m_in += m_temp;
              }
          }
      }
      if(m_in != "\r")
      {
      scanString2(m_in, eofFound);
      curr_line_num += currLineNumberCatchUp;
    }
}
void Scanner::scan_space()
{
    while(isspace(curr_char))
    {
        if(curr_char == '\n')
        {
            curr_line_num++;
        }
        curr_char = in_file.get();
    }
    scan_new_line();
}

void Scanner::scan_new_line()
{
    while(curr_char == '\n')
    {
        curr_line_num++;
        curr_char = in_file.get();
    }
}

void Scanner::print_tokens()
{
    for(unsigned i=0; i< token_vector.size(); i++)
    {
        cout << token_vector.at(i).to_string() << '\n';
    }
    cout << "Total Tokens = " << token_vector.size();
}
void Scanner::scancomment2(string m_in, bool eofFound, int currLineNumberCatchUp)
{
      if(m_in != "\r")
      {
          if(!eofFound)
          {
            token_vector.push_back(Token(COMMENT, m_in, curr_line_num));
          }
          else
          {
            if(!stringIsEmpty(m_in))
              token_vector.push_back(Token(UNDEFINED, m_in, curr_line_num));
          }
      curr_line_num += currLineNumberCatchUp;
    }
}

void Scanner::scanLineComment(string m_in)
{
    string m_temp = "";
    bool commentScan = true;
    while(commentScan)
    {
        m_temp = in_file.get();
        if((m_temp == "\n") || in_file.eof())
        {
            commentScan = false;
        }
        else
        {
            m_in = m_in + m_temp;
        }
  }
  token_vector.push_back(Token(COMMENT, m_in, curr_line_num));
  curr_line_num++;
}

void Scanner::scan_comment()
{
    string m_temp;
    string m_in(curr_char,1);
    if(in_file.peek() == '|') //scan multi-line comment
    {
          bool stringScan = true, eofFound = false;
          int currLineNumberCatchUp = 0;
          while(stringScan)
          {
                // REPORT END OF FILE ERROR
                //cout << "Stuck 1 \n";
                if(in_file.peek() == '\n')
                {
                    currLineNumberCatchUp++;
                }
                if(in_file.eof())
                {
                    stringScan = false; //end of file reached
                    eofFound = true;
                }
                else
                {
                    m_temp = in_file.get();
                    if((m_temp == "|") && in_file.peek() == '#')
                    {
                        // found end of comment
                        m_in += m_temp;
                        m_temp = in_file.get();
                        m_in += m_temp;
                        stringScan = false;
                    }
                    else
                    {
                        m_in += m_temp;
                    }
                }
          }
          scancomment2(m_in, eofFound, currLineNumberCatchUp);
          scan_new_line();
    }
    else // scan line comment
    {
        string m_in(1,curr_char);
        scanLineComment(m_in);
    }
}

void Scanner::scan_other()
{
    if(isalnum(curr_char))
    {
          scan_ident();
    }
    else if (curr_char != '\n' && !in_file.eof())
    {
        string temp(1,curr_char);
        token_vector.push_back(Token(UNDEFINED, temp, curr_line_num));
    }
}
