#ifndef __CMINUSMINUS_SCANNER_HPP__
#define __CMINUSMINUS_SCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "grammar.hh"
#include "errors.hpp"

using TokenKind = cminusminus::Parser::token;

namespace cminusminus{

class Scanner : public yyFlexLexer{
public:
   
   Scanner(std::istream *in) : yyFlexLexer(in)
   {
	lineNum = 1;
	colNum = 1;
   };
   virtual ~Scanner() {
   };

   //get rid of override virtual function warning
   using FlexLexer::yylex;

   // YY_DECL defined in the flex cminusminus.l
   virtual int yylex( cminusminus::Parser::semantic_type * const lval);

   int makeBareToken(int tagIn){
	size_t len = static_cast<size_t>(yyleng);
	Position * pos = new Position(
	  this->lineNum, this->colNum,
	  this->lineNum, this->colNum+len);
        this->yylval->lexeme = new Token(pos, tagIn);
        colNum += len;
        return tagIn;
   }

   void errIllegal(Position * pos, std::string match){
	cminusminus::Report::fatal(pos, "Illegal character "
		+ match);
   }

   void errStrEsc(Position * pos){
	cminusminus::Report::fatal(pos, "String literal with bad"
	" escape sequence ignored");
   }

   void errStrUnterm(Position * pos){
	cminusminus::Report::fatal(pos, "Unterminated string"
	" literal ignored");
   }

   void errStrEscAndUnterm(Position * pos){
	cminusminus::Report::fatal(pos, "Unterminated string literal"
	" with bad escape sequence ignored");
   }

   void errIntOverflow(Position * pos){
	cminusminus::Report::fatal(pos, "Integer literal overflow");
   }

   void errIntUnderflow(Position * pos){
	cminusminus::Report::fatal(pos, "Integer literal underflow");
   }

   void errShortOverflow(Position * pos){
	cminusminus::Report::fatal(pos, "Short literal overflow");
   }

   void errShortUnderflow(Position * pos){
	cminusminus::Report::fatal(pos, "Short literal underflow");
   }

/*
   void warn(int lineNumIn, int colNumIn, std::string msg){
	std::cerr << lineNumIn << ":" << colNumIn 
		<< " ***WARNING*** " << msg << std::endl;
   }

   void error(int lineNumIn, int colNumIn, std::string msg){
	std::cerr << lineNumIn << ":" << colNumIn 
		<< " ***ERROR*** " << msg << std::endl;
   }
*/

   static std::string tokenKindString(int tokenKind);

   void outputTokens(std::ostream& outstream);

private:
   cminusminus::Parser::semantic_type *yylval = nullptr;
   size_t lineNum;
   size_t colNum;
};

} /* end namespace */

#endif /* END __CMINUSMINUS_SCANNER_HPP__ */
