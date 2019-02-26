//TODO CANNOT do just var x;
//TODO have to assign it to a dummy val... for ex. var x = 0;

#include <stdio.h>
#include <string.h>
#include "evaluator.h"
#include "environment.h"
#include "lexer.h"
#include "lexeme.h"
#include "types.h"

int countCL;
char** argsCL;

LEXEME* eval(LEXEME* tree, LEXEME* env) {
	//literals
    if(getType(tree) == INTEGER) return tree;
    else if(getType(tree) ==  REAL) return tree;
    else if(getType(tree) == CHARACTER) return tree;
    else if(getType(tree) == STRING) return tree;
	
	//special
	if(getType(tree) == PROGRAM) return evalProgram(tree, env);
	else if(getType(tree) == VARIABLE) {
		if(car(tree) != NULL) {
			return evalFuncCall(car(tree), env);
		}
		else {
			return lookup(tree, env);
		}
	}
    else if(getType(tree) == OPAREN) return eval(car(tree), env);

	//print
    else if(getType(tree) == PRINT) return evalPrint(tree, env);

	//array
    else if(getType(tree) == NEWARRAY) return evalNewArray(tree, env);
    else if(getType(tree) == SETARRAY) return evalSetArray(tree, env);
    else if(getType(tree) == GETARRAY) return evalGetArray(tree, env);

	//command line args
	else if(getType(tree) == GETARGCOUNT) return evalGetArgCount();
	else if(getType(tree) == GETARGAT) return evalGetArgAt(tree, env);

	//readFile
    else if(getType(tree) == OPENFILE) return evalOpenFile(tree, env);
    else if(getType(tree) == READINTEGER) return evalReadInteger(tree, env);
    else if(getType(tree) == ATFILEEND) return evalAtFileEnd(tree, env);
    else if(getType(tree) == CLOSEFILE) return evalCloseFile(tree, env);

	//operators
 	else if(getType(tree) == DOT) return evalDot(tree, env);
	else if(getType(tree) == PLUS) return evalPlus(tree, env);				
    else if(getType(tree) == TIMES) return evalTimes(tree, env);
    else if(getType(tree) == DIVIDES) return evalDivide(tree, env);
    else if(getType(tree) == MINUS) return evalMinus(tree, env);
    else if(getType(tree) == ASSIGN) return evalAssign(tree, env);		
    else if(getType(tree) == EQUALTO) return evalEqualTo(tree, env);	
    else if(getType(tree) == NOT) return evalNot(tree, env);
    else if(getType(tree) == LESSTHAN) return evalLessThan(tree, env);	
    else if(getType(tree) == GREATERTHAN) return evalGreaterThan(tree, env);
    else if(getType(tree) == UMINUS) return evalUminus(tree, env);	
    
	//logical operators
	else if(getType(tree) == AND) return evalLogAnd(tree, env);
    else if(getType(tree) == OR) return evalLogOr(tree, env);
    
	
	//statements
    else if(getType(tree) == STATEMENTLIST) return evalStatementList(tree, env);
	else if(getType(tree) == RETURN) return evalReturn(tree, env);
    else if(getType(tree) == STATEMENT) return evalStatement(tree, env);
    else if(getType(tree) == FUNC_CALL) return evalFuncCall(tree, env);	
    else if(getType(tree) == ARGLIST) return evalArgList(tree, env);
    else if(getType(tree) == IF) return evalIf(tree, env);		
    else if(getType(tree) == ELSE) return evalElse(tree, env);
    else if(getType(tree) == WHILE) return evalWhile(tree, env);	
    
	//miscellaneous
	else if(getType(tree) == EXIT) return evalExit();
	else if(getType(tree) == LAMBDA) return evalLambda(tree, env);
	else if(getType(tree) == BLOCK) return evalBlock(tree, env);	
    else if(getType(tree) == FUNCTION_DEF) return evalFuncDef(tree, env);
    else if(getType(tree) == PARAMLIST) return evalParamList(tree, env);	//FIXME might not need this
    else if(getType(tree) == VARDEF) return evalVardef(tree, env);	
    else {
        printf("Fatal error: Bad expression!\nDetected in evaluator\n");
		exit(1);
    }
}

LEXEME* evalExit() {
	exit(1);
	return NULL;
}

LEXEME* evalLambda(LEXEME* tree, LEXEME* env) {
	return cons(CLOSURE, env, tree);
}

LEXEME* evalProgram(LEXEME* tree, LEXEME* env) {
	LEXEME* ptr = tree;
	while(ptr != NULL) {
		eval(car(ptr), env);
		ptr = cdr(ptr);
	}
	return NULL;
}

LEXEME* evalParamList(LEXEME* tree, LEXEME* env) {
	if(env != NULL) {
		return tree;
	}
	return tree;
}

LEXEME* evalWhile(LEXEME* tree, LEXEME* env) {
	LEXEME* result = NULL;
	while(getIntVal(eval(car(tree), env)) == 1) {
		result = eval(cdr(tree), env);
	}
	return result;
}

LEXEME* evalIf(LEXEME* tree, LEXEME* env) {
	if(getIntVal(eval(car(tree), env)) == 1) {
		return evalBlock(car(cdr(tree)), env);
	}
	else {
		return evalElse(cdr(cdr(tree)), env);
	}
	return NULL;
}

LEXEME* evalElse(LEXEME* tree, LEXEME* env) {
	if(tree == NULL) {
		return NULL;
	}
	if(getType(car(tree)) == IF) {
		return evalIf(car(tree), env);
	}
	else {
		return evalBlock(car(tree), env);
	}
}

LEXEME* evalReturn(LEXEME* tree, LEXEME* env) {
	return eval(car(tree), env);
}

LEXEME* evalVardef(LEXEME* tree, LEXEME* env) {
	if(cdr(tree) != NULL) {
		insert(car(tree), eval(cdr(tree), env), env);
	}
	else {
		insert(car(tree), NULL, env);
	}
	return cdr(tree);
}

LEXEME* evalFuncDef(LEXEME* func, LEXEME* env) {
	LEXEME* closure = cons(CLOSURE,env,func);
	insert(car(func), closure, env);
	return NULL;
}

LEXEME* evalFuncCall(LEXEME* func, LEXEME* env) {
	LEXEME* closure = eval(cdr(func), env);
	LEXEME* args = car(func);
	LEXEME* params = getClosureParams(closure);
	LEXEME* body = getClosureBody(closure);
	LEXEME* senv = car(closure);
	LEXEME* eargs = evalArgList(args, env);
	LEXEME* xenv = extend(params, eargs, senv);

	//objects
	insert(newLexemeStr(VARIABLE, "this"), xenv, xenv);

	LEXEME* result = eval(body, xenv);
	return result;
}

LEXEME* getClosureBody(LEXEME* closure) {
	return cdr(cdr(cdr(closure)));
}

LEXEME* getClosureParams(LEXEME* closure) {
	return car(cdr(cdr(closure)));
}

LEXEME* evalBlock(LEXEME* tree, LEXEME* env) {
	LEXEME* result;
	while(tree != NULL) {
		result = eval(car(tree), env);
		tree = cdr(tree);
	}
	return result;
}

LEXEME* evalStatementList(LEXEME* tree, LEXEME* env) {
	LEXEME* val = NULL;	

	while(tree != NULL) {
		val = eval(car(tree), env);
		tree = cdr(tree);
	}
	return val;
}

LEXEME* evalStatement(LEXEME* tree, LEXEME* env) {
	/*if((car(car(tree)) != NULL) && getType(car(car(tree))) == FUNC_CALL) {
		printf("204: CAll\n");
		LEXEME* result = car(car(tree));
		eval(car(tree), env);
		return result;
	}*/
	return eval(car(tree), env);
}

LEXEME* evalLogAnd(LEXEME* tree, LEXEME* env) {
	if(getIntVal(eval(car(tree), env)) == 1 && getIntVal(eval(cdr(tree), env)) == 1) {
		return newLexemeInt(INTEGER, 1);
	}
	else return newLexemeInt(INTEGER, 0);
}

LEXEME* evalLogOr(LEXEME* tree, LEXEME* env) {
	if(getIntVal(eval(car(tree), env)) == 1 || getIntVal(eval(cdr(tree), env)) == 1) {
		return newLexemeInt(INTEGER, 1);
	}
	else return newLexemeInt(INTEGER, 0);
}

LEXEME* evalUminus(LEXEME* tree, LEXEME* env) {
	LEXEME* unary = eval(car(tree), env);
	if(getType(unary) == INTEGER) {
		return newLexemeInt(INTEGER, getIntVal(unary) * -1);
	}
	else {
		return newLexemeReal(REAL, getDoubleVal(unary) * -1);
	}
}

LEXEME* evalDot(LEXEME* tree, LEXEME* env) {
	LEXEME* object = eval(car(tree), env);
//extern void displayCurrEnv(LEXEME* env);
	return eval(cdr(tree), object);
}

LEXEME* evalLessThan(LEXEME* tree, LEXEME* env) {	
	LEXEME* left = eval(car(tree), env);
	LEXEME* right = eval(cdr(tree), env);

	if(getType(left) == INTEGER && getType(right) == INTEGER) {
		if(getIntVal(left) < getIntVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == REAL && getType(right) == INTEGER) {
		if(getDoubleVal(left) < getIntVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == INTEGER && getType(right) == REAL) {
		if(getIntVal(left) < getDoubleVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == REAL && getType(right) == REAL) {
		if(getDoubleVal(left) < getDoubleVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	if(getType(left) == STRING && getType(right) == STRING) {
		if(strcmp(getStrVal(left), getStrVal(right)) < 0) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	if(getType(left) == CHARACTER && getType(right) == CHARACTER) {
		if(getCharVal(left) < getCharVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else {
		printf("Fatal error: trying to compare %s with %s\n", getType(left), getType(right));
		exit(1);
		return 0;
	}
}

LEXEME* evalGreaterThan(LEXEME* tree, LEXEME* env) {
	LEXEME* left = eval(car(tree), env);
	LEXEME* right = eval(cdr(tree), env);

	if(getType(left) == INTEGER && getType(right) == INTEGER) {
		if(getIntVal(left) > getIntVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else {
			return newLexemeInt(INTEGER, 0);
		}
	}
	else if(getType(left) == REAL && getType(right) == INTEGER) {
		if(getDoubleVal(left) > getIntVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == INTEGER && getType(right) == REAL) {
		if(getIntVal(left) > getDoubleVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == REAL && getType(right) == REAL) {
		if(getDoubleVal(left) > getDoubleVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	if(getType(left) == STRING && getType(right) == STRING) {
		if(strcmp(getStrVal(left), getStrVal(right)) > 0) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	if(getType(left) == CHARACTER && getType(right) == CHARACTER) {
		if(getCharVal(left) > getCharVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else {
		printf("Fatal error: trying to compare %s with %s\n", getType(left), getType(right));
		exit(1);
		return 0;
	}
}

LEXEME* evalEqualTo(LEXEME* tree, LEXEME* env) {
	LEXEME* left = eval(car(tree), env);
	LEXEME* right = eval(cdr(tree), env);
	
	if(getType(left) == INTEGER && getType(right) == INTEGER) {
		if(getIntVal(left) == getIntVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == REAL && getType(right) == INTEGER) {
		if(getDoubleVal(left) == getIntVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == INTEGER && getType(right) == REAL) {
		if(getIntVal(left) == getDoubleVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == REAL && getType(right) == REAL) {
		if(getDoubleVal(left) == getDoubleVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	if(getType(left) == STRING && getType(right) == STRING) {
		if(strcmp(getStrVal(left), getStrVal(right)) == 0) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	if(getType(left) == CHARACTER && getType(right) == CHARACTER) {
		if(getCharVal(left) == getCharVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else {
		printf("Fatal error: trying to compare %s with %s\n", getType(left), getType(right));
		exit(1);
		return 0;
	}
}

LEXEME* evalNot(LEXEME* tree, LEXEME* env) {
	LEXEME* left = eval(car(tree), env);
	LEXEME* right = eval(cdr(tree), env);

	if(getType(left) == INTEGER && getType(right) == INTEGER) {
		if(getIntVal(left) != getIntVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == REAL && getType(right) == INTEGER) {
		if(getDoubleVal(left) != getIntVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == INTEGER && getType(right) == REAL) {
		if(getIntVal(left) != getDoubleVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else if(getType(left) == REAL && getType(right) == REAL) {
		if(getDoubleVal(left) != getDoubleVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	if(getType(left) == STRING && getType(right) == STRING) {
		if(strcmp(getStrVal(left), getStrVal(right)) != 0) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	if(getType(left) == CHARACTER && getType(right) == CHARACTER) {
		if(getCharVal(left) != getCharVal(right)) {
			return newLexemeInt(INTEGER, 1);
		}
		else return newLexemeInt(INTEGER, 0);
	}
	else {
		printf("Fatal error: trying to compare %s with %s\n", getType(left), getType(right));
		exit(1);
		return 0;
	}
}

LEXEME* evalAssign(LEXEME* tree, LEXEME* env) {		//TODO edit this to take care of DOT
	LEXEME* value = eval(cdr(tree), env);
	if(getType(car(tree)) == VARIABLE) {
		update(car(tree), value, env);
	}
	else if(getType(car(tree)) == DOT) {
		LEXEME* object = eval(car(car(tree)), env);
		update(car(cdr(tree)), value, object);
	}
	else{
		printf("Bad expression!\n");
		exit(1);
	}
	
	return value;
}

LEXEME* evalPlus(LEXEME* tree, LEXEME* env) {
	LEXEME* left = eval(car(tree), env);
	LEXEME* right = eval(cdr(tree), env);
	if(getType(left) == INTEGER && getType(right) == INTEGER) {
		return newLexemeInt(INTEGER, getIntVal(left) + getIntVal(right));
	}
	else if(getType(left) == INTEGER && getType(right) == REAL) {
		return newLexemeReal(REAL, getIntVal(left) + getDoubleVal(right));
	}
	else if(getType(left) == REAL && getType(right) == INTEGER) {
		return newLexemeReal(REAL, getDoubleVal(left) + getIntVal(right));
	}
	else {
		return newLexemeReal(REAL, getDoubleVal(left) + getDoubleVal(right));
	}
}

LEXEME* evalMinus(LEXEME* tree, LEXEME* env) {
	LEXEME* left = eval(car(tree), env);
	LEXEME* right = eval(cdr(tree), env);
	if(getType(left) == INTEGER && getType(right) == INTEGER) {
		return newLexemeInt(INTEGER, getIntVal(left) - getIntVal(right));
	}
	else if(getType(left) == INTEGER && getType(right) == REAL) {
		return newLexemeReal(REAL, getIntVal(left) - getDoubleVal(right));
	}
	else if(getType(left) == REAL && getType(right) == INTEGER) {
		return newLexemeReal(REAL, getDoubleVal(left) - getIntVal(right));
	}
	else {
		return newLexemeReal(REAL, getDoubleVal(left) - getDoubleVal(right));
	}
}

LEXEME* evalTimes(LEXEME* tree, LEXEME* env) {
	LEXEME* left = eval(car(tree), env);
	LEXEME* right = eval(cdr(tree), env);
	if(getType(left) == INTEGER && getType(right) == INTEGER) {
		return newLexemeInt(INTEGER, getIntVal(left) * getIntVal(right));
	}
	else if(getType(left) == INTEGER && getType(right) == REAL) {
		return newLexemeReal(REAL, getIntVal(left) * getDoubleVal(right));
	}
	else if(getType(left) == REAL && getType(right) == INTEGER) {
		return newLexemeReal(REAL, getDoubleVal(left) * getIntVal(right));
	}
	else {
		return newLexemeReal(REAL, getDoubleVal(left) * getDoubleVal(right));
	}
}

LEXEME* evalDivide(LEXEME* tree, LEXEME* env) {
	LEXEME* left = eval(car(tree), env);
	LEXEME* right = eval(cdr(tree), env);
	if(getType(left) == INTEGER && getType(right) == INTEGER) {
		return newLexemeInt(INTEGER, getIntVal(left) / getIntVal(right));
	}
	else if(getType(left) == INTEGER && getType(right) == REAL) {
		return newLexemeReal(REAL, getIntVal(left) / getDoubleVal(right));
	}
	else if(getType(left) == REAL && getType(right) == INTEGER) {
		return newLexemeReal(REAL, getDoubleVal(left) / getIntVal(right));
	}
	else {
		return newLexemeReal(REAL, getDoubleVal(left) / getDoubleVal(right));
	}
}

LEXEME* evalArgList(LEXEME* tree, LEXEME* env) {
	LEXEME* result = newLexeme(EVALEDARGLIST);
	LEXEME* ptr = result;

	while(tree != NULL) {
		LEXEME* val = eval(car(tree), env); 
		setCar(ptr, val);
		tree = cdr(tree);

		if(tree != NULL) {
			setCdr(ptr, newLexeme(EVALEDARGLIST));
			ptr = cdr(ptr);
		}
	}
	
	return result;
}

LEXEME* evalNewArray(LEXEME* tree, LEXEME* env) {
	LEXEME* argList = eval(car(tree), env);
	LEXEME* a = newLexemeArr(ARRAY, getIntVal(car(argList)));
	return a;
}

LEXEME* evalGetArray(LEXEME* tree, LEXEME* env) {
	LEXEME* argList = eval(car(tree), env);
	LEXEME* a = car(argList);
	LEXEME* b = car(cdr(argList));
	return getArrayVal(extractArr(a), getIntVal(b));
}


LEXEME* evalSetArray(LEXEME* tree, LEXEME* env) {
	LEXEME* argList = eval(car(tree), env);
	LEXEME* a = car(argList);
	LEXEME* i = car(cdr(argList));
	LEXEME* v = car(cdr(cdr(argList)));
	
	setArrVal(extractArr(a), getIntVal(i), v);

	return v;
}

LEXEME* evalOpenFile(LEXEME* tree, LEXEME* env) {
	LEXEME* fileName = car(eval(car(tree), env));
	LEXEME* fp = newFileLexeme(FILEPTR, getStrVal(fileName));
	setFileVal(fp, fopen(getStrVal(fileName), "r"));
	return fp;
}

LEXEME* evalReadInteger(LEXEME* tree, LEXEME* env) {
	FILE* filePtr = getFileVal(car(eval(car(tree), env)));
	int x;
	fscanf(filePtr, "%d", &x);
	return newLexemeInt(INTEGER, x);
}

LEXEME* evalAtFileEnd(LEXEME* tree, LEXEME* env) {
	FILE* filePtr = getFileVal(car(eval(car(tree), env)));
	if(feof(filePtr)) {
		return newLexemeInt(INTEGER, 1);
	}
	else {
		return newLexemeInt(INTEGER, 0);
	}
}

LEXEME* evalCloseFile(LEXEME* tree, LEXEME* env) {
	FILE* filePtr = getFileVal(car(eval(car(tree), env)));
	fclose(filePtr);
	return newLexemeInt(INTEGER, 1);
}

LEXEME* evalGetArgCount() {
	return newLexemeInt(INTEGER, countCL);
}

LEXEME* evalGetArgAt(LEXEME* tree, LEXEME* env) {
	LEXEME* index = car(eval(car(tree), env));
	return newLexemeStr(STRING, argsCL[getIntVal(index)]);
}

LEXEME* evalPrint(LEXEME* tree, LEXEME* env) {
	LEXEME* argList = eval(car(tree), env);
	LEXEME* result = NULL;
	while(argList != NULL) {
		result = car(argList);
		if(getType(car(argList)) == INTEGER) {
			printf("%d", getIntVal(car(argList)));
		}
		else if(getType(car(argList)) == REAL) {
			printf("%lf", getDoubleVal(car(argList)));
		}
		else if(getType(car(argList)) == CHARACTER) {
			printf("%c", getCharVal(car(argList)));
		}
		else {
			if(strcmp(getStrVal(car(argList)), "\\n") == 0) {
				printf("\n");
			}
			else {
				printf("%s", getStrVal(car(argList)));
			}
		}	
		argList = cdr(argList);
	}
	return result;
}






