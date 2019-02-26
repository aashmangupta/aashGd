#include <stdio.h>
#include "types.h"
#include "environment.h"
#include "lexeme.h"
#include "lexer.h"
#include "recognizer.h"
#include "evaluator.h"

int main(int argc, char* argv[]) {
	countCL = argc;
	argsCL = argv;

	if(argc == 1) {
		printf("ERROR: no source file was given...\n");
		return 0;
	}
	
	FILE* fp = fopen(argv[1], "r");

	if(fp == 0) {
		printf("Empty file...\n");
		return 0;
	}
	
	recieveFile(fp);		//passes the file to lex
	advance();
	
	LEXEME* tree = program();
	LEXEME* env = newEnvironment();

	eval(tree, env);

return 0;
}
