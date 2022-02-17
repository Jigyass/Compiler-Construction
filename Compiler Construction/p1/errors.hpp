#ifndef CMM_ERRORS_H
#define CMM_ERRORS_H

#include <iostream>
#include "position.hpp"

namespace cminusminus{

class Report{
public:
	static void fatal(
		Position * pos,
		const char * msg
	){
		std::cerr << "FATAL " 
		<< pos->span()
		<< ": " 
		<< msg  << std::endl;
	}

	static void fatal(
		Position * pos,
		const std::string msg
	){
		fatal(pos,msg.c_str());
	}

};

class InternalError{
public:
	InternalError(const char * msgIn) : myMsg(msgIn){}
	std::string msg(){ return myMsg; }
private:
	std::string myMsg;
};

class ToDoError{
public:
	ToDoError(const char * msgIn) : myMsg(msgIn){}
	const char * msg(){ return myMsg; }
private:
	const char * myMsg;
};

}

#endif
