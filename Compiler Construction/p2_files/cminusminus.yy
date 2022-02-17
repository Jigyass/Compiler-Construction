%skeleton "lalr1.cc"
%require "3.0"
%debug
%defines
%define api.namespace{cminusminus}
%define api.parser.class {Parser}
%define parse.error verbose
%output "parser.cc"
%token-table

%code requires{
	#include <list>
	#include "tokens.hpp"
	namespace cminusminus {
		class Scanner;
	}

//The following definition is required when 
// we don't use the %locations directive (which we won't)
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

//End "requires" code
}

%parse-param { cminusminus::Scanner &scanner }
%code{
   // C std code for utility functions
   #include <iostream>
   #include <cstdlib>
   #include <fstream>

   // Our code for interoperation between scanner/parser
   #include "scanner.hpp"

  //Request tokens from our scanner member, not 
  // from a global function
  #undef yylex
  #define yylex scanner.yylex
}

/*
The %union directive is a way to specify the 
set of possible types that might be used as
translation attributes on a symbol.
For this project, only terminals have types (we'll
have translation attributes for non-terminals in the next
project)
*/
%union {
   cminusminus::Token* lexeme;
   cminusminus::Token* transToken;
   cminusminus::StrToken* transStrToken;
   cminusminus::IntLitToken* transIntToken;
   cminusminus::ShortLitToken* transShortToken;
   cminusminus::Position* transPosition;
}

%define parse.assert

/* Terminals 
 *  No need to touch these, but do note the translation type
 *  of each node. Most are just "transToken", which is defined in
 *  the %union above to mean that the token translation is an instance
 *  of cminusminus::Token *, and thus has no fields (other than line and column).
 *  Some terminals, like ID, are "transIDToken", meaning the translation
 *  also has a name field. 
*/
%token                   END	   0 "end file"
%token	<transToken>     AMP
%token	<transToken>     AND
%token	<transToken>     ASSIGN
%token	<transToken>     AT
%token	<transToken>     BOOL
%token	<transToken>     COMMA
%token	<transToken>     DEC
%token	<transToken>     DIVIDE
%token	<transToken>     ELSE
%token	<transToken>     EQUALS
%token	<transToken>     FALSE
%token	<transToken>     GREATER
%token	<transToken>     GREATEREQ
%token	<transIDToken>   ID
%token	<transToken>     IF
%token	<transToken>     INC
%token	<transToken>     INT
%token	<transIntToken>  INTLITERAL
%token	<transToken>     LCURLY
%token	<transToken>     LESS
%token	<transToken>     LESSEQ
%token	<transToken>     LPAREN
%token	<transToken>     MINUS
%token	<transToken>     NOT
%token	<transToken>     NOTEQUALS
%token	<transToken>     OR
%token	<transToken>     PLUS
%token	<transToken>     PTR
%token	<transToken>     READ
%token	<transToken>     RETURN
%token	<transToken>     RCURLY
%token	<transToken>     RPAREN
%token	<transToken>     SEMICOL
%token	<transToken>     SHORT
%token	<transShortToken> SHORTLITERAL
%token	<transToken>     STRING
%token	<transStrToken>  STRLITERAL
%token	<transToken>     TIMES
%token	<transToken>     TRUE
%token	<transToken>     VOID
%token	<transToken>     WHILE
%token	<transToken>     WRITE

%type <transPosition> program
%type <transPosition> globals
%type <transPosition> decl

/* NOTE: Make sure to add precedence and associativity 
 * declarations
*/

%right ASSIGN
%left AND
%left OR
%nonassoc LESS LESSEQ GREATER GREATEREQ NOTEQUALS EQUALS
%left MINUS
%left PLUS
%left TIMES
%left DIVIDE
%left NOT

%%

/* TODO: add productions for the other nonterminals in the 
   grammar and make sure that all of the productions of the 
   given nonterminals are complete
*/
program 	: globals
		  {
		  }
globals 	: globals decl 
	  	  { 
	  	  }
		| /* epsilon */
		  { 
		  }

decl 		: varDecl SEMICOL {}
		   | fnDecl {}

varDecl 	: type id
		  {
		  }

type 		: INT {}
		   | BOOL {}
		   | STRING {}
		   | SHORT {}
		   |VOID {}
         |PTR type {}

fnDecl		:type id LPAREN formals RPAREN fnBody {}

formals		: formalDecl {}
            | formals COMMA formalDecl {}
            |{}

formalDecl  : type id {}


		
		


fnBody		: LCURLY stmtList RCURLY {}

stmtList	: stmtList stmt{}
		   | {}

stmt		: varDecl SEMICOL {}
         | assignExp SEMICOL {}
         | lval DEC SEMICOL {}
         | lval INC SEMICOL {}
         | READ lval SEMICOL {}
         | WRITE exp SEMICOL {}
         | IF LPAREN exp RPAREN LCURLY stmtList RCURLY {}
         | IF LPAREN exp RPAREN LCURLY stmtList RCURLY ELSE LCURLY stmtList RCURLY {}
         | WHILE LPAREN exp RPAREN LCURLY stmtList RCURLY {}
         | RETURN exp SEMICOL {}
         | RETURN SEMICOL {}
         | callExp SEMICOL {}



assignExp	: lval ASSIGN exp {}

exp		: assignExp {}
         | exp MINUS exp {}
         | exp PLUS exp {}
         | exp TIMES exp {}
         | exp DIVIDE exp {}
         | exp AND exp {}
         | exp OR exp {}
         | exp EQUALS exp {}
         | exp NOTEQUALS exp {}
         | exp GREATER exp {}
         | exp GREATEREQ exp {}
         | exp LESS exp {}
         | exp LESSEQ exp {}
         | NOT exp {}
         | MINUS term {}
         | term {}

term		: lval {}
         | INTLITERAL {}
         | SHORTLITERAL {}
         | STRLITERAL {}
         | AMP id {}
         | TRUE {}
         | FALSE {}
         | LPAREN exp RPAREN {}
         | callExp {}

lval		: id {}
         | AT id {}

id 		: ID{}

callExp		: id LPAREN RPAREN {}  // fn call with no args
            | id LPAREN actualsList RPAREN {} // with args

actualsList	: exp {}
		      |actualsList COMMA exp {}




 /* TODO: add productions for the entire grammar of the language */
	
%%

void cminusminus::Parser::error(const std::string& msg){
	std::cout << msg << std::endl;
	std::cerr << "syntax error" << std::endl;
}
