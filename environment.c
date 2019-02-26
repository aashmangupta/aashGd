#include "environment.h"
#include <stdio.h>

LEXEME* newEnvironment() {
	return cons(ENVIRONMENT, NULL, cons(VALUES, NULL, NULL));
}

//cons(type, left, right)
//car-> left
//cdr-> right

LEXEME* extend(LEXEME* vars, LEXEME* vals, LEXEME* env) {
	return cons(ENVIRONMENT, vars, cons(VALUES, vals, env));
}

LEXEME* insert(LEXEME* variable, LEXEME* value, LEXEME* env) {
	setCar(env, cons(JOIN, variable, car(env)));
	setCar(cdr(env), cons(JOIN, value, car(cdr(env))));
	return value;
}

LEXEME* lookup(LEXEME* variable, LEXEME* env) {
	LEXEME *vars, *vals;
	
	if(getStrVal(variable) == NULL) {
		variable = cdr(variable);
	}

	while(env != NULL) {
		vars = car(env);
		vals = car(cdr(env));
		while(vars != NULL) {
			if(sameVariable(variable, car(vars))) {
				return car(vals);
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(cdr(env));
	}
	
	printf("FATAL ERROR: variable ");
	displayToken(variable);
	printf("is undefined.\n");
	exit(1);
	return NULL;
}

LEXEME* update(LEXEME* variable, LEXEME* value, LEXEME* env) {
	LEXEME *vars, *vals;
	
	if(getStrVal(variable) == NULL) {
		variable = cdr(variable);
	}
	while(env != NULL) {
		vars = car(env);
		vals = car(cdr(env));
		while(vars != NULL) {
			if(sameVariable(variable, car(vars))) {
				if(getType(car(vals)) != getType(value)) {
					printf("FATAL ERROR: update var type does no match...\n");
					exit(1);
				}
				setCar(vals, value);	//TODO
				//displayCurrEnv(env);
				return car(vals);
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(cdr(env));
	}
	
	printf("FATAL ERROR: variable ");
	displayToken(variable);
	printf("is undefined.\n");
	exit(1);

	return NULL;
}

void displayCurrEnv(LEXEME* env) {
	LEXEME *vars, *vals;
	printf("Table:\n");
	
	vars = car(env);
	vals = car(cdr(env));
	while(vars != NULL) {
		if(getType(car(vars)) == VARIABLE) {
			printf("	%s = ", getStrVal(car(vars)));
		}
		if(getType(car(vals)) == INTEGER) {
			printf("%d\n", getIntVal(car(vals)));
		}
		else if(getType(car(vals)) == REAL) {
			printf("%lf\n", getDoubleVal(car(vals)));
		}
		else if(getType(car(vals)) == CHARACTER) {
			printf("%c\n", getCharVal(car(vals)));
		}
		else if(getType(car(vals)) == STRING) {
			printf("%s\n", getStrVal(car(vals)));
		}
		else if(getType(car(vals)) == CLOSURE) {
			printf("CLOSURE\n");
		}
		else if(getType(car(vals)) == ENVIRONMENT) {
			printf("ENVIRONMENT\n");
		}
		else {
			printf("Uknown\n");
		}
		vars = cdr(vars);
		vals = cdr(vals);
	}
}

void displayAll(LEXEME* env) {
	LEXEME *vars, *vals;
	int tableNum = 0;

	while(env != NULL) {
		printf("Table %d:\n", tableNum);
	
		vars = car(env);
		vals = car(cdr(env));
		while(vars != NULL) {
			if(getType(car(vars)) == VARIABLE) {
				printf("	%s = ", getStrVal(car(vars)));
			}	
			if(getType(car(vals)) == INTEGER) {
				printf("%d\n", getIntVal(car(vals)));
			}
			else if(getType(car(vals)) == REAL) {
				printf("%lf\n", getDoubleVal(car(vals)));
			}
			else if(getType(car(vals)) == CHARACTER) {
				printf("%c\n", getCharVal(car(vals)));
			}
			else if(getType(car(vals)) == STRING) {
				printf("%s\n", getStrVal(car(vals)));
			}
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(cdr(env));
		tableNum++;
	}
	printf("\n");
}


