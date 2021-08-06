#ifndef __LEXER__H__
#define __LEXER__H__

#include <vector>
#include <string>

#include "inputbuf.h"

// ------- token types -------------------

typedef enum { END_OF_FILE = 0,
    PUBLIC, PRIVATE, EQUAL, COLON, COMMA, SEMICOLON,
       LBRACE, RBRACE, ID, ERROR 
   } TokenType;

class Token {
  public:
    void Print();

    std::string lexeme;
    TokenType token_type;
    int line_no;
};

class LexicalAnalyzer {
  public:
    Token GetToken();
    TokenType UngetToken(Token);
    LexicalAnalyzer();
	
	
	int public_vars_parse(void); 
	int varlist_parse(void);
	int private_vars_parse(void);
	int scope_parse(void);
	int stmt_list_parse(void);
	int global_vars_parse(void);
	int stmt_parse(void);
	int program_parse(void);
	


  private:
    std::vector<Token> tokens;
    int line_no;
    Token tmp;
    InputBuffer input;

    bool SkipSpace();
    bool IsKeyword(std::string);
    TokenType FindKeywordIndex(std::string);
    Token ScanIdOrKeyword();
    Token ScanNumber();
	bool Skip();
};

#endif  //__LEXER__H__
