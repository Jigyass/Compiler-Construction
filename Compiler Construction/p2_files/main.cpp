#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include "errors.hpp"
#include "scanner.hpp"

using namespace cminusminus;

static void usageAndDie(){
	std::cerr << "Usage: cmmc <infile>"
	<< " [-p]: Parse the input to check syntax\n"
	<< " [-t <tokensFile>]: Output tokens to <tokensFile>\n"
	;
	exit(1);
}

static void writeTokenStream(const char * inPath, const char * outPath){
	std::ifstream inStream(inPath);
	if (!inStream.good()){
		std::string msg = "Bad input stream";
		msg += inPath;
		throw new InternalError(msg.c_str());
	}
	if (outPath == nullptr){
		std::string msg = "No tokens output file given";
		throw new InternalError(msg.c_str());
	}

	Scanner scanner(&inStream);
	if (strcmp(outPath, "--") == 0){
		scanner.outputTokens(std::cout);
	} else {
		std::ofstream outStream(outPath);
		if (!outStream.good()){
			std::string msg = "Bad output file ";
			msg += outPath;
			throw new InternalError(msg.c_str());
		}
		scanner.outputTokens(outStream);
		outStream.close();
	}
}

static bool parse(const char * inFile){
	std::ifstream inStream(inFile);
	if (!inStream.good()){
		std::string msg = "Bad input stream ";
		msg += inFile;
		throw new UserError(msg.c_str());
	}

	cminusminus::Scanner scanner(&inStream);
	cminusminus::Parser parser(scanner);
	int errCode = parser.parse();
	if (errCode != 0){ return false; }

	return true;
}

int 
main( const int argc, const char **argv )
{
	if (argc == 0){
		usageAndDie();
	}
	const char * inFile = NULL;
	const char * tokensFile = NULL;
	bool checkParse = false;
	bool useful = false;
	int i = 1;
	for (int i = 1 ; i < argc ; i++){
		if (argv[i][0] == '-'){
			if (argv[i][1] == 't'){
				i++;
				tokensFile = argv[i];
				useful = true;
			} else if (argv[i][1] == 'p'){
				i++;
				checkParse = true;
				useful = true;
			} else {
				std::cerr << "Unrecognized argument: ";
				std::cerr << argv[i] << std::endl;
				usageAndDie();
			}
		} else {
			if (inFile == NULL){
				inFile = argv[i];
			} else {
				std::cerr << "Only 1 input file allowed";
				std::cerr << argv[i] << std::endl;
				usageAndDie();
			}
		}
	}
	if (inFile == NULL){
		usageAndDie();
	}
	if (!useful){
		std::cerr << "Hey, you didn't tell cmmc to do anything!\n";
		usageAndDie();
	}

	try {
		if (tokensFile != NULL){
			writeTokenStream(inFile, tokensFile);
		} if (checkParse){
			bool parsed = parse(inFile);
			if (!parsed){
				std::cerr << "Parse failed" << std::endl;
			}
		}
	} catch (ToDoError * e){
		std::cerr << "ToDo: " << e->msg() << std::endl;
		exit(1);
	} catch (InternalError * e){
		std::string msg = "Something in the compiler is broken: ";
		std::cerr << msg << e->msg() << std::endl;
		exit(1);
	} catch (UserError * e){
		std::string msg = "The user made a mistake: ";
		std::cerr << msg << e->msg() << std::endl;
		exit(1);
	}
	
	return 0;
}
