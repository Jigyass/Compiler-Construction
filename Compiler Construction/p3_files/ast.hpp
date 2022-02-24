#ifndef CMINUSMINUS_AST_HPP
#define CMINUSMINUS_AST_HPP

#include <ostream>
#include <list>
#include "tokens.hpp"
#include <cassert>


/* You absolutely will need to add a bunch of ASTNode subclasses */

namespace cminusminus{

/* You may find it useful to forward declare AST subclasses
   here so that you can use a class before it's full definition
*/
class DeclNode;
class TypeNode;
class StmtNode;
class IDNode;

class FnDeclNode;
class AssignStmtNode;
class ReadStmtNode;
class WriteStmtNode;
class PostDecStmtNode;
class PostIncStmtNode;
class IfStmtNode;
class IfElseStmtNode;
class WhileStmtNode;
class ReturnStmtNode;
class CallStmtNode;
class CallExpNode;
class IDNode;
class BinaryExpNode;
class IndexNode;
class UnaryExpNode;
class DerefNode;
class NotNode;
class NegNode;
class IntTypeNode;
class PtrTypeNode;
class AssignExpNode;
class LiteralNode;
class FormalDeclNode;
class LValNode;
class ExpNode;

/** 
* \class ASTNode
* Base class for all other AST Node types
**/
class ASTNode{
public:
	ASTNode(Position * p) : myPos(p){ }
	virtual void unparse(std::ostream& out, int indent) = 0;
	Position * pos() { return myPos; }
	std::string posStr() { return pos()->span(); }
protected:
	Position * myPos;
};

/** 
* \class ProgramNode
* Class that contains the entire abstract syntax tree for a program.
* Note the list of declarations encompasses all global declarations
* which includes (obviously) all global variables and struct declarations
* and (perhaps less obviously), all function declarations
**/
class ProgramNode : public ASTNode{
public:
	ProgramNode(std::list<DeclNode *> * globalsIn) ;
	void unparse(std::ostream& out, int indent) override;
private:
	std::list<DeclNode * > * myGlobals;
};

class StmtNode : public ASTNode{
public:
	StmtNode(Position * p) : ASTNode(p){ }
	void unparse(std::ostream& out, int indent) override = 0;
};



class AssignStmtNode : public StmtNode{
public:
	AssignStmtNode(Position * p, AssignExpNode * assignExp) 
	: StmtNode(p),  myAssignExp(assignExp){ 
		assert (myAssignExp != nullptr);
	}
	void unparse(std::ostream& out, int indent) override = 0;
private:
	AssignExpNode * myAssignExp;
};

class CallStmtNode : public StmtNode{
public:
	CallStmtNode(Position * p, CallExpNode * callExp) 
	: StmtNode(p),  myCallExp(callExp){ 
		assert (myCallExp != nullptr);
	}
	void unparse(std::ostream& out, int indent) override = 0;
private:
	CallExpNode * myCallExp;
};

/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to 
* declare a struct, function, variable, etc).  This base class will 
**/
class DeclNode : public StmtNode{
public:
	DeclNode(Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
};

class ReadStmtNode : public StmtNode{
public:
	ReadStmtNode(Position * p, LValNode * lVal) : StmtNode(p), myLVal(lVal){ }
	void unparse(std::ostream& out, int indent) override = 0;
private:
	LValNode * myLVal;
};

class WriteStmtNode : public StmtNode{
public:
	WriteStmtNode(Position * p, ExpNode * exp) : StmtNode(p), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override = 0;
private:
	ExpNode * myExp;
};

class PostDecStmtNode : public StmtNode{
public:
	PostDecStmtNode(Position * p, LValNode * lVal) : StmtNode(p), myLVal(lVal){ }
	void unparse(std::ostream& out, int indent) override = 0;
private:
	LValNode * myLVal;
};

class PostIncStmtNode : public StmtNode{
public:
	PostIncStmtNode(Position * p, ExpNode * exp) : StmtNode(p), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override = 0;
private:
	ExpNode * myExp;
};

class IfStmtNode : public StmtNode{
public:
	IfStmtNode(Position * p, ExpNode * exp, std::list<StmtNode *> * stmtsIn) 
	: StmtNode(p), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override = 0;
private:
	ExpNode * myExp;
	std::list<StmtNode *> * myStmtsIn;
};

class IfElseStmtNode : public StmtNode{
public:
	IfElseStmtNode(Position * p, ExpNode * exp, std::list<StmtNode *> * trueStmtsIn, std::list<StmtNode *> * falseStmtsIn) 
	: StmtNode(p), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override = 0;
private:
	ExpNode * myExp;
	std::list<StmtNode *> * myTrueStmtsIn;
	std::list<StmtNode *> * myFalseStmtsIn;
};

class WhileStmtNode : public StmtNode{
public:
	WhileStmtNode(Position * p, ExpNode * exp, std::list<StmtNode *> * stmtsIn) 
	: StmtNode(p), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override = 0;
private:
	ExpNode * myExp;
	std::list<StmtNode *> * myStmtsIn;
};

class ReturnStmtNode : public StmtNode{
public:
	ReturnStmtNode(Position * p, ExpNode * exp) : StmtNode(p), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override = 0;
private:
	ExpNode * myExp;
};



/**  \class ExpNode
* Superclass for expression nodes (i.e. nodes that can be used as
* part of an expression).  Nodes that are part of an expression
* should inherit from this abstract superclass.
**/
class ExpNode : public ASTNode{
protected:
	ExpNode(Position * p) : ASTNode(p){ }
};

class AssignExpNode : public ExpNode{
public:
	AssignExpNode(Position * p, LValNode * lVal, ExpNode * exp) 
	: ExpNode(p), myLVal(lVal), myExp(exp){ }
	void unparse(std::ostream& out, int indent) override = 0;
private:
	LValNode * myLVal;
	ExpNode * myExp;
};

/**  \class TypeNode
* Superclass of nodes that indicate a data type. For example, in 
* the declaration "int a", the int part is the type node (a is an IDNode
* and the whole thing is a DeclNode).
**/
class TypeNode : public ASTNode{
protected:
	TypeNode(Position * p) : ASTNode(p){
	}
public:
	virtual void unparse(std::ostream& out, int indent) = 0;
};

class LValNode : public ExpNode{
public:
	LValNode(Position * p) : ExpNode(p){}
	void unparse(std::ostream& out, int indent) override = 0;
};

class UnaryExpNode : public ExpNode{
public:
	UnaryExpNode(Position * p, ExpNode * exp) : ExpNode(p), myExp(exp){}
	void unparse(std::ostream& out, int indent) override = 0;
private:
	ExpNode * myExp;
};

class NegNode : public UnaryExpNode{
public:
	NegNode(Position * p, ExpNode * exp) : UnaryExpNode(p, exp){ }
	void unparse(std::ostream& out, int indent);
};

class NotNode : public UnaryExpNode{
public:
	NotNode(Position * p, ExpNode * exp) : UnaryExpNode(p, exp){ }
	void unparse(std::ostream& out, int indent);
};

class RefNode : public UnaryExpNode{
public:
	RefNode(Position * p, ExpNode * exp) : UnaryExpNode(p, exp){ }
	void unparse(std::ostream& out, int indent);
};

class BinaryExpNode : public ExpNode{
public:
	BinaryExpNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: ExpNode(p), myLHS(lhs), myRHS(rhs){}
	void unparse(std::ostream& out, int indent) override = 0;
private:
	ExpNode * myLHS;
	ExpNode * myRHS;
};

class AndNode : public BinaryExpNode{
public:
	AndNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class DivideNode : public BinaryExpNode{
public:
	DivideNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class EqualsNode : public BinaryExpNode{
public:
	EqualsNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class GreaterEqNode : public BinaryExpNode{
public:
	GreaterEqNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class GreaterNode : public BinaryExpNode{
public:
	GreaterNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class LessEqNode : public BinaryExpNode{
public:
	LessEqNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class LessNode : public BinaryExpNode{
public:
	LessNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class MinusNode : public BinaryExpNode{
public:
	MinusNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class NotEqualsNode : public BinaryExpNode{
public:
	NotEqualsNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class OrNode : public BinaryExpNode{
public:
	OrNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class PlusNode : public BinaryExpNode{
public:
	PlusNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

class TimesNode : public BinaryExpNode{
public:
	TimesNode(Position * p, ExpNode * lhs, ExpNode * rhs) 
	: BinaryExpNode(p, lhs, rhs){ }
	void unparse(std::ostream& out, int indent);
};

/** An identifier. Note that IDNodes subclass
 * ExpNode because they can be used as part of an expression. 
**/
class IDNode : public LValNode{
public:
	IDNode(Position * p, std::string nameIn) 
	: LValNode(p), name(nameIn){ }
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	std::string name;
};

class DerefNode : public LValNode{
public:
	DerefNode(Position * p, IDNode * id) 
	: LValNode(p), myID(id){ }
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	IDNode * myID;
};

class IndexNode : public LValNode{
public:
	IndexNode(Position * p, IDNode * baseID, IDNode * fieldName) 
	: LValNode(p), myBaseID(baseID), myFieldName(fieldName){ }
	void unparse(std::ostream& out, int indent);
private:
	/** The name of the identifier **/
	IDNode * myBaseID;
	IDNode * myFieldName;
};
 
/** A variable declaration.
**/
class VarDeclNode : public DeclNode{
public:
	VarDeclNode(Position * p, TypeNode * type, IDNode * id) 
	: DeclNode(p), myType(type), myId(id){
		assert (myType != nullptr);
		assert (myId != nullptr);
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IDNode * myId;
};

class FnDeclNode : public DeclNode{
public:
	FnDeclNode(Position * p, TypeNode * type, IDNode * id, std::list<FormalDeclNode *> * formalDeclsIn, std::list<StmtNode *> * stmtsIn) 
	: DeclNode(p), myType(type), myId(id){
		assert (myType != nullptr);
		assert (myId != nullptr);
		assert (myFormalDeclsIn != nullptr);
		assert (myStmtsIn != nullptr);
	}
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
	IDNode * myId;
	FormalDeclNode * myFormalDeclsIn;
	StmtNode * myStmtsIn;
};

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class BoolTypeNode : public TypeNode{
public:
	BoolTypeNode(Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class PtrTypeNode : public TypeNode{
public:
	PtrTypeNode(Position * p, TypeNode * type) : TypeNode(p), myType(type){ }
	void unparse(std::ostream& out, int indent);
private:
	TypeNode * myType;
};

class ShortTypeNode : public TypeNode{
public:
	ShortTypeNode(Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class StringTypeNode : public TypeNode{
public:
	StringTypeNode(Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class VoidTypeNode : public TypeNode{
public:
	VoidTypeNode(Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

class BoolLitNode : public BoolTypeNode{
public:
	BoolLitNode(Position * p, bool val) : BoolTypeNode(p), myVal(val){ }
	void unparse(std::ostream& out, int indent);
private:
	bool myVal;
};

class ShortLitNode : public ShortTypeNode{
public:
	ShortLitNode(Position * p, short val) : ShortTypeNode(p), myVal(val){ }
	void unparse(std::ostream& out, int indent);
private:
	short myVal;
};

class StringLitNode : public StringTypeNode{
public:
	StringLitNode(Position * p, std::string val) : StringTypeNode(p), myVal(val){ }
	void unparse(std::ostream& out, int indent);
private:
	std::string myVal;
};

} //End namespace cminusminus

#endif
