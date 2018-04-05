CXX=g++
CXXFLAGS=-g --std=c++11
SHELL=/bin/bash

all: prog dotest

OBJECTS=main.o db.o init.o

#
# $@ будет заменено на то, что стоит справа от символа ":", то есть на "test"
# 

#стандартный шаблон создания объектных файлов
%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<  -o $@ 

prog: main.o db.o init.o
	$(CXX) --std=c++11 -o prog $(OBJECTS)

init.cpp: databasestudents.h text/csv/ostream.hpp text/csv/istream.hpp
main.cpp: databasestudents.h 
db.cpp: databasestudents.h 
