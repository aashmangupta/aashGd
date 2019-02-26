//Lexical Analyzer
//Author: Aashman Gupta

#include "lexer.h"
#include "lexeme.h"
#include "types.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

FILE* srcFile;						//sourceFile
char ch;							//currently read character
char pushbackChar;					//character that was pushed back
bool charAlreadyPushed = false;		//boolean if pushback happened or not
int readSuccessful = 1;				//variable to see if fscanf worked successfully
int lineNum = 1;

void recieveFile(FILE* passedFile) {		//recieve srcFile from scanneri
	srcFile = passedFile;
}

int getLineNum() {
	return lineNum;
}

void myPushback() {			//pushes back
	if(charAlreadyPushed) {
		printf("ERROR: Too many pushbacks...\n");
		exit(1);
	}
	charAlreadyPushed = true;
	pushbackChar = ch;
}

void myRead() {						//reads character by character into the global variable ch
	if(charAlreadyPushed) {
		charAlreadyPushed = false;
		ch = pushbackChar;          
	}
	else {
		if(feof(srcFile)) {
			readSuccessful = 0;
		}
		else {
			readSuccessful = 1;
		}
		fscanf(srcFile, "%c", &ch);
	}
}

void skipWhiteSpace() {                //skip any whitespace
	int flag = 0;
	while(isspace(ch) && !feof(srcFile)) {
		myRead();
		flag = 1;
	}

	if(flag == 1) {
		myPushback();                    //pushback since the char that broke the loop isn't whitespace
	}
}

int invisWhitespace() {					//returns 1 for end of file and 0 for void
	while(isspace(ch)) {				//here to handle the invisible space values	
		if(ch == '\n') {				//Counts number of lines
			lineNum++;
		}
		myRead();
		if(feof(srcFile)) {
			return 1;
		}
	}
	return 0;
}

void handleComments() {
	if(ch == '#') {						//here to handle single line comment
		while(ch != '\n') {
			myRead();
		}
		myRead();
		lineNum++;
	}

	if(ch == '$') {
		myRead();
		while(ch != '$') {
			myRead();
			if(ch == '\n') {
				lineNum++;
			}
		}
		myRead();
	}

}

LEXEME* lex() {
	skipWhiteSpace();
	myRead();
	
	int endOfFile = invisWhitespace();
	
	if(endOfFile == 1) {
		return newLexeme(ENDOFFILE);
	}
	
	while(ch == '#' || ch == '$') {
		handleComments();
		endOfFile = invisWhitespace();
	}
	
	if(endOfFile == 1) {
		return newLexeme(ENDOFFILE);
	}
	
	switch(ch) {
		case '[': return newLexeme(OBRACKET);
		case ']': return newLexeme(CBRACKET);
		case '(': return newLexeme(OPAREN); 	
		case ')': return newLexeme(CPAREN);
		case ',': return newLexeme(COMMA); 
		case '+': 
			myRead();
			if(ch == '+') {
				return newLexeme(PLUSPLUS);
			}
			else {
				myPushback();
				return newLexeme(PLUS);
			}
		case '*': return newLexeme(TIMES); 
		case '-':
			myRead();
			if(ch == '-') {
				return newLexeme(MINUSMINUS);
			}
			else {
				myPushback();
				return newLexeme(MINUS);
			}
		case '/': return newLexeme(DIVIDES); 
		case '<': return newLexeme(LESSTHAN); 
		case '>': return newLexeme(GREATERTHAN); 
		case '=':
			myRead();
			if(ch == '=') {
				return newLexeme(EQUALTO);
			}
			else {
				myPushback();
				return newLexeme(ASSIGN);
			}
		case ';': return newLexeme(SEMICOLON); 
		case '{': return newLexeme(OBRACE);
		case '}': return newLexeme(CBRACE);
		case '.': return newLexeme(DOT);
		case '!': return newLexeme(NOT);
		case '&': 
			myRead();
			if(ch == '&') {
				return newLexeme(ANDAND);
			}
			else {
				myPushback();
				return newLexeme(AND);
			}
		case '|': 
			myRead();
			if(ch == '|') {
				return newLexeme(OROR);
			}
			else {
				myPushback();
				return newLexeme(OR);
			}

		default:
			if(isdigit(ch)) {
				myPushback();
				return lexNumber();
			}
			else if(isalpha(ch)) {
				myPushback();
				return lexVariableOrKeyword();
			}
			else if(ch == '\"') return lexString();

			else {
				return newLexeme(UNKNOWN);
			}
			break;
	}	
}

LEXEME* lexVariableOrKeyword() {			//max variable word length is 100
	char token[100];
	int index = 0;							//keeps track of where to insert newly read char
	
	myRead();
	while (isalpha(ch) || isdigit(ch)) {
		token[index] = ch;
		token[index + 1] = '\0';
		index++;
		myRead();
	}
	myPushback();
	
	if(strcmp(token, "if") == 0) return newLexeme(IF);
	else if(strcmp(token, "else") == 0) return newLexeme(ELSE);
	else if(strcmp(token, "while") == 0) return newLexeme(WHILE);	//FIXME might need to add some more cases here
	else if(strcmp(token, "var") == 0) return newLexeme(VAR_TYPE);
	else if(strcmp(token, "terminate") == 0) return newLexeme(TERMINATE);
	else if(strcmp(token, "return") == 0) return newLexeme(RETURN);
	else if(strcmp(token, "function") == 0) return newLexeme(FUNCTION_DEF);
	else if(strcmp(token, "print") == 0) return newLexeme(PRINT);
	else if(strcmp(token, "newArray") == 0) return newLexeme(NEWARRAY);
	else if(strcmp(token, "getArray") == 0) return newLexeme(GETARRAY);
	else if(strcmp(token, "setArray") == 0) return newLexeme(SETARRAY);
	else if(strcmp(token, "openFile") == 0) return newLexeme(OPENFILE);
	else if(strcmp(token, "readInteger") == 0) return newLexeme(READINTEGER);
	else if(strcmp(token, "atFileEnd") == 0) return newLexeme(ATFILEEND);
	else if(strcmp(token, "closeFile") == 0) return newLexeme(CLOSEFILE);
	else if(strcmp(token, "getArgCount") == 0) return newLexeme(GETARGCOUNT);
	else if(strcmp(token, "getArgAt") == 0) return newLexeme(GETARGAT);
	else if(strcmp(token, "lambda") == 0) return newLexeme(LAMBDA);
	else if(strcmp(token, "exit") == 0) return newLexeme(EXIT);
	else return newLexemeStr(VARIABLE, token);
}

LEXEME* lexNumber() {
	char buffer[100];
	int index = 0;
	int decimal = 0;

	myRead();
	while(readSuccessful && (isdigit(ch) || ch == '.')) {
		if(index == 30) {
			printf("ERROR: number is too long...\n");
			printf("check line %d\n", lineNum);
			exit(1);
		}
		buffer[index] = ch;
		buffer[index + 1] = '\0';
		index++;

		if(ch == '.') {
			if(decimal == 0) {
				decimal = 1;
			}
			else {
				printf("ERROR: BAD NUMBER. Too many decimals\n");
				printf("Check line number %d\n", lineNum);
				exit(1);
			}
		}
		myRead();
	}
	myPushback();
	
	if(decimal == 1) {
		return newLexemeReal(REAL, atof(buffer));
	}

	else { 
		return newLexemeInt(INTEGER, atoi(buffer));
	}
}

LEXEME* lexString() {
	int length = 32;
	char* buffer = malloc(sizeof(char) * 33);
	int index = 0;
	
	myRead();
	while(ch != '\"') {
		if(feof(srcFile)) {
			printf("ERROR: Bad string...\n");
			printf("Check line %d\n", lineNum);
			exit(1);
		}
		if(index == length) {
			buffer = realloc(buffer, sizeof(char) * ((length * 2) + 1));
			length *= 2;
		}

		buffer[index] = ch;
		index++;
		myRead();
	}
	buffer[index] = '\0';
	return newLexemeStr(STRING, buffer);
}



