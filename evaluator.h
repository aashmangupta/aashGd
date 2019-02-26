#include <stdio.h>
#include "environment.h"
#include "lexeme.h"
#include "lexer.h"
#include "types.h"

extern int countCL;
extern char** argsCL;
extern LEXEME* eval(LEXEME* tree, LEXEME* env);
extern LEXEME* evalExit();
extern LEXEME* evalParamList(LEXEME* tree, LEXEME* env);
extern LEXEME* evalWhile(LEXEME* tree, LEXEME* env);
extern LEXEME* evalIf(LEXEME* tree, LEXEME* env);
extern LEXEME* evalElse(LEXEME* tree, LEXEME* env);
extern LEXEME* evalReturn(LEXEME* tree, LEXEME* env);
extern LEXEME* evalVardef(LEXEME* tree, LEXEME* env);
extern LEXEME* evalFuncDef(LEXEME* func, LEXEME* env);
extern LEXEME* evalFuncCall(LEXEME* func, LEXEME* env);
extern LEXEME* evalBlock(LEXEME* tree, LEXEME* env);
extern LEXEME* evalStatementList(LEXEME* tree, LEXEME* env);
extern LEXEME* evalStatement(LEXEME* tree, LEXEME* env);
extern LEXEME* evalLogAnd(LEXEME* tree, LEXEME* env);
extern LEXEME* evalLogOr(LEXEME* tree, LEXEME* env);
extern LEXEME* evalUminus(LEXEME* tree, LEXEME* env);
extern LEXEME* evalLessThan(LEXEME* tree, LEXEME* env);
extern LEXEME* evalGreaterThan(LEXEME* tree, LEXEME* env);
extern LEXEME* evalEqualTo(LEXEME* tree, LEXEME* env);
extern LEXEME* evalNot(LEXEME* tree, LEXEME* env);
extern LEXEME* evalAssign(LEXEME* tree, LEXEME* env);
extern LEXEME* evalPlus(LEXEME* tree, LEXEME* env);
extern LEXEME* evalMinus(LEXEME* tree, LEXEME* env);
extern LEXEME* evalTimes(LEXEME* tree, LEXEME* env);
extern LEXEME* evalDivide(LEXEME* tree, LEXEME* env);
extern LEXEME* evalArgList(LEXEME* tree, LEXEME* env);
extern LEXEME* evalPrint(LEXEME* tree, LEXEME* env);
extern LEXEME* evalProgram(LEXEME* tree, LEXEME* env);
extern LEXEME* getClosureBody(LEXEME* closure);
extern LEXEME* getClosureParams(LEXEME* closure);
extern LEXEME* evalNewArray(LEXEME* tree, LEXEME* env);
extern LEXEME* evalGetArray(LEXEME* tree, LEXEME* env);
extern LEXEME* evalSetArray(LEXEME* tree, LEXEME* env);
extern LEXEME* evalDot(LEXEME* tree, LEXEME* env);
extern LEXEME* evalOpenFile(LEXEME* tree, LEXEME* env);
extern LEXEME* evalReadInteger(LEXEME* tree, LEXEME* env);
extern LEXEME* evalAtFileEnd(LEXEME* tree, LEXEME* env);
extern LEXEME* evalCloseFile(LEXEME* tree, LEXEME* env);
extern LEXEME* evalGetArgCount();
extern LEXEME* evalGetArgAt(LEXEME* tree, LEXEME* env);
extern LEXEME* evalLambda(LEXEME* tree, LEXEME* env);

