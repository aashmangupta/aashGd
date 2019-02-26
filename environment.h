#include <stdio.h>
#include "types.h"
#include "lexeme.h"

extern LEXEME* newEnvironment();
extern LEXEME* update(LEXEME* variable, LEXEME* value, LEXEME* env);
extern LEXEME* lookup(LEXEME* variable, LEXEME* env);
extern LEXEME* insert(LEXEME* variable, LEXEME* value, LEXEME* env);
extern LEXEME* extend(LEXEME* vars, LEXEME* vals, LEXEME* env);
extern void displayCurrEnv(LEXEME* env);
extern void displayAll(LEXEME* env);
