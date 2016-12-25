# Это комментарий, который говорит, что переменная CC указывает компилятор, используемый для сборки
CC=g++
#Это еще один комментарий. Он поясняет, что в переменной CFLAGS лежат флаги, которые передаются компилятору
CFLAGS=-c -Wall
LFLAGS=-lsfml-graphics -lsfml-window -lsfml-system

all: main

main: main.o
	$(CC) main.o $(LFLAGS) -o Snake

main.o: ./src/main.cpp
	$(CC) $(CFLAGS) ./src/main.cpp 


