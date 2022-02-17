#include <fstream>
#include <string.h>

#include "errors.hpp"
#include "lexer.hpp"

using namespace std;
using namespace cminusminus;

static void usageAndDie(){
	std::cerr << "Usage: teencc <infile> <options>\n"
	<< " [-t <tokensFile>]\n"
	<< "\n"
	;
	std::cout << std::flush;
	std::cerr << std::flush;
	exit(1);
}

static std::ifstream * openInput(const char * inputPath){
	if (inputPath == nullptr){ usageAndDie(); }

	std::ifstream * input = new std::ifstream(inputPath);
	if (input == NULL){ usageAndDie(); }
	if (!input->good()){
		std::cerr << "Bad path " <<  inputPath << std::endl;
		usageAndDie();
	}
	return input;
}

static void doTokenization(const char * inputPath, const char * outPath){
	std::ifstream * input = openInput(inputPath);

	cminusminus::Scanner scanner(input);
	if (strcmp(outPath, "--") == 0){
		scanner.outputTokens(std::cout);
	} else {
		std::ofstream outStream(outPath);
		if (!outStream.good()){
			std::string msg = "Bad output file ";
			msg += outPath;
			throw new cminusminus::InternalError(msg.c_str());
		}
		scanner.outputTokens(outStream);
	}
}

int main(int argc, char * argv[]){
	if (argc <= 1){ usageAndDie(); }
	const char * input = argv[1];

	const char * tokensFile = nullptr; // Output file if 
	                                   // printing tokens
	bool useful = false; // Check whether the command is 
                         // a no-op

	//Loop through command line looking for options.
	// More options will be added as the compiler is extended
	for (int i = 1; i < argc; i++){
		if (argv[i][0] == '-'){
			if (argv[i][1] == 't'){
				i++;
				if (i >= argc){ usageAndDie(); }
				tokensFile = argv[i];
				useful = true;
			} else {
				std::cerr << "Unknown option"
				  << " " << argv[i] << "\n";
				usageAndDie();
			}
		}
	}

	if (useful == false){
		std::cerr << "You didn't specify an operation to do!\n";
		usageAndDie();
	}


	if (tokensFile != nullptr){
		doTokenization(input, tokensFile);
	}

	return 0;
}
