%{
#include <string>
#include <limits.h>

/* Get our custom yyFlexScanner subclass */
#include "scanner.hpp"
#undef YY_DECL
#define YY_DECL int cminusminus::Scanner::yylex(cminusminus::Parser::semantic_type * const lval)

using TokenKind = cminusminus::Parser::token;

/* define yyterminate as returning an EOF token (instead of NULL) */
#define yyterminate() return ( TokenKind::END )

/* exclude unistd.h for Visual Studio compatibility. */
#define YY_NO_UNISTD_H

#define EXIT_ON_ERR 0


%}

%option nodefault
%option yyclass="cminusminus::Scanner"
%option noyywrap
%option c++

DIGIT [0-9]
WHITESPACE [\040\t]
LETTER [a-zA-Z]
ESCAPEE [nt'"\\]

 /* */ 
BADESC [\\][^nt"\\\n]?
STRELT ([\\][nt"\\])|([^\\\n"])

%%
%{
	this->yylval = lval;
%}

int    		      { return makeBareToken(TokenKind::INT); }
[@]    		      { return makeBareToken(TokenKind::AT); }
[&]    		      { return makeBareToken(TokenKind::AMP); }
bool 		      { return makeBareToken(TokenKind::BOOL); }
short	      	      { return makeBareToken(TokenKind::SHORT); }
ptr	      	      { return makeBareToken(TokenKind::PTR); }
string	      	      { return makeBareToken(TokenKind::STRING); }
void 		      { return makeBareToken(TokenKind::VOID); }
if  		      { return makeBareToken(TokenKind::IF); }
else		      { return makeBareToken(TokenKind::ELSE); }
while		      { return makeBareToken(TokenKind::WHILE); }
return		    { return makeBareToken(TokenKind::RETURN); }
write		    { return makeBareToken(TokenKind::WRITE); }
read		    { return makeBareToken(TokenKind::READ); }
false  		    { return makeBareToken(TokenKind::FALSE); }
true 		    { return makeBareToken(TokenKind::TRUE); }
"{"		        { return makeBareToken(TokenKind::LCURLY); }
"}"		        { return makeBareToken(TokenKind::RCURLY); }
"("		        { return makeBareToken(TokenKind::LPAREN); }
")"		        { return makeBareToken(TokenKind::RPAREN); }
";"		        { return makeBareToken(TokenKind::SEMICOL); }
","		        { return makeBareToken(TokenKind::COMMA); }
"++"          { return makeBareToken(TokenKind::INC); }
"+"           { return makeBareToken(TokenKind::PLUS); }
"--"          { return makeBareToken(TokenKind::DEC); }
"-"		        { return makeBareToken(TokenKind::MINUS); }
"*"		        { return makeBareToken(TokenKind::TIMES); }
"/"		        { return makeBareToken(TokenKind::DIVIDE); }
"!"	 	        { return makeBareToken(TokenKind::NOT); }
"and"          { return makeBareToken(TokenKind::AND); }
"or"          { return makeBareToken(TokenKind::OR); }
"=="          { return makeBareToken(TokenKind::EQUALS); }
"!="          { return makeBareToken(TokenKind::NOTEQUALS); }
"<"	      { return makeBareToken(TokenKind::LESS); }
"<="          { return makeBareToken(TokenKind::LESSEQ); }
">"		        { return makeBareToken(TokenKind::GREATER); }
">="          { return makeBareToken(TokenKind::GREATEREQ); }
"="		        { return makeBareToken(TokenKind::ASSIGN); }
"gets"		        { return makeBareToken(TokenKind::ASSIGN); }
({LETTER}|_)({LETTER}|{DIGIT}|_)* { 
			  Position * pos = new Position(lineNum, colNum,
				lineNum, colNum + yyleng);
		            yylval->transToken = 
		            new IDToken(pos, yytext);
		            colNum += yyleng;
		            return TokenKind::ID; }

{DIGIT}+	    { double asDouble = std::stod(yytext);
			          int intVal = atoi(yytext);
			          bool overflow = false;
			          bool underflow = false;
			          if (asDouble > INT_MAX){ overflow = true; }
			          if (asDouble < INT_MIN){ underflow = true; }

								std::string str = yytext;
				  			std::string suffix = "";
				  			for(int i = 0 ; i < str.length(); i++){
									if (str[i] != '0'){
										suffix = str.substr(i, std::string::npos);
										break;
									}
				  			}
			          if (suffix.length() > 10){ overflow = true; }

			          if (overflow){
										Position pos(lineNum,colNum,lineNum,colNum+yyleng);
				            errIntOverflow(&pos);
					    intVal = 0;
			          }
								if (underflow){
										Position pos(lineNum,colNum,lineNum,colNum+yyleng);
				            errIntUnderflow(&pos);
					    intVal = 0;
								}
				  			Position * pos = new Position(lineNum, colNum,
									lineNum, colNum + yyleng);
			          yylval->transToken = 
			              new IntLitToken(pos, intVal);
			          colNum += yyleng;
			          return TokenKind::INTLITERAL; }


{DIGIT}+"S"	    { 
								std::string str = yytext;
								str = str.substr(0, str.length() - 1);
							  int intVal = atoi(yytext);
                bool overflow = false;
                bool underflow = false;
		      			if (intVal > 32767){
									overflow = true; 
								}
								if (intVal < -32768){
									underflow = true; 
								}

				  			std::string suffix = "";
				  			for(int i = 0 ; i < str.length(); i++){
									if (str[i] != '0'){
										suffix = str.substr(i, std::string::npos);
										break;
									}
				  			}
			          if (suffix.length() > 10){ overflow = true; }

			          if (overflow){
										Position pos(lineNum,colNum,lineNum,colNum+yyleng);
				            errShortOverflow(&pos);
					    intVal = 0;
			          }
								if (underflow){
										Position pos(lineNum,colNum,lineNum,colNum+yyleng);
				            errShortUnderflow(&pos);
					    intVal = 0;
								}

				  			Position * pos = new Position(lineNum, colNum,
									lineNum, colNum + yyleng);
			          yylval->transToken = 
			              new ShortLitToken(pos, intVal);
			          colNum += yyleng;
			          return TokenKind::SHORTLITERAL; }

\"{STRELT}*\" {
			Position * pos;
			pos = new Position(lineNum, colNum, lineNum, colNum + yyleng);
   		          yylval->transToken = 
                    new StrToken(pos, yytext);
		            this->colNum += yyleng;
		            return TokenKind::STRLITERAL; }

\"{STRELT}* {
			Position pos(lineNum, colNum, lineNum, colNum + yyleng);
		            errStrUnterm(&pos);
		            colNum += yyleng; /*Upcoming \n resets lineNum */
			    #if EXIT_ON_ERR
			    exit(1);
			    #endif
		            }

["]({STRELT}*{BADESC}{STRELT}*)+(\\["])? {
                // Bad, unterm string lit
		Position pos(lineNum,colNum,lineNum,colNum+yyleng);
		errStrEscAndUnterm(&pos);
                colNum += yyleng;
        }

["]({STRELT}*{BADESC}{STRELT}*)+["] {
                // Bad string lit
		Position pos(lineNum,colNum,lineNum,colNum+yyleng);
		errStrEsc(&pos);
                colNum += yyleng;
        }

\n|(\r\n)     { lineNum++; colNum = 1; }


[ \t]+	      { colNum += yyleng; }

#[^\n]*	  	{ /* Comment. No token, but update the 
                   char num in the very specific case of 
                   getting the correct EOF position */ 
		   colNum += yyleng;
		  }

.		          { 
				
				Position pos(lineNum,colNum,lineNum,colNum+yyleng);
				errIllegal(&pos, yytext);
			    #if EXIT_ON_ERR
			    exit(1);
			    #endif
		            this->colNum += yyleng; }
%%
