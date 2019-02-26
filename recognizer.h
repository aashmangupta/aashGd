#include <stdio.h>
#include "types.h"
#include "lexeme.h"
#include "lexer.h"

extern LEXEME* program();
extern int programPending();
extern LEXEME* definition();
extern int definitionPending();
extern LEXEME* expression();
extern int expressionPending();
extern LEXEME* operators();
extern int operatorPending();
extern LEXEME* unary();
extern int unaryPending();
extern LEXEME* funcCall();
extern int funcCallPending();
extern LEXEME* argList();
extern int argListPending();
extern LEXEME* optArgList();
extern int optArgListPending();
extern LEXEME* ifstatement();
extern int ifstatementPending();
extern LEXEME* elsestatement();
extern int elsestatementPending();
extern LEXEME* optElse();
extern int optElsePending();
extern LEXEME* whileLoop();
extern int whileLoopPending();
extern LEXEME* block();
extern int blockPending();
extern LEXEME* statements();
extern LEXEME* statementList();
extern int statementListPending();
extern LEXEME* optStatementList();
extern int statementPending();
extern LEXEME* functionDef();
extern int functionDefPending();
extern LEXEME* parameterList();
extern int paramListPending();
extern LEXEME* optParamList();
extern int optParamListPending();
extern LEXEME* varDef();
extern int varDefPending();

extern int check(char* type);
extern LEXEME* advance();
extern LEXEME* match(char* type);
extern void matchNoAdvance(char* type);
