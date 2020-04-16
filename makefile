main: globals.h main.cpp lexer.o util.o
	g++ -g lexer.o util.o main.cpp -o main

util.o: util.cpp
	g++ -g -c util.cpp -o util.o

lexer.o: globals.h lexer.cpp lexer.h
	g++ -g -c lexer.cpp -o lexer.o