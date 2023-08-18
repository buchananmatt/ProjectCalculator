// 
// CALCULATOR.CPP [PROJECT CALCULATOR]
// C++ VERSION GNU++14
// macOS 11.7.9
// DUAL-CORE INTEL CORE i5 @ 2.8 GHZ
//
// COPYRIGHT [2023] [MATTHEW T. BUCHANAN] [BOCAN SOFTWARE]
//
// LICENSED UNDER THE APACHE LICENSE, VERSION 2.0 (THE "LICENSE");
// YOU MAY NOT USE THIS FILE EXCEPT IN COMPLIANCE WITH THE LICENSE.
// YOU MAY OBTAIN A COPY OF THE LICENSE AT 
//
// http://www.apacher.org/licenses.LICENSE-2.0
//
// UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING, SOFTWARE
// DISTRIBUTED UNDER THE LICENSE IS DISTRIBUTED ON AN "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
// SEE THE LICENSE FOR THE SPECIFIC LANGUAGE GOVERNING PERMISSIONS AND
// LIMITATIONS UNDER THE LICENSE.

#include <iostream>
#include <string>
#include <cmath>

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::getline;

#include "calculator.hpp"

using bocan::Calculator;

//
//
//
Calculator::Calculator() {
	exit_flag = false;
	cli_arg = false;
}
//
//
//
Calculator::~Calculator() {

}
//
//
//
bool Calculator::Input(int argc, char** argv) {
	expression.clear();
	
	// receives input as a command line argument at runtime. solves the expression and exits.
	if (argc > 1) {
		int x = 0;
		for(int i = 1; i < argc; i++) {			
			for (int j = 0; argv[i][j] != '\0'; j++) {				
				expression.insert(x, 1, argv[i][j]);
				x++;
			}
		}
		cli_arg = true;
		cout << expression << endl;
		argc = 1;
	} 
	// receives input after program start. loops the program for multiple inputs until user exits program.
	else {
		cout << "INPUT EXPRESSION AND PRESS 'ENTER' OR PRESS 'Q'+'ENTER' TO EXIT." << endl;
		getline(std::cin, expression);
	}
	return ValidateInputString();
}
//
//
//
void Calculator::Solve() {

	int result = 0;
	int op1 = 0;
	int op2 = 0;
	int left_index = 0;
	int right_index = 0;

	// resolve multiplication and division operator expressions
	for(int i = 0; i < expression.size(); i++) {
		if(expression.at(i) == 'x' || expression.at(i) == '*' || expression.at(i) == '/') {
			op1 = GetLeftOperand(i-1, &left_index);
			op2 = GetRightOperand(i+1, &right_index);
			result = PerformIntegerOperation(op1, op2, expression.at(i));
			expression.erase(left_index, (right_index - left_index) + 1);
			expression.insert(left_index, std::to_string(result));
			i = left_index;
		}
		result = 0; op1 = 0; op2 = 0; left_index = 0; right_index = 0;
	}

	// resolve addition and subtraction operator expressions
	for(int i = 0; i < expression.size(); i++) {

		if(expression.at(i) == '+' || expression.at(i) == '-') {
			op1 = GetLeftOperand(i-1, &left_index);
			op2 = GetRightOperand(i+1, &right_index);
			result = PerformIntegerOperation(op1, op2, expression.at(i));
			expression.erase(left_index, (right_index - left_index) + 1);
			expression.insert(left_index, std::to_string(result));
			i = left_index;
		}
		result = 0; op1 = 0; op2 = 0; left_index = 0; right_index = 0;
	}

}
//
//
//
void Calculator::Output() {
	cout << expression << endl;
	if(cli_arg) exit_flag = true;
}
//
//
//
bool Calculator::CheckExitFlag() {
	return exit_flag;
}
//
//
//
void Calculator::Exit() { 

}
//
//
//
bool Calculator::ValidateInputString() {
	if(expression.at(0) == 'Q' || expression.at(0) == 'q') {
		exit_flag = true;
		return 1;
	}
	for(int i = 0; i < expression.size(); i++) {
		switch(expression.at(i)) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9': 
				break;	// valid integers
			case '+':
			case '-':
			case 'x':
			case '*':
			case '/': 
				break;	// valid operators
			case ' ':
				expression.erase(i, 1);
				break;	// remove white spaces
			default:
				cerr << "ERROR. INVALID INPUT '" << expression.at(i) << "'. INPUT ONLY VALID INTEGERS (0-9) AND OPERATORS (+, -, x, *, /)." << endl;
				if(cli_arg) exit_flag = true;
				return 1;
		}
	}
	return 0;
}
//
//
//
int Calculator::GetLeftOperand(int index, int* left_index) {

	int op = 0;
	int pos = 0;

	// resolve each digit of the left operand and set the left_index to the leftmost character of the operand
	for(int i = index; i >= 0 && !IsOperator(expression.at(i)); i--) {
		op = op + ( (expression.at(i) - '0') * std::pow(10, pos) );
		pos++;
		*left_index = i;
	}
	return op;
}
//
//
//
int Calculator::GetRightOperand(int index, int* right_index) {
	int op = 0;
	int pos = 0;

	// find the decimal ones place of the right operand
	for(int i = index; i < expression.size() && !IsOperator(expression.at(i)); i++) {
		index = i;
	}
	//set the right_index to the rightmost character of the operand
	*right_index = index;

	// resolve each digit of the right operand 
	for(int i = index; i >= 0 && !IsOperator(expression.at(i)); i--) {
		op = op + ( (expression.at(i) - '0') * std::pow(10, pos) );
		pos++;
	}
	return op;
}
//
//
//
int Calculator::PerformIntegerOperation(int operand1, int operand2, char oper) {
	switch (oper) {
		case 'x': 
		case '*': return operand1 * operand2;
		case '/': return operand1 / operand2;
		case '+': return operand1 + operand2;
		case '-': return operand1 - operand2;
		default: 
			cerr << "ERROR IN CALCULATOR. INVALID OPERATOR." << endl;
			return 0xFF;
	}
}
//
//
//
bool Calculator::IsOperator(char c) {
	switch (c) {
		case 'x':
		case '*':
		case '/':
		case '+':
		case '-':
			return true;
		default:
			return false;
	}
}