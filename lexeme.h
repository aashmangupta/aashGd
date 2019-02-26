//Lexical Analyzer
//Author: Aashman Gupta

#ifndef __LEXEME_INCLUDED__
#define __LEXEME_INCLUDED__

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

typedef struct lexeme LEXEME;

extern LEXEME* cons(char* type, LEXEME* left, LEXEME* right);
extern LEXEME* car(LEXEME* lexObj);	//getLexemeLeft
extern LEXEME* cdr(LEXEME* lexObj);	//getLexemeRight
extern void setCar(LEXEME* lexObj, LEXEME* left); //setLexemeLeft
extern void setCdr(LEXEME* lexObj, LEXEME* right); //setLexemeRight
extern void setArrVal(LEXEME** lexObj, int pos, LEXEME* val);
extern void setFileVal(LEXEME* lexObj, FILE* fp);

extern LEXEME* newLexeme(char* type);
extern LEXEME* newFileLexeme(char* type, char* fileName);
extern LEXEME* newLexemeStr(char* type, char* strVal);
extern LEXEME* newLexemeChar(char* type, char charVal);
extern LEXEME* newLexemeInt(char* type, int intVal);
extern LEXEME* newLexemeReal(char* type, double realVal);
extern LEXEME* newLexemeArr(char* type, int size);

extern char* getType(LEXEME* token);
extern int sameVariable(LEXEME* var, LEXEME* var2); //1 for same, 0 for not
extern void displayToken(LEXEME* token);

extern LEXEME* getArrayVal(LEXEME** lexObj, int pos);
extern int getIntVal(LEXEME* lexObj);
extern char getCharVal(LEXEME* lexObj);
extern char* getStrVal(LEXEME* lexObj);
extern double getDoubleVal(LEXEME* lexObj);
extern LEXEME** extractArr(LEXEME* lexObj);
extern FILE* getFileVal(LEXEME* lexObj);

#endif
