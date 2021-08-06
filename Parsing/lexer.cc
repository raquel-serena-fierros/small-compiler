#include <iostream>
#include <algorithm>
#include <istream>
#include <cctype>
#include <vector>
#include <string>
#include <cctype>
#include <string.h>
#include <stack>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "inputbuf.h"

using namespace std;

string reserved[] = { "END_OF_FILE",
    "PUBLIC", "PRIVATE", "EQUAL", "COLON", "COMMA",
     "SEMICOLON", "LBRACE", "RBRACE", "ID", "ERROR" 
};


#define KEYWORDS_COUNT 2
string keyword[] = {"public", "private"};

void Token::Print()
{
    cout << "{" << this->lexeme << " , "
         << reserved[(int) this->token_type] << " , "
         << this->line_no << "}\n";
}


struct symTableElement
{
	
	int x; 		
	char *c;  
	char *scope;

};

struct symTable
{
	symTableElement *element;
	symTable *next;
	symTable *prev;
}; 

struct syml
{
	
	char *cscope;
	char *cscope1;
	string r;
	string l;
};

int y = 0; 
char *symScope = "::";

struct symTable *temp; 
struct symTable *newN;
struct symTable *sTable;

vector<struct syml> symlist; 

void print(Token t1, Token t2)
{

	char* cscope = (char*)malloc(sizeof(char)*20);			
	char* cscope1 = (char*)malloc(sizeof(char)*20);
	struct symTable* temp1 = newN;
	struct symTable* temp2 = newN;
	
	while(temp1 != NULL){
	   if(temp1->element->c == t1.lexeme){
	   if(temp1->element->x == 2 && symScope != temp1->element->scope){
	        temp1= temp1->prev;
	        continue;
	    }else{
	        cscope = temp1->element->scope;
	        break;
	    }
	   }
	   temp1= temp1->prev;
   
	}
	while(temp2 != NULL){
	   if(temp2->element->c == t2.lexeme){
	    if(temp2->element->x == 2 && symScope != temp2->element->scope){
	    temp2 = temp2->prev;
	    continue;
        
	    }else{
	        cscope1 = temp2->element->scope;
	        break;
	    }
	   }
	   temp2 = temp2->prev;
	}

	if(strlen(cscope) == 0) {
	 cscope = new char[2];
	 cscope[0] = '?';
	 cscope[1] = '\0';
	}
	if(strlen(cscope1) == 0){
	cscope1 = new char[2];
	cscope1[0] = '?';
	cscope1[1] = '\0';
	} 
	struct syml tnode;
	tnode.l = t1.lexeme;
	tnode.r = t2.lexeme;
	tnode.cscope = cscope;
	tnode.cscope1 = cscope1;
	symlist.push_back(tnode);

	}


	void print()
	{
	for(int i=0; i < symlist.size(); i++)
	{
	    cout << symlist[i].cscope;
	    if(symlist[i].cscope != "::") 
	        cout << "."; 
	    cout<< symlist[i].l << " = " << symlist[i].cscope1;
	    if(symlist[i].cscope1 != "::") 
	        cout<<".";
	    cout<< symlist[i].r << endl;
	    }
	}
	
	
	void addElement(char *lexeme)
	{ 
	 if(sTable == NULL){ 
	 sTable = (symTable*) malloc (sizeof(symTable));
	 struct symTableElement* newElement = (symTableElement*) malloc (sizeof(symTableElement));
	 sTable ->element = newElement;
	 sTable-> next = NULL;   
	 sTable-> prev = NULL;
  
	 int z = strlen(lexeme) ; 
	 sTable->element->c = new char[z+1];
	 strcpy(sTable->element->c,lexeme);
	 sTable->element->c[z] = '\0'; 
	 sTable->element->scope = symScope;
	 sTable->element->x = y; 
 
	 newN = sTable;
	 temp = sTable; 
	 }
	 else 
	 { 
	 temp = sTable; 
	 while (temp->next != NULL){ 
	 temp = temp-> next;
	 }
	 newN = (symTable*) malloc (sizeof(symTable));
	 struct symTableElement *newElement = (symTableElement*) malloc(sizeof(symTableElement));
	 newN->element = newElement;
	 newN->next = NULL;
	 newN->prev = temp;
	 temp->next = newN; 
	 int i = strlen(lexeme) ;
	 newN ->element->c = new char[i+1];
	 strcpy(newN ->element->c, lexeme);
	 newN ->element ->c[i] = '\0';
	 newN ->element-> scope = symScope;
	 newN ->element->x = y;

	 }
 
	}
	
	

	void deleteElement(char *lexeme)
	{
	  if(sTable == NULL)
	  {
	  	return;
	  }
	   while(newN->element->scope == lexeme){   
	   temp->next = NULL;
	   newN ->prev = NULL;
	   if(temp->prev != NULL){
	   temp = temp->prev;
	   newN = temp->next;
	   newN->next = NULL;
	   return;
	   }
	   else if(temp == newN)
	   {
	   newN = NULL;
	   temp = NULL;
	   return;
	   }
	   else
	   {
	   temp->next = NULL;
	   newN ->prev = NULL;
	   newN=temp;
	   temp->next =NULL;
	   }
	   }
	   symScope = newN->element->scope;
	}

	
LexicalAnalyzer::LexicalAnalyzer()
{
    this->line_no = 1;
    tmp.lexeme = "";
    tmp.line_no = 1;
    tmp.token_type = ERROR;
}


LexicalAnalyzer lexer;

Token t1,t2,t3;

int LexicalAnalyzer::program_parse(void)
{

 t1 = lexer.GetToken();

 if(t1.token_type == ID) 
 {
   t2 = lexer.GetToken();
   if(t2.token_type == SEMICOLON || t2.token_type == COMMA)
   {
   lexer.UngetToken(t2);
   lexer.UngetToken(t1);
   lexer.global_vars_parse();
   lexer.scope_parse();

   }
   else if(t2.token_type == LBRACE)
   {
   lexer.UngetToken(t2);
   lexer.UngetToken(t1);
   lexer.scope_parse();

   }
   else {
   exit(1);
   }
 } 
 else
 {	 
 exit(1);
  }
  
  return 0;
}


int LexicalAnalyzer::global_vars_parse(void){

t1 = lexer.GetToken();
if(t1.token_type == ID)
{
 lexer.UngetToken(t1);
 lexer.varlist_parse();
 t1 = lexer.GetToken();
   if(t1.token_type != SEMICOLON)
   {
   	exit(1); 
   }   
 }
else
 {
	 exit(1);
}
return 0;
}



int LexicalAnalyzer::scope_parse(void)
{
t1 = lexer.GetToken();

 if(t1.token_type == ID)
 {
 string str = t1.lexeme; 
 
   symScope = const_cast<char *>(str.c_str());  
   t1= lexer.GetToken();   
  if(t1.token_type == LBRACE)
  {   
   lexer.public_vars_parse();
   lexer.private_vars_parse();
   lexer.stmt_list_parse();
   t1 = lexer.GetToken();
    if(t1.token_type == RBRACE)
	{
    deleteElement(symScope);
    t1=lexer.GetToken();
       if(t1.token_type == END_OF_FILE)
	   {
       deleteElement(symScope);
       }
       else{
       UngetToken(t1);
       }
    return 0; 
    }
    else {
    exit(1);
    }
   }
  else{
  exit(1);
  }
 }
 else{
 exit(1);
 }
return 0; 
}


int LexicalAnalyzer::public_vars_parse(void)
{
t1 = lexer.GetToken();
 if(t1.token_type == PUBLIC)
 {
  y = 1;
  t1 = lexer.GetToken();
   if(t1.token_type == COLON)
   {
    t1 = lexer.GetToken(); 
    if(t1.token_type == ID)
	{
    lexer.UngetToken(t1);
    lexer.varlist_parse();
    t1 = lexer.GetToken();
      if(t1.token_type == SEMICOLON)
	  {
      }
      else{
      exit(1);
      }
     }
    else{
     exit(1);
     }
    }
   else{
    exit(1);
    }
   }
  else if(t1.token_type == ID || t1.token_type == PRIVATE)
  {
  lexer.UngetToken(t1);
   }
  else{
  exit(1);
  }
  return 0;
}


int LexicalAnalyzer::private_vars_parse(void)
{
	
 t1= lexer.GetToken();
 if(t1.token_type == PRIVATE)
 {
	y = 2;
 	t1 = lexer.GetToken();
   	if(t1.token_type == COLON)
	{
     t1 =lexer.GetToken();
    if(t1.token_type == ID)
	{
     lexer.UngetToken(t1);
     lexer.varlist_parse();
     t1 = lexer.GetToken();
     if(t1.token_type == SEMICOLON)
	 {
     }
     else{
     exit(1);
     }
     }
    else{
    exit(1);
    }
   }
   else{
   exit(1);
   }
  
 }
 else if(t1.token_type == ID) 
 {
 lexer.UngetToken(t1);
 }
 else{
	 exit(1);
 }
 return 0;
}


int LexicalAnalyzer::varlist_parse(void)
{

  t1 = lexer.GetToken();
  char* lexeme = (char*) malloc (sizeof (t1.lexeme)); 
  memcpy(lexeme,(t1.lexeme).c_str(),sizeof(t1)); 
  addElement(lexeme); 
  
  symTable* s1 = sTable; 
  while(s1 != NULL){
   s1 = s1 ->next; 
  }
  if(t1.token_type ==ID){
    t1 = lexer.GetToken();
     if(t1.token_type == COMMA)
	 {    
       lexer.varlist_parse();
       return 0;        
       }
     else if(t1.token_type == SEMICOLON)
	 {
       UngetToken(t1);
       return 0;
       }
     else{
      exit(1);
     }
  }
  else {
   exit(1);
  }  
}



int LexicalAnalyzer::stmt_parse(void)
{
  t1 = lexer.GetToken();
  if(t1.token_type == ID)
  {
    t2 = lexer.GetToken();
     if(t2.token_type == EQUAL)
	 {     
      t3 = lexer.GetToken();
       if(t3.token_type == ID) 
	   {
        print(t1, t3);
        
        t1 = lexer.GetToken();
          if(t1.token_type == SEMICOLON)
		  {
          return 0;
          }
          else{
           exit(1);
          }
       }
       else {
       exit(1);
       }
      }
     else if(t2.token_type == LBRACE)
	 {
     symScope = const_cast<char*>((t1.lexeme).c_str()); 
     lexer.UngetToken(t2);
     lexer.UngetToken(t1);
     lexer.scope_parse();     
     }
     else {
     exit(1);
     }
  }
  
  else{
  exit(1);
  }
  return 0;
}



int LexicalAnalyzer::stmt_list_parse(void)
{
 t1 = lexer.GetToken();
   if(t1.token_type == ID)
   {
     lexer.UngetToken(t1);
      lexer.stmt_parse();
       t2 = lexer.GetToken();
       if(t2.token_type == ID)
	   {
       lexer.UngetToken(t2);
       lexer.stmt_list_parse();
       return 0;
       }
       else if(t2.token_type == RBRACE)
	   {      
       lexer.UngetToken(t2);       
       return 0;
       }  
       else {
       exit(1);
       }
    }
    else {
    exit(1);
    }
}


bool LexicalAnalyzer::Skip()
{
 
  char ch;
  bool com = false;
  input.GetChar(ch);
  if(input.EndOfInput())
  {
  input.UngetChar(ch);
  return com;
  }
    if(ch =='/')
	{
    input.GetChar(ch);
         if(ch =='/')
		 {
         com = true;
           while (ch != '\n')
		   {
           com = true;
           input.GetChar(ch);
           }
         line_no += 1;
         Skip();      
         }
         else{
         com = false;
         exit(1);
         }
      }
     else 
	 {
     input.UngetChar(ch);    
     return com;
     }
}


bool LexicalAnalyzer::SkipSpace()
{
    char ch;
    bool space_encountered = false;

    input.GetChar(ch);
    line_no += (ch == '\n');

    while (!input.EndOfInput() && isspace(ch)) {
        space_encountered = true;
        input.GetChar(ch);
        line_no += (ch == '\n');
    }

    if (!input.EndOfInput()) {
        input.UngetChar(ch);
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



Token LexicalAnalyzer::ScanIdOrKeyword()
{
    char ch;
    input.GetChar(ch);

    if (isalpha(ch)) {
        tmp.lexeme = "";
        while (!input.EndOfInput() && isalnum(ch)) 
		{
            tmp.lexeme += ch;
            input.GetChar(ch);
        }
        if (!input.EndOfInput()) {
            input.UngetChar(ch);
        }
        tmp.line_no = line_no;     
        if (IsKeyword(tmp.lexeme))
		{          
            tmp.token_type = FindKeywordIndex(tmp.lexeme);           
            }
        else
            tmp.token_type = ID;
    } else {
        if (!input.EndOfInput()) {
            input.UngetChar(ch);
        }
        tmp.lexeme = "";
        tmp.token_type = ERROR;
    }
    return tmp;
}


TokenType LexicalAnalyzer::UngetToken(Token tok)
{
    tokens.push_back(tok);;
    return tok.token_type;
}

Token LexicalAnalyzer::GetToken()
{
    char ch;

    // if there are tokens that were previously
    // stored due to UngetToken(), pop a token and
    // return it without reading from input
    if (!tokens.empty()) {
        tmp = tokens.back();
        tokens.pop_back();
        return tmp;
    }

    SkipSpace();
	Skip();
	SkipSpace();
    tmp.lexeme = "";
    tmp.line_no = line_no;
    input.GetChar(ch);
    switch (ch) {
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
         case '{':
             tmp.token_type = LBRACE;
             return tmp;
         case '}':
             tmp.token_type = RBRACE;
             return tmp;
        
         default:
             if (isdigit(ch)){
               input.UngetChar(ch);
               tmp.token_type = ERROR;
               return tmp;
              }            
             else if (isalpha(ch)) 
			 {
             input.UngetChar(ch);
             return ScanIdOrKeyword();
             }
             else if (input.EndOfInput())
             tmp.token_type = END_OF_FILE;
               
             else
			 {
             tmp.token_type = ERROR;
             }

             return tmp;
     }
 }

int main()
{
    int run;
	run = lexer.program_parse();
    print();
  
}
