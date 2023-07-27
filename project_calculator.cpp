/* 

Matthew Buchanan
24 July 2023

Project Calculator
	This program is a command-line calculator program with the goal to build basic proficiency in C++ and software development.

	Goals:
		1. utilize at least one class type
		2. utilize at least one linked list
		3. develop command-line parser for command-line execution and input
		4. develop basic graphical user interface for execution and input
		5. develop program version in C++ and Python
		6. analyze assembly code to understand low-level actions occuring in the program
		7. analyze memory usage to understand stack vs heap memory allocated

	Tasks:
		1. setup command-line GCC to complile C++ code...DONE!
		2. build basic arithmetic calculator that can add two numbers via prompt input after runtime...DONE!
		3. setup command-line git to enable upload of code into a github repository.
		5. setup add capability as its own "object" (function or class).
		4. add subtraction capability.
		5. add multiplication capability.
		6. add division capability.
		7. develop capability to input and parse out a string (e.g. 1+4 instead of 1 ENTER 4 ENTER).
		8. program in the order of operations to correctly solve problems in order and detect bad inputs

Git Commands
	git config --global user.name "buchananmatt"
	git config --global user.email buch.matt@gmail.com
	git init
	git remote add origin https://github.com/buchananmatt/projectcalculator
	git add .
	git commit -m "initial project file and day one progress"
	git push origin master
	

*/

#include<iostream>

using namespace std;

int main() 
{
	int x = 0; // max value is 2,147,483,647. how much memory?
	int y = 0;
	int z = 0;

	cout << "Project Calculator" << endl;
	cout << "Input first addend..." << endl;
	cin >> x;
	cout << "Input second addend..." << endl;
	cin >> y;
	z = x + y;
	cout << "The sum of " << x << " and " << y << " is equal to " << z << "." << endl;

	return 0;

}