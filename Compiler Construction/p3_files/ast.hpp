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


/** \class DeclNode
* Superclass for declarations (i.e. nodes that can be used to 
* declare a struct, function, variable, etc).  This base class will 
**/
class DeclNode : public StmtNode{
public:
	DeclNode(Position * p) : StmtNode(p) { }
	void unparse(std::ostream& out, int indent) override = 0;
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

class IntTypeNode : public TypeNode{
public:
	IntTypeNode(Position * p) : TypeNode(p){ }
	void unparse(std::ostream& out, int indent);
};

} //End namespace cminusminus

#endif
