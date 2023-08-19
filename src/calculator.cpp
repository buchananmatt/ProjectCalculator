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
// constructor
//
Calculator::Calculator() {

	flag.exit = false;
	flag.cli_arg = false;
	flag.solve_err = false;
	flag.left_neg = false;
	flag.right_neg = false;
	flag.modulus = false;
}

//
// destructor. not used at the moment as all data is on the stack.
//
Calculator::~Calculator() {

}

// 
// receive the expression from the user and input into a standard string.
//
bool Calculator::Input(int argc, char** argv) {
	flag.solve_err = false;
	expression.clear();
	
	// receives input as a command line argument at runtime. 
	// if CLI argument is passed, program will solve the expression
	// and immediately exit.
	if (argc > 1) {
		int x = 0;
		for(int i = 1; i < argc; i++) {			
			for (int j = 0; argv[i][j] != '\0'; j++) {				
				expression.insert(x, 1, argv[i][j]);
				x++;
			}
		}
		flag.cli_arg = true;
		cout << expression << endl;
		argc = 1;
	} 
	// receives input after program start. 
	// loops the program for multiple inputs until user exits program.
	else {
		cout << "INPUT EXPRESSION AND PRESS 'ENTER' OR PRESS 'Q'+'ENTER' TO EXIT." << endl;
		getline(std::cin, expression);
	}
	return ValidateInputString();
}

// 
// solves the expression within the string based on PEMDAS.
//
void Calculator::Solve() {

	int result = 0;
	int op1 = 0;
	int op2 = 0;
	int left_index = 0;
	int right_index = 0;

	// resolve multiplication and division operator expressions left to right
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

	// resolve addition and subtraction operator expressions left to right
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
// prints the solution from the string. if an error occured during the solution
// the error code is printed instead of the solution.
//
void Calculator::Output() {
	if (!flag.solve_err) {
		cout << expression << endl;
		if(flag.modulus) {
			PrintError(INTEGER_DIVIDE_REMAINDER);
			flag.modulus = false;
		}
	} else {
		PrintError(SOLVE_ERROR);
	}
	if(flag.cli_arg) flag.exit = true;
}

// 
// self explanatory
//
bool Calculator::CheckExitFlag() {
	return flag.exit;
}

// 
// not sure this is going to be needed since all data is instantiated on the stack.
//
void Calculator::Exit() { }

// 
// checks the user input for valid integers and operators and ensures proper syntax. 
// returns 0 if input passes. if input fails, prints the error code and returns 1.
//
bool Calculator::ValidateInputString() {

	// check for user exit command
	if(expression.at(0) == 'Q' || expression.at(0) == 'q') {
		flag.exit = true;
		return 1;
	}

	for(int i = 0; i < expression.size(); i++) {

		// check for valid characters
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
				break;
			case '-':

				// check for three '-' in a row. 
				// two '-' will be resolved as an add operator. 
				// more than two '-' is an error.
				if(	(i+1) < expression.size() &&
					(1+2) < expression.size() &&
					expression.at(i+1) == '-' &&
					expression.at(i+2) == '-'  ) {
					
					PrintError(INVALID_INPUT_DUAL_OPERATORS);
					if(flag.cli_arg) flag.exit = true;
					return 1;

				// check if '-' operator was passed as last character
				} else if (i == expression.size()-1) {
					PrintError(INVALID_INPUT_OPERATOR_LAST);
					if(flag.cli_arg) flag.exit = true;
					return 1;
				}

				break;
			case '/':
				// check for a divide by zero error
				if(expression.at(i+1) == '0') {
					PrintError(DIVIDE_BY_ZERO);
					if(flag.cli_arg) flag.exit = true;
					return 1;
				}
			case '+':
			case 'x':
			case '*':

				// check if operator was passed as first character
				if(i == 0) {
					PrintError(INVALID_INPUT_OPERATOR_FIRST);
					if(flag.cli_arg) flag.exit = true;
					return 1;
				
				// check if operator was passed as last character
				} else if (i == (expression.size()-1)) {
					PrintError(INVALID_INPUT_OPERATOR_LAST);
					if(flag.cli_arg) flag.exit = true;
					return 1;
				
				// check if two consecutive operators were passed
		 		} else if(	expression.at(i-1) == '-' ||
							expression.at(i-1) == '+' ||
							expression.at(i-1) == 'x' ||
							expression.at(i-1) == '*' ||
							expression.at(i-1) == '/'  ) {

					PrintError(INVALID_INPUT_DUAL_OPERATORS);
					if(flag.cli_arg) flag.exit = true;
					return 1;

				} else {
					break;
				}
			case ' ':

				// remove white spaces
				expression.erase(i, 1);
				break;	
	
			default:
				PrintError(INVALID_INPUT_INVALID_INTEGER);
				if(flag.cli_arg) flag.exit = true;
				return 1;
		}
	}
	return 0;
}

// 
// retrieves the left operand of an operation from the string.
// returns operand as an integer.
//
int Calculator::GetLeftOperand(int index, int* left_index) {

	int op = 0;
	int pos = 0;

	// resolve each digit of the left operand and set the left_index 
	// to the leftmost character of the operand
	for(int i = index; i >= 0 && !IsOperator(expression.at(i)); i--) {
		op = op + ( (expression.at(i) - '0') * std::pow(10, pos) );
		pos++;
		*left_index = i;
	}

	// check to see if operand is a negative number
	if(flag.left_neg && (*left_index-2) > 0 && IsOperator(expression.at(*left_index-2))) {
		flag.left_neg = false;
		return op = -op;
	} else if(flag.left_neg && (*left_index-1) == 0) {
		*left_index = 0;
		flag.left_neg = false;
		return op = -op;
	} else {
		return op;
	}
}

// 
// retrieves the right operand of an operation from the string.
// returns operand as an integer.
//
int Calculator::GetRightOperand(int index, int* right_index) {

	int op = 0;
	int pos = 0;

	// check to see if operand is a negative number
	if(expression.at(index) == '-') {

		flag.right_neg = true;
		index++;
	}

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

	if(flag.right_neg) {
		flag.right_neg = false;
		return op = -op;
	} else {
		return op;
	}
}

// 
// performs the specified integer operation based on the character operator.
//
int Calculator::PerformIntegerOperation(int operand1, int operand2, char oper) {
	switch (oper) {
		case 'x': 
		case '*': return operand1 * operand2;
		case '/': 

			// check for a divide by zero error
			if(operand2 == 0) {
				PrintError(DIVIDE_BY_ZERO);
				flag.solve_err = true;
				return 0xFF;
			}

			// check for a division remainder
			if((operand1 % operand2) > 0) {
				flag.modulus = true;
			}
			return operand1 / operand2;
		case '+': return operand1 + operand2;
		case '-': return operand1 - operand2;
		default: 
			PrintError(INVALID_INPUT_INVALID_OPERATOR);
			flag.solve_err = true;	
			return 0xFF;
	}
}

//
// returns true if the argument character is a valid operator
//
bool Calculator::IsOperator(char c) {
	switch (c) {
		case '-':
			flag.left_neg = true;
		case 'x':
		case '*':
		case '/':
		case '+':
			return true;
		default:
			return false;
	}
}

//
// prints the error stream based on the argument error code
//
void Calculator::PrintError(int error_code) {
	switch(error_code) {
		case(DIVIDE_BY_ZERO):
			cerr << "ERROR. DIVIDE BY ZERO." << endl;
			break;
		case(SOLVE_ERROR):
			cerr << "ERROR DURING SOLVING. UNABLE TO COMPUTE SOLUTION." << endl;
			break;
		case(INTEGER_DIVIDE_REMAINDER):
			cerr << "WARNING: DIVISION OPERATION RESULTED IN A NONINTEGER SOLUTION. SOLUTION IS ROUNDED DOWN TO THE NEXT WHOLE NUMBER." << endl;
		case(INVALID_INPUT_INVALID_OPERATOR):
			cerr << "ERROR IN CALCULATOR. INVALID OPERATOR." << endl;
			break;
		case(INVALID_INPUT_OPERATOR_FIRST):
			cerr << "ERROR. INVALID INPUT. BEGIN THE EXPRESSION WITH A VALID INTEGER (0-9) OR NEGATIVE OPERATOR (-) IF NEGATIVE NUMBER." << endl;
			break;
		case(INVALID_INPUT_OPERATOR_LAST):
			cerr << "ERROR. INVALID INPUT. THE EXPRESSION MUST END WITH A VALID INTEGER (0-9)." << endl;
			break;
		case(INVALID_INPUT_DUAL_OPERATORS):
			cerr << "ERROR. INVALID INPUT. INPUT ONLY ONE VALID OPERATOR BETWEEN TWO INTEGERS." << endl;
			break;
		case(INVALID_INPUT_INVALID_INTEGER):
			cerr << "ERROR. INVALID INPUT. INPUT ONLY VALID INTEGERS (0-9) AND OPERATORS (+, -, x, *, /)." << endl;
			break;
	}

}