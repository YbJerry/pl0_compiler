main: globals.h main.cpp lexer.o
	g++ -g lexer.o main.cpp -o main

lexer.o: globals.h lexer.cpp lexer.h
	g++ -g -c lexer.cpp -o lexer.o