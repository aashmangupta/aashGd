# Makefile for evaluator  module of the AASHGOD language
# Created by Walt Loumakis

OBJS = lexeme.o types.o lexer.o recognizer.o evaluator.o environment.o
TARGS = evaluator
OBJFL = -Wall -Wextra -g -c -std=c99
TARFL = -Wall -Wextra -g -std=c99

# Makes all object files and executables
all: $(TARGS)

# Makes the pretty executable
evaluator: $(OBJS) main.c
	gcc $(TARFL) $(OBJS) main.c -o evaluator

# Makes the Lexeme object file
lexeme.o: lexeme.c lexeme.h
	gcc $(OBJFL) lexeme.c

# Makes the Types object file
types.o: types.c types.h
	gcc $(OBJFL) types.c

# Makes the Lexer object file
lexer.o: lexer.c lexer.h
	gcc $(OBJFL) lexer.c

# Makes the Recognizer object file
recognizer.o: recognizer.c recognizer.h
	gcc $(OBJFL) recognizer.c

environment.o: environment.c environment.h
	gcc $(OBJFL) environment.c

# Makes the Pretty object file
evaluator.o: evaluator.c evaluator.h
	gcc $(OBJFL) evaluator.c

#Finally, provide an executable shellscript named run that runs a given program.
    #!/bin/bash
 #   ./mylang $*

test: all
	@echo arrays:
	@./evaluator arrays.ag
	@echo conditionals:
	@./evaluator conditionals.ag
	@echo recursion:
	@./evaluator recursion.ag
	@echo iteration:
	@./evaluator iteration.ag
	@echo function:
	@./evaluator function.ag
	@echo lamda:
	@./evaluator lambda.ag
	@echo object:
	@./evaluator object.ag
	@echo AVL1
	@./evaluator iAVL.ag file
	@echo AVL2
	@./evaluator iAVL.ag file2

error1: evaluator
	@echo error1.ag:
	@cat error1.ag

error1x: evaluator
	@echo executing
	@./evaluator error1.ag

error2: evaluator
	@echo error2.ag:
	@cat error2.ag

error2x: evaluator
	@echo executing:
	@./evaluator error2.ag

error3: evaluator
	@echo error3.ag:
	@cat error3.ag

error3x: evaluator
	@echo executing:
	@./evaluator error3.ag

error4: evaluator
	@echo error4.ag:
	@cat error4.ag

error4x: evaluator
	@echo executing:
	@./evaluator error4.ag

error5: evaluator
	@echo error5.ag:
	@cat error5.ag

error5x: evaluator
	@echo executing:
	@./evaluator error5.ag

arrays: evaluator
	@echo arrays.ag
	@cat arrays.ag

arraysx: evaluator
	@echo executing:
	@./evaluator arrays.ag

conditionals: evaluator
	@echo conditionals.ag:
	@cat conditionals.ag

conditionalsx: evaluator
	@echo executing:
	@./evaluator conditionals.ag

recursion: evaluator
	@echo recursion.ag:
	@cat recursion.ag

recursionx: evaluator
	@echo executing:
	@./evaluator recursion.ag

iteration: evaluator
	@echo iteration.ag:
	@cat iteration.ag

iterationx: evaluator
	@echo executing:
	@./evaluator iteration.ag

functions: evaluator
	@echo function.ag:
	@cat function.ag

functionsx: evaluator
	@echo executing:
	@./evaluator function.ag

lambda: evaluator
	@echo lambda.ag:
	@cat lambda.ag

lambdax: evaluator
	@echo executing:
	@./evaluator lambda.ag

objects: evaluator
	@echo object.ag:
	@cat object.ag

objectsx: evaluator
	@echo executing:
	@./evaluator object.ag

problem: evaluator
	@echo iAVL.ag
	@cat iAVL.ag

problemx: evaluator
	@echo executing:
	@./evaluator iAVL.ag file

# Cleans object files and executables
clean:
	rm -f $(OBJS) $(TARGS)
