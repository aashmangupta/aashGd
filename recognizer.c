#include <stdio.h>
#include "recognizer.h"
#include "environment.h"
#include "types.h"

LEXEME* currLexeme;

LEXEME* program() {
	LEXEME* p = NULL;
	LEXEME* d = definition();
	if(definitionPending()) {
		p = program();
	}
	else if(statementPending()) {
		p = statements();
	}
	return cons(PROGRAM, d, cons(PROGRAM, p, NULL));
}

int programPending() {
	return (definitionPending() || statementPending());
}

LEXEME* definition() {
	if(varDefPending()) {
		return varDef();
	}
	else {
		return functionDef();
	}
}

int definitionPending() {
	return (varDefPending() || functionDefPending());	
}

LEXEME* expression() {			//TODO started on this func. for pretty print
	char* type;
	LEXEME *u, *o, *e;
	e = NULL;
	u = unary();
	if(operatorPending()) {
		type = getType(currLexeme);
		o = operators();
		if((type != MINUSMINUS) && (type != PLUSPLUS)) {
			e = expression();
		}
		else {
			match(SEMICOLON);
		}
		return cons(getType(o), u, e);
	}
	return u;
}

int expressionPending() {
	return unaryPending();
}

LEXEME* operators() {
	if(check(PLUS)) return match(PLUS);
	else if(check(TIMES)) return match(TIMES);
	else if(check(DIVIDES)) return match(DIVIDES);
	else if(check(DOT)) return match(DOT);
	else if(check(MINUS)) return match(MINUS);
	else if(check(ASSIGN)) return match(ASSIGN);
	else if(check(EQUALTO)) return match(EQUALTO);
	else if(check(AND)) return match(AND);
	else if(check(OR)) return match(OR);
	else if(check(NOT)) return match(NOT);
	else if(check(LESSTHAN)) return match(LESSTHAN);
	else if(check(PLUSPLUS)) return match(PLUSPLUS);
	else if(check(MINUSMINUS)) return match(MINUSMINUS);
	else return match(GREATERTHAN);
}

int operatorPending() {
	if(check(PLUS) || check(TIMES) || check(DOT) || check(DIVIDES) || check(MINUS) || check(ASSIGN)) {
		return 1;
	}

	if(check(EQUALTO) || check(AND) || check(OR) || check(NOT) || check(LESSTHAN) || check(GREATERTHAN)) {
		return 1;
	}

	if(check(PLUSPLUS) || check(MINUSMINUS)) {
		return 1;
	}
	return 0;
}

LEXEME* unary() {
	LEXEME *right, *left;
	if(check(INTEGER)) return match(INTEGER);
	else if(check(REAL)) return match(REAL);
	else if(check(CHARACTER)) return match(CHARACTER);
	else if(check(STRING)) {
		LEXEME* str = match(STRING);
		return str;
	}
	else if(check(EXIT)) {
		match(EXIT);
		return cons(EXIT, NULL, NULL);
	}
	else if(check(PRINT)) {
		match(PRINT);
		match(OBRACKET);
		left = optArgList();
		match(CBRACKET);
		return cons(PRINT, left, NULL);
	}
	else if(check(GETARGCOUNT)) {
		match(GETARGCOUNT);
		match(OPAREN);
		match(CPAREN);
		return cons(GETARGCOUNT, NULL, NULL);
	}
	else if(check(GETARGAT)) {
		match(GETARGAT);
		match(OPAREN);
		left = argList();
		match(CPAREN);
		return cons(GETARGAT, left, NULL);
	}
	else if(check(OPENFILE)) {
		match(OPENFILE);
		match(OPAREN);
		left = argList();
		match(CPAREN);
		return cons(OPENFILE, left, NULL);
	}
	else if(check(READINTEGER)) {
		match(READINTEGER);
		match(OPAREN);
		left = argList();
		match(CPAREN);
		return cons(READINTEGER, left, NULL);
	}
	else if(check(ATFILEEND)) {
		match(ATFILEEND);
		match(OPAREN);
		left = argList();
		match(CPAREN);
		return cons(ATFILEEND, left, NULL);
	}
	else if(check(CLOSEFILE)) {
		match(CLOSEFILE);
		match(OPAREN);
		left = argList();
		match(CPAREN);
		return cons(CLOSEFILE, left, NULL);	
	}
	else if(check(NEWARRAY)) {
		match(NEWARRAY);
		match(OPAREN);
		left = argList();
		match(CPAREN);
		return cons(NEWARRAY, left, NULL);
	}
	else if(check(GETARRAY)) {
		match(GETARRAY);
		match(OPAREN);
		left = argList();
		match(CPAREN);
		return cons(GETARRAY, left, NULL);
	}
	else if(check(SETARRAY)) {
		match(SETARRAY);
		match(OPAREN);
		left = argList();
		match(CPAREN);
		return cons(SETARRAY, left, NULL);
	}
	else if(check(OPAREN)) {
		match(OPAREN);
		left = expression();
		match(CPAREN);
		return cons(OPAREN, left, NULL);
	}
	else if(check(MINUS)) {
		match(MINUS);
		left = unary();
		return cons(UMINUS, left, NULL);
	}
	else if(check(NOT)) {
		match(NOT);
		left = unary();
		return cons(NOT, left, NULL);
	}
	else if(check(RETURN)) {
		match(RETURN);
		left = expression();
		right = NULL;
		return cons(RETURN, left, right);
	}
	
	else if(check(LAMBDA)) {
		match(LAMBDA);
		match(OPAREN);
		LEXEME* rl = optParamList();
		match(CPAREN);
		LEXEME* rr = block();
		return cons(LAMBDA, NULL, cons(JOIN, rl, rr));
	}

	else {
		right = match(VARIABLE);
		left = NULL;
		if(funcCallPending()) {
			left = funcCall();
			setCdr(left, right);
		}
		return cons(VARIABLE, left, right);			//FIXME
	}
}

int unaryPending() {
	if(check(INTEGER) || check(NEWARRAY) || check(GETARRAY) || check(REAL) || check(CHARACTER) || check(VARIABLE) || check(STRING)) {
		return 1;
	}
	if(check(OPAREN) || check(LAMBDA) || check(PRINT) || check(SETARRAY) || check(MINUS) || check(NOT) || check(RETURN) || funcCallPending()) {
		return 1;
	}
	if(check(OPENFILE) || check(EXIT) || check(GETARGCOUNT) || check(GETARGAT) || check(READINTEGER) || check(ATFILEEND) || check(CLOSEFILE)) {
		return 1;
	}
	return 0;
}

LEXEME* funcCall() {
	LEXEME* list;
	match(OPAREN);
	list = optArgList();
	match(CPAREN);
	return cons(FUNC_CALL, list, NULL);
}

int funcCallPending() {
	return check(OPAREN);
}

LEXEME* argList() {
	LEXEME *a, *b;
	b = NULL;
	a = expression();
	if(check(COMMA)) {
		match(COMMA);
		b = argList();
	}
	return cons(ARGLIST, a, b);
}

int argListPending() {
	return expressionPending();
}

LEXEME* optArgList() {
	if(argListPending()) {
		return argList();
	}
	return NULL;
}

int optArgListPending() {
	return argListPending();
}

LEXEME* ifstatement() {
	LEXEME *l, *rl, *rr;
	match(IF);
	match(OPAREN);
	l = expression();
	match(CPAREN);
	rl = block();
	rr = optElse();
	return cons(IF, l, cons(JOIN, rl, rr));
}

int ifstatementPending() {
	return check(IF);
}

LEXEME* elsestatement() {
	match(ELSE);
	LEXEME* l;

	if(blockPending()) {
		l = block();
	}
	else {
		l = ifstatement();
	}
	return cons(ELSE, l, NULL);
}

int elsestatementPending() {
	return check(ELSE);
}

LEXEME* optElse() {
	if(elsestatementPending()) {
		return elsestatement();
	}
	return NULL;
}

int optElsePending() {
	return elsestatementPending();
}

LEXEME* whileLoop() {
	LEXEME *l, *r;

	match(WHILE);
	match(OPAREN);
	l = expression();
	match(CPAREN);
	r = block();
	return cons(WHILE, l, r);
}

int whileLoopPending() {
	return check(WHILE);
}

LEXEME* block() {
	match(OBRACE);
	LEXEME* a = optStatementList();
	match(CBRACE);
	return cons(BLOCK, a, NULL);
}

int blockPending() {
	return check(OBRACE);
}

LEXEME* optStatementList() {
	if(statementPending()) {
		return statementList();
	}
	else {
		return NULL;
	}
}

LEXEME* statementList() {
	LEXEME* a = statements();
	LEXEME* b = NULL;

	if(statementListPending()) {
		b = statementList();
	}
	return cons(STATEMENTLIST, a, b);
}

int statementListPending() {
	return statementPending();
}

LEXEME* statements() {
	LEXEME* a;
	if(expressionPending()) {
		a = expression();
		match(SEMICOLON);
	}
	else if(ifstatementPending()) {
		a = ifstatement();
	}
	else if(whileLoopPending()) {
		a = whileLoop();
	}
	else {
		a = definition();
	}
	return cons(STATEMENT, a, NULL);
}

int statementPending() {
	return (expressionPending() || ifstatementPending() || whileLoopPending() || definitionPending());
}

LEXEME* functionDef() {
	LEXEME *a, *rl, *rr;

	match(FUNCTION_DEF);
	a = match(VARIABLE);
	match(OPAREN);
	rl = optParamList();
	match(CPAREN);
	rr = block();

	return cons(FUNCTION_DEF, a, cons(JOIN, rl, rr));
}

int functionDefPending() {
	return check(FUNCTION_DEF);
}

LEXEME* parameterList() {
	match(VAR_TYPE);
	LEXEME* b = NULL;
	LEXEME* a = match(VARIABLE);
	if(check(COMMA)) {
		match(COMMA);
		b = parameterList();
	}
	return cons(PARAMLIST, a, b);
}

int parameterListPending() {
	return check(VAR_TYPE);
}

LEXEME* optParamList() {
	if(parameterListPending()) {
		return parameterList();
	}
	return NULL;
}

int optParamListPending() {
	return parameterListPending();
}

LEXEME* varDef() {
	match(VAR_TYPE);
	LEXEME* b = NULL;
	LEXEME* a = match(VARIABLE);
	if(check(ASSIGN)) {
		match(ASSIGN);
		b = expression();
		match(SEMICOLON);
	}
	else {
		match(SEMICOLON);
	}
	return cons(VARDEF, a, b);
}

int varDefPending() {
	return check(VAR_TYPE);
}

/////////////////////////////////////////////////////// HELPER FUNCTION /////////////////////////////////
int check(char* type) {		//checks type of currLexeme
	return (getType(currLexeme) == type);
}

LEXEME* advance() {			//moves on to the next lexeme
	LEXEME* old = currLexeme;
	currLexeme = lex();
	return old;
}

LEXEME* match(char* type) 	{	//matches type and then advances
	matchNoAdvance(type);
	return advance();
}

void matchNoAdvance(char* type) {	//if types didn't match, it will be reported here
	if(!check(type)) {
		printf("ERROR: Syntax error...\n");
		printf("\nExpected: %s\n", type);
		printf("Found: ");
		displayToken(currLexeme);
		printf("\nCheck line number: %d\n", getLineNum());
		printf("illegal\n");
		exit(1);
	}
}

