CXX=clang++
CXXFLAGS=-std=c++14

output: ./src/main.o ./src/calculator/calculator.o
	g++ ./src/main.o ./src/calculator/calculator.o -o ./bin/calc.out

main.o: ./src/main.cpp
	c++ -c ./src/main.cpp

calculator.o: ./src/calculator/calculator.cpp ./src/calculator/calculator.hpp
	c++ -c ./src/calculator/calculator.cpp

clean:
	rm ./src/*.o
	rm ./src/calculator/*.o