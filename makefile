
output: ./src/main.o ./src/calculator.o
	g++ ./src/main.o ./src/calculator.o -o calc.out

main.o: ./src/main.cpp
	g++ -c ./src/main.cpp

calculator.o: ./src/calculator.cpp ./src/calculator.hpp
	g++ -c ./src/calculator.cpp

clean:
	rm ./src/*.o
	rm calc.out