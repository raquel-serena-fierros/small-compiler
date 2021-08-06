
#include <iostream>
#include <algorithm>
#include <istream>
#include <vector>
#include <string>
#include <cctype>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;



string reserved[] = { "END_OF_FILE",
    "IF", "WHILE", "DO", "THEN", "PRINT",
    "PLUS", "MINUS", "DIV", "MULT",
    "EQUAL", "COLON", "COMMA", "SEMICOLON",
    "LBRAC", "RBRAC", "LPAREN", "RPAREN",
    "NOTEQUAL", "GREATER", "LESS", "LTEQ", "GTEQ",
    "DOT", "NUM", "ID", "ERROR", "REALNUM", "BASE08NUM", "BASE16NUM" 
};

int dnum = 0;


#define KEYWORDS_COUNT 5
string keyword[] = { "IF", "WHILE", "DO", "THEN", "PRINT" };

Token pToken;
void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}

LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}

bool LexicalAnalyzer::SkipSpace()
{
    char c;
    bool space_encountered = false;

    input.GetChar(c);
    line_no += (c == '\n');

    while (!input.EndOfInput() && isspace(c)) {
        space_encountered = true;
        input.GetChar(c);
        line_no += (c == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(c);
    }
    return space_encountered;
}

bool LexicalAnalyzer::IsKeyword(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return true;
        }
    }
    return false;
}

TokenType LexicalAnalyzer::FindKeywordIndex(string s)
{
    for (int i = 0; i < KEYWORDS_COUNT; i++) {
        if (s == keyword[i]) {
            return (TokenType) (i + 1);
        }
    }
    return ERROR;
}

Token LexicalAnalyzer::ScanNumber()
{
    char c;

    input.GetChar(c);
    if (isdigit(c)) {
        if (c == '0' && !dnum) {
            tmp.lexeme = "0";
        } else {
            tmp.lexeme = "";
            while (!input.EndOfInput() && isdigit(c)) {
                tmp.lexeme += c;
                input.GetChar(c);
            }
            if (!input.EndOfInput()) {
                input.UngetChar(c);
            }
        }
        // TODO: You can check for REALNUM, BASE08NUM and BASE16NUM here!
        tmp.token_type = NUM;
        tmp.line_no = line_no;
        return tmp;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
        tmp.line_no = line_no;
        return tmp;
    }
}

Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char c;
    input.GetChar(c);

    if (isalpha(c)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(c)) {
			Token temp; 
            tmp.lexeme += c;
            input.GetChar(c);
			
			int i = 0;
			if(tmp.lexeme[i+2] == '8' && tmp.lexeme[i] == 'x' && tmp.lexeme[i+1] == '0')
			{
				input.UngetChar(c);
				temp.lexeme = "x08"; 
				temp.token_type = ID;
				temp.line_no = line_no;

				return temp;
				
			}
        }
		
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.line_no = line_no;
        if (IsKeyword(tmp.lexeme))
            tmp.token_type = FindKeywordIndex(tmp.lexeme);
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(c);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}

// you should unget tokens in the reverse order in which they
// are obtained. If you execute
//
//    t1 = lexer.GetToken();
//    t2 = lexer.GetToken();
//    t3 = lexer.GetToken();
//
// in this order, you should execute
//
//    lexer.UngetToken(t3);
//    lexer.UngetToken(t2);
//    lexer.UngetToken(t1);
//
// if you want to unget all three tokens. Note that it does not
// make sense to unget t1 without first ungetting t2 and t3
//
TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char c;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(c);
    switch (c) {
        case '.':
            tmp.token_type = DOT;
            return tmp;
        case '+':
            tmp.token_type = PLUS;
            return tmp;
        case '-':
            tmp.token_type = MINUS;
            return tmp;
        case '/':
            tmp.token_type = DIV;
            return tmp;
        case '*':
            tmp.token_type = MULT;
            return tmp;
        case '=':
            tmp.token_type = EQUAL;
            return tmp;
        case ':':
            tmp.token_type = COLON;
            return tmp;
        case ',':
            tmp.token_type = COMMA;
            return tmp;
        case ';':
            tmp.token_type = SEMICOLON;
            return tmp;
        case '[':
            tmp.token_type = LBRAC;
            return tmp;
        case ']':
            tmp.token_type = RBRAC;
            return tmp;
        case '(':
            tmp.token_type = LPAREN;
            return tmp;
        case ')':
            tmp.token_type = RPAREN;
            return tmp;
        case '<':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = LTEQ;
            } else if (c == '>') {
                tmp.token_type = NOTEQUAL;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = LESS;
            }
            return tmp;
        case '>':
            input.GetChar(c);
            if (c == '=') {
                tmp.token_type = GTEQ;
            } else {
                if (!input.EndOfInput()) {
                    input.UngetChar(c);
                }
                tmp.token_type = GREATER;
            }
            return tmp;
        default:
		
			Token curr; 
            if (isdigit(c)) {
				
				Token tok, tok1, tok2, tok3, tok4; 
				Token character; 
				string str = ""; 
				string str1 = ""; 
				
                input.UngetChar(c);
				
				pToken = curr;
				
				character = GetToken(); 
				
				if(character.token_type == ID)
				{
					
					if(!dnum && character.lexeme == "x08")
					{
						if(curr.lexeme == "0")
						{
							tok3.lexeme = tok3.lexeme + curr.lexeme;
							tok3.lexeme = tok3.lexeme + character.lexeme.substr(0,3); 
							tok3.token_type = BASE08NUM;
							tok3.line_no = line_no;
							return tok3; 
						}
						else if(curr.lexeme.length() == 0)
						{
							return character; 
						}
						
						else 
						{
							for(int i = curr.lexeme.length()-1; i >= 0; i--)
							{
								if(curr.lexeme[i]-'7' <= 0 && curr.lexeme[i]-'0' >= 0)
								{
									str = str + curr.lexeme[i]; 
								}
								else
								{
									if(str.length() == 0)
									{
										curr.token_type = NUM;
										curr.line_no = line_no;
										
										for(int i = character.lexeme.length()-1; i >= 0; i--)
										{
											input.UngetChar(i);
										}
										return curr; 
									}
									
									tok1.lexeme = curr.lexeme.substr(0, i+1);
									tok1.token_type = NUM;
									tok1.line_no = line_no;
									
									for(i = str.length()-1; i>0; i--)
									{
										c = str[i];
										input.UngetChar(c);
									}
									
									return tok1;
								}
							}
						}
						
						std::reverse(str.begin(), str.end()); 
						
						tok2.lexeme = str;
						
						tok3.lexeme += tok2.lexeme;
						
						tok3.lexeme += character.lexeme;
						
						tok3.token_type = BASE08NUM;
						
						tok3.line_no = line_no;
						
						return tok3; 
					}
						
						else if(character.lexeme == "x16")
						{
							if(curr.lexeme.length() != 0)
							{
								tok4.lexeme = curr.lexeme;
								tok4.lexeme = tok4.lexeme + character.lexeme;
								tok4.token_type = BASE16NUM;
								tok4.line_no = line_no;
								
								return tok4;
							}
							
							else
							{
								return character; 
							}
						}
						
						else
						{
							UngetToken(character); 
							return curr; 
						}
					}
					
					else if(character.token_type == DOT)
					{
						dnum = 1;
						
						UngetToken(character);
						
						return ScanRealNum(curr.lexeme);
					}
					
					else if(character.token_type == BASE16NUM)
					{
						
						if(curr.lexeme.length() != 0 && curr.lexeme[0] != '0')
						{
							
							tok4.lexeme = curr.lexeme;
							tok4.lexeme = tok4.lexeme + character.lexeme;
							tok4.token_type = BASE16NUM;
							tok4.line_no = line_no;
							
							return tok4;
						
						}
						
						else if(curr.lexeme == "")
						{
							
							character.token_type = ID;
							character.line_no = line_no;
							return character;
						}
						
						else
						{
							UngetToken(character);
							return curr;
						}
					
					}
					
					else
					{
						
						UngetToken(character);
						return curr;
					
					}
						
				
				}
				
				else if(isalpha(c))
				{
					char ch,ch1; 
					Token tok5, tok6;
					string str2;
					
					if( c == 'F' || c == 'E' || c == 'D' || c == 'A' || c == 'B' || c == 'C')
					{
						tok6 = B16NUM(); 
						input.UngetChar(c);
						
						if(pToken.lexeme == "")
						{
							tok6.token_type = ID;
							return tok6;
						}
						else
						{
							return tok6;
						}
					}
						
						else
						{
							input.UngetChar(c);
							
							return ScanIdOrKeyword();
						}
					
				
            } else if (input.EndOfInput())
                tmp.token_type = END_OF_FILE;
            else
                tmp.token_type = ERROR;

            return tmp;
    }
}


Token LexicalAnalyzer::B16NUM()
{
	
	
	Token tok7, tok8;
	
	string str = "";
	
	char ch1,ch2,ch3,ch4; 
	
	
	input.GetChar(ch1);
	while(!input.EndOfInput())
	{
		if(!isalnum(ch1))
		{
			input.UngetChar(ch1);
		}
		
		str += ch1;
		input.GetChar(ch1);
		
		
		if(ch1 == 'x')
		{
			str += ch1;
			input.GetChar(ch3);
			
			if(ch3 == '1')
			{
				str += ch3;
				input.GetChar(ch4);
				
				if(ch4 == '6')
				{
					str += ch4;
					tok7.lexeme = str;
					tok7.token_type = BASE16NUM;
					tok7.line_no = line_no;
					
					return tok7;
				}
				
				else
				{
					input.UngetChar(ch4);
					input.UngetChar(ch3);
				}
			}
			
			else
			{
				input.UngetChar(ch3);
			}
			
		}
		
	}
	
	input.UngetChar(ch1);
	
	
	tok8.lexeme = str;
	tok8.token_type = ID;
	tok8.line_no = line_no;
	return tok8;

}



Token LexicalAnalyzer::ScanRealNum(string curr)
{
	Token tok = GetToken();
	char ch;
	
	if(tok.token_type = DOT)
	{
		
		Token tok1 = GetToken();
		
		dnum = 0;
		
		
		if(tok1.token_type == NUM)
		{
			
			tmp.lexeme = curr; 
			tmp.lexeme += ".";
			tmp.lexeme += tok1.lexeme;
			input.GetChar(ch);
			
			
			while(isdigit(ch) && !input.EndOfInput())
			{
				
				tmp.lexeme += ch;
				input.GetChar(ch);
			}
			
			if(!input.EndOfInput())
			{
				input.UngetChar(ch);
			}
			
			tmp.token_type = REALNUM;
			tmp.line_no = tok1.line_no;
			return tmp;
		}
		
		else
		{
			UngetToken(tok1);
			UngetToken(tok);
			tmp.lexeme = curr;
			tmp.token_type = NUM;
			tmp.line_no = line_no;
			return tmp; 
		}
	}

}


int main()
{
    LexicalAnalyzer lexer;
    Token token;

    token = lexer.GetToken();
    token.Print();
    while (token.token_type != END_OF_FILE)
    {
        token = lexer.GetToken();
        token.Print();
    }
}

