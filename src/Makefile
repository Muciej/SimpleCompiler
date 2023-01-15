FLAGS = -W -pedantic -std=c++17
.PHONY = all clean cleanall

all: kompilator

kompilator: lexer.o parser.o main.o logic.o
	$(CXX) $^ -o $@
	strip $@

%.o: %.cpp
	$(CXX) $(FLAGS) -c $^

lexer.cpp: lexer.l parser.hh
	flex -o $@ $<

parser.cpp parser.hh: parser.y
	bison -Wall -d -o parser.cpp $^

clean:
	rm -f *.o parser.cpp parser.hpp lexer.cpp

cleanall: clean
	rm -f kompilator debugfile.txt *.geba