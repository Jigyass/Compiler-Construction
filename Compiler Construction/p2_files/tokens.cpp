#include "tokens.hpp" // Get the class declarations
#include "grammar.hh" // Get the TokenKind definitions

namespace cminusminus{

using TokenKind = cminusminus::Parser::token;
using Lexeme = cminusminus::Parser::semantic_type;

static std::string tokenKindString(int tokKind){
	switch(tokKind){
		case TokenKind::AMP: return "AMP";
		case TokenKind::AND: return "AND";
		case TokenKind::ASSIGN: return "ASSIGN";
		case TokenKind::AT: return "AT";
		case TokenKind::BOOL: return "BOOL";
		case TokenKind::COMMA: return "COMMA";
		case TokenKind::DEC: return "DEC";
		case TokenKind::DIVIDE: return "DIVIDE";
		case TokenKind::ELSE: return "ELSE";
		case TokenKind::END: return "EOF";
		case TokenKind::EQUALS: return "EQUALS";
		case TokenKind::FALSE: return "FALSE";
		case TokenKind::GREATER: return "GREATER";
		case TokenKind::GREATEREQ: return "GREATEREQ";
		case TokenKind::ID: return "ID";
		case TokenKind::IF: return "IF";
		case TokenKind::INC: return "INC";
		case TokenKind::INT: return "INT";
		case TokenKind::INTLITERAL: return "INTLITERAL";
		case TokenKind::LCURLY: return "LCURLY";
		case TokenKind::LESS: return "LESS";
		case TokenKind::LESSEQ: return "LESSEQ";
		case TokenKind::LPAREN: return "LPAREN";
		case TokenKind::MINUS: return "MINUS";
		case TokenKind::NOT: return "NOT";
		case TokenKind::NOTEQUALS: return "NOTEQUALS";
		case TokenKind::OR: return "OR";
		case TokenKind::PLUS: return "PLUS";
		case TokenKind::PTR: return "PTR";
		case TokenKind::READ: return "READ";
		case TokenKind::RETURN: return "RETURN";
		case TokenKind::RCURLY: return "RCURLY";
		case TokenKind::RPAREN: return "RPAREN";
		case TokenKind::SEMICOL: return "SEMICOL";
		case TokenKind::SHORT: return "SHORT";
		case TokenKind::SHORTLITERAL: return "SHORTLITERAL";
		case TokenKind::STRING: return "STRING";
		case TokenKind::STRLITERAL: return "STRINGLITERAL";
		case TokenKind::TIMES: return "TIMES";
		case TokenKind::TRUE: return "TRUE";
		case TokenKind::VOID: return "VOID";
		case TokenKind::WHILE: return "WHILE";
		case TokenKind::WRITE: return "WRITE";
		default:	
			return "OTHER";
	}
	
}

Token::Token(Position * posIn, int kindIn)
  : myPos(posIn), myKind(kindIn){
}

std::string Token::toString(){
	return tokenKindString(kind())
	+ " " + myPos->begin();
}

int Token::kind() const { 
	return this->myKind; 
}

Position * Token::pos() const {
	return myPos;
}

IDToken::IDToken(Position * posIn, std::string vIn)
  : Token(posIn, TokenKind::ID), myValue(vIn){ 
}

std::string IDToken::toString(){
	return tokenKindString(kind()) + ":"
	+ myValue + " " + myPos->begin();
}

const std::string IDToken::value() const { 
	return this->myValue; 
}

StrToken::StrToken(Position * posIn, std::string sIn)
  : Token(posIn, TokenKind::STRLITERAL), myStr(sIn){
}

std::string StrToken::toString(){
	return tokenKindString(kind()) + ":"
	+ this->myStr + " " + myPos->begin();
}

const std::string StrToken::str() const {
	return this->myStr;
}

IntLitToken::IntLitToken(Position * pos, int numIn)
  : Token(pos, TokenKind::INTLITERAL), myNum(numIn){}


std::string IntLitToken::toString(){
	return tokenKindString(kind()) + ":"
	+ std::to_string(this->myNum) + " "
	+ myPos->begin();
}

int IntLitToken::num() const {
	return this->myNum;
}

ShortLitToken::ShortLitToken(Position * pos, int numIn)
  : Token(pos, TokenKind::SHORTLITERAL), myNum(numIn){}

std::string ShortLitToken::toString(){
	return tokenKindString(kind()) + ":"
	+ std::to_string(this->myNum) + " "
	+ myPos->begin();
}

int ShortLitToken::num() const {
	return this->myNum;
}

} //End namespace cminusminus
