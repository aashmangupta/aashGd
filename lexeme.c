//Lexical Analyzer
//Author: Aashman Gupta

#include "lexeme.h"
#include "types.h"
#include <string.h>

struct lexeme {
	char* type;
	char *strVal; 
	char charVal;
	int intVal; 
	double realVal;
	FILE* fval;
	LEXEME** aval;
	LEXEME* left;
	LEXEME* right;
};

LEXEME* cons(char* type, LEXEME* left, LEXEME* right) {
	LEXEME* lexemeObj = malloc(sizeof(LEXEME));
	lexemeObj->type = type;
	lexemeObj->left = left;
	lexemeObj->right = right;
	return lexemeObj;
}

LEXEME* car(LEXEME* lexObj) {	//getLexemeLeft
	return lexObj->left;
}

LEXEME* cdr(LEXEME* lexObj) {	//getLexemeRight
	return lexObj->right;
}

void setCar(LEXEME* lexObj, LEXEME* left) {	//setLexemeLeft
	lexObj->left = left;
}

void setCdr(LEXEME* lexObj, LEXEME* right) {	//setLexemeRight
	lexObj->right = right;
}

char* getType(LEXEME* token) {
	return token->type;
}

void setArrVal(LEXEME** lexObj, int pos, LEXEME* val) {
	lexObj[pos] = val;
}

void displayToken(LEXEME* token) {
	if(token->type == INTEGER) {
		printf("INTEGER ");
		printf("%d\n", token->intVal);
	}
	
	else if(token->type == REAL) {
		printf("REAL ");
		printf("%lf\n", token->realVal);
	}

	else if(token->type == STRING) {
		printf("STRING ");
		printf("\"%s\"\n", token->strVal);
	}

	else if(token->type == VARIABLE) {
		printf("VARIABLE ");
		printf("%s\n", token->strVal);
	}

	else {
		printf("%s\n", token->type);
	}
}

LEXEME** extractArr(LEXEME* lexObj) {
	return lexObj->aval;
}

int getIntVal(LEXEME* lexObj) {
	return lexObj->intVal;
}

char* getStrVal(LEXEME* lexObj) {
	return lexObj->strVal;
}

char getCharVal(LEXEME* lexObj) {
	return lexObj->charVal;
}

double getDoubleVal(LEXEME* lexObj) {
	return lexObj->realVal;
}

LEXEME* getArrayVal(LEXEME** lexObj, int pos) {
	return lexObj[pos];
}

void setFileVal(LEXEME* lexObj, FILE* fp) {
	lexObj->fval = fp;
}

FILE* getFileVal(LEXEME* lexObj) {
	return lexObj->fval;
}

LEXEME* newFileLexeme(char* type, char* fileName) {
	LEXEME* lexObj = malloc(sizeof(LEXEME));
	lexObj->type = fileName;
	lexObj->type = type;
	fileName = 0;
	lexObj->fval = NULL;
	return lexObj;
}

LEXEME* newLexemeArr(char* type, int size) {
	LEXEME* lexemeObj = malloc(sizeof(LEXEME));
	lexemeObj->type = type;
	lexemeObj->aval = malloc(sizeof(LEXEME*) * (size + 1));
	return lexemeObj;
}

LEXEME* newLexeme(char* type) {
	LEXEME* lexemeObj = malloc(sizeof(LEXEME));
	lexemeObj->type = type;
	return lexemeObj;
}

LEXEME* newLexemeStr(char* type, char* strVal) {
	LEXEME* lexemeObj = malloc(sizeof(LEXEME));
	lexemeObj->type = type;
	lexemeObj->strVal = malloc(sizeof(char*) * (strlen(strVal) + 1));
	strcpy(lexemeObj->strVal, strVal);
	//lexemeObj->strVal[strlen(lexemeObj->strVal)] = '\0';
	return lexemeObj;
}


LEXEME* newLexemeChar(char* type, char charVal) {
	LEXEME* lexemeObj = malloc(sizeof(LEXEME));
	lexemeObj->type = type;
	lexemeObj->charVal = charVal;
	return lexemeObj;
}


LEXEME* newLexemeInt(char* type, int intVal) {
	LEXEME* lexemeObj = malloc(sizeof(LEXEME));
	lexemeObj->type = type;
	lexemeObj->intVal = intVal;
	return lexemeObj;
}


LEXEME* newLexemeReal(char* type, double realVal) {
	LEXEME* lexemeObj = malloc(sizeof(LEXEME));
	lexemeObj->type = type;
	lexemeObj->realVal = realVal;
	return lexemeObj;
}

int sameVariable(LEXEME* var, LEXEME* var2) { //1 for same, 0 for not
	if(var->type != var2->type) return 0;
	if(strcmp(var->strVal, var2->strVal) != 0) return 0;
	return 1;
}



