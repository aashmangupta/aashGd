//Lexical Analyzer
//Author: Aashman Gupta

#ifndef __LEXER_INCLUDED__
#define __LEXER_INCLUDED__

#include <stdio.h>
#include "lexeme.h"

typedef int bool;
#define true 1
#define false 0

extern void myPushback();
extern LEXEME* lex();
extern void myRead();
extern void skipWhiteSpace();
extern void recieveFile(FILE* passedFile);
extern LEXEME* lexVariableOrKeyword();
extern LEXEME* lexNumber();
extern LEXEME* lexString();
extern int getLineNum();
extern int invisWhitespace();
extern void handleComments();

#endif
