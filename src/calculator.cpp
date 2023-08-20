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

// brief Class Constructor.
// brief Sets all flags to false.
// param None.
Calculator::Calculator() {

	flag.exit = false;
	flag.cli_arg = false;
	flag.solve_err = false;
	flag.left_neg = false;
	flag.right_neg = false;
	flag.modulus = false;
}

// brief Class Destructor.
// brief Unused function as all data is allocated on the stack.
// param None.

Calculator::~Calculator() { }

// brief Receives the expression to solve as user input as either a command line argument or from the input stream.
// brief Returns 0 if user input is valid and 1 if the input contains an error.
// param Integer is the number of command line arguments passed at runtime.
// param Character Pointer Pointer is the vector of command line arguments passed at runtime.
bool Calculator::Input(int argc, char** argv) {
	flag.solve_err = false;
	expression.clear();
	
	// receive command line argument. exits program after error or solution.
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
	// receive input from input stream. loops program until user exits.
	else {
		cout << "INPUT EXPRESSION AND PRESS 'ENTER' OR PRESS 'Q'+'ENTER' TO EXIT." << endl;
		getline(std::cin, expression);
		if(expression.empty()) { return 1; }
	}
	return ValidateInputString();
}

// brief Solves the expression within the standard string using the order of operations PEMDAS.
// param None.
//
void Calculator::Solve() {

	int result = 0;
	int op1 = 0;
	int op2 = 0;
	int left_index = 0;
	int right_index = 0;

	int left_paren_index = 0;

	// resolve parentheses operator expressions from innermost to outermost
	for(int i = 0; i < expression.size(); i++) {
		if(expression.at(i) == '(') { left_paren_index = i; }
		if(expression.at(i) == ')') {

			std::string paren_expr = expression.substr( (left_paren_index + 1), (i - left_paren_index) - 1);
			expression.erase(left_paren_index, (i - left_paren_index) + 1);

			// resolve parentheses expression multiplication and division operators left to right
			for(int i = 0; i < paren_expr.size(); i++) {
				if(paren_expr.at(i) == 'x' || paren_expr.at(i) == '*' || paren_expr.at(i) == '/') {
					op1 = GetLeftOperand(&paren_expr, i-1, &left_index);
					op2 = GetRightOperand(&paren_expr, i+1, &right_index);
					result = PerformIntegerOperation(op1, op2, paren_expr.at(i));
					paren_expr.erase(left_index, (right_index - left_index) + 1);
					paren_expr.insert(left_index, std::to_string(result));
					i = left_index;
				}
				result = 0; op1 = 0; op2 = 0; left_index = 0; right_index = 0;
			}

			// resolve parentheses expression addition and subtraction operators left to right
			for(int i = 0; i < paren_expr.size(); i++) {
				if(paren_expr.at(i) == '+' || paren_expr.at(i) == '-') {
					op1 = GetLeftOperand(&paren_expr, i-1, &left_index);
					op2 = GetRightOperand(&paren_expr, i+1, &right_index);
					result = PerformIntegerOperation(op1, op2, paren_expr.at(i));
					paren_expr.erase(left_index, (right_index - left_index) + 1);
					paren_expr.insert(left_index, std::to_string(result));
					i = left_index;
				}
				result = 0; op1 = 0; op2 = 0; left_index = 0; right_index = 0;
			}
			expression.insert(left_paren_index, paren_expr);
			i = -1;
			left_paren_index = 0;	
		}

	}

	// resolve multiplication and division operator expressions left to right
	for(int i = 0; i < expression.size(); i++) {
		if(expression.at(i) == 'x' || expression.at(i) == '*' || expression.at(i) == '/') {
			op1 = GetLeftOperand(&expression, i-1, &left_index);
			op2 = GetRightOperand(&expression, i+1, &right_index);
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
			op1 = GetLeftOperand(&expression, i-1, &left_index);
			op2 = GetRightOperand(&expression, i+1, &right_index);
			result = PerformIntegerOperation(op1, op2, expression.at(i));
			expression.erase(left_index, (right_index - left_index) + 1);
			expression.insert(left_index, std::to_string(result));
			i = left_index;
		}
		result = 0; op1 = 0; op2 = 0; left_index = 0; right_index = 0;
	}
}

// brief Prints the solution or prints an error code.
// param None.
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

// brief Returns the value of the exit flag.
// param None.
//
bool Calculator::CheckExitFlag() {
	return flag.exit;
}

// brief Checks for valid integers, operators, and syntax, and prepares the expression for the solver.
// brief Returns 0 if expression passes all checks. Returns 1 and prints error code of expression fails.
// param None.
bool Calculator::ValidateInputString() {

	// check for user exit command
	if(expression.at(0) == 'Q' || expression.at(0) == 'q') {
		flag.exit = true;
		return 1;
	}

	int paren_counter = 0;

	for(int i = 0; i < expression.size(); i++) {

		// check for valid characters and syntax
		switch(expression.at(i)) {

			case '(':
				
				// check to ensure left paren is followed by an integer or '-' or another '(')
				if(i == (expression.size()-1) ||
				  ( i < (expression.size()-1) && IsOperator(expression.at(i+1)) && 
				  	!flag.left_neg && !(expression.at(i+1) == '(') ) ) {

					PrintError(INVALID_INPUT_LEFT_PAREN);
					if(flag.cli_arg) flag.exit = true;
					return 1;
				}

				// check if left paren is preceded by an integer and insert a 'x' operator into string
				if(i > 0 && IsInteger(expression.at(i-1))) {
					expression.insert(i, 1, 'x');
					i++;
				}
				flag.left_neg = false;
				paren_counter++;
				break;

			case ')':

				// check to ensure right paren is preceded by an integer or another ')'
				if((i == 0 || IsOperator(expression.at(i-1))) && !(expression.at(i-1) == ')')) {
					PrintError(INVALID_INPUT_RIGHT_PAREN);
					if(flag.cli_arg) flag.exit = true;
					flag.left_neg = false;
					return 1;
				}
				
				// check if right paren is followed by an integer and insert a 'x' operator into string
				if(i < (expression.size()-1) && IsInteger(expression.at(i+1))) {
					expression.insert(i+1, 1, 'x');
				  }

				flag.left_neg = false;
				paren_counter--;
				break;

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
				i--;
				break;	
	
			default:
				PrintError(INVALID_INPUT_INVALID_INTEGER);
				if(flag.cli_arg) flag.exit = true;
				return 1;
		}
	}

	// check to ensure all parenthesis symbols are paired
	if(paren_counter) {
		PrintError(INVALID_INPUT_PARENTHESES_MISMATCH);
		if(flag.cli_arg) flag.exit = true;
		return 1;
	}
	return 0;
}

// brief Retreives the left operand of an operation and returns it as an integer.
// param String Pointer to the current expression being solved.
// param Integer is the positiion of the rightmost character of the left operand.
// param Integer Pointer to the position of the leftmost character of the operand.
int Calculator::GetLeftOperand(std::string* expr, int index, int* left_index) {

	int op = 0;
	int pos = 0;

	// resolve each digit of the left operand
	// set the left_index to the leftmost character of the operand
	for(int i = index; i >= 0 && !IsOperator(expr->at(i)); i--) {
		op = op + ( (expr->at(i) - '0') * std::pow(10, pos) );
		pos++;
		*left_index = i;
	}

	// check to see if operand is a negative number
	if(flag.left_neg && (*left_index-2) > 0 && IsOperator(expr->at(*left_index-2))) {
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

// brief Retreives th right operand of an operation and returns it as an integer.
// param String Pointer to the current expression being solved.
// param Integer is the position of the leftmost character of the right operand.
// param Integer Pointers to the position of the rightmost character of the operand.
int Calculator::GetRightOperand(std::string* expr, int index, int* right_index) {

	int op = 0;
	int pos = 0;

	// check to see if operand is a negative number
	if(expr->at(index) == '-') {

		flag.right_neg = true;
		index++;
	}

	// find the rightmost position (decimal 'ones' place) of the right operand
	for(int i = index; i < expr->size() && !IsOperator(expr->at(i)); i++) {
		index = i;
	}
	// set the right_index to the rightmost character of the operand
	*right_index = index;

	// resolve each digit of the right operand 
	for(int i = index; i >= 0 && !IsOperator(expr->at(i)); i--) {
		op = op + ( (expr->at(i) - '0') * std::pow(10, pos) );
		pos++;
	}

	if(flag.right_neg) {
		flag.right_neg = false;
		return op = -op;
	} else {
		return op;
	}
}

// brief Performs the specified integer operation and returns the solution as an integer.
// param Integer is the first (left) operand.
// param Integer is the next (right) operand.
// param Character is the operator to use in the operation.
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

// brief Returns true if the argument is a valid operator.
// brief Sets the flag for a negative left operand if the character is a '-'.
// param Character is the symbol being checked.
bool Calculator::IsOperator(char c) {
	switch (c) {
		case '-':
			flag.left_neg = true;
		case 'x':
		case '*':
		case '/':
		case '+':
		case '(':
		case ')':
			return true;
		default:
			return false;
	}
}

// brief Returns true if the argument is a valid integer.
// param Character is the symbol being checked.
//
bool Calculator::IsInteger(char c) {
	switch(c) {
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
			return true;
		default:
			return false;
	}
}

// brief Prints the error code and description for the specified error.
// param Integer corresponds to the errors enumerator.
//
void Calculator::PrintError(int error_code) {
	switch(error_code) {
		case(DIVIDE_BY_ZERO):
			cerr << "ERROR " << error_code << ". DIVIDE BY ZERO." << endl;
			break;
		case(SOLVE_ERROR):
			cerr << "ERROR " << error_code << ". UNABLE TO COMPUTE SOLUTION." << endl;
			break;
		case(INTEGER_DIVIDE_REMAINDER):
			cerr << "WARNING. DIVISION OPERATION RESULTED IN A NONINTEGER SOLUTION. SOLUTION IS ROUNDED DOWN TO THE NEXT WHOLE NUMBER." << endl;
			break;
		case(INVALID_INPUT_INVALID_OPERATOR):
			cerr << "ERROR " << error_code << ". INVALID OPERATOR." << endl;
			break;
		case(INVALID_INPUT_OPERATOR_FIRST):
			cerr << "ERROR " << error_code << ". INVALID INPUT. BEGIN THE EXPRESSION WITH A VALID INTEGER (0-9) OR NEGATIVE OPERATOR (-) IF NEGATIVE NUMBER." << endl;
			break;
		case(INVALID_INPUT_OPERATOR_LAST):
			cerr << "ERROR " << error_code << ". INVALID INPUT. THE EXPRESSION MUST END WITH A VALID INTEGER (0-9)." << endl;
			break;
		case(INVALID_INPUT_DUAL_OPERATORS):
			cerr << "ERROR " << error_code << ". INVALID INPUT. INPUT ONLY ONE VALID OPERATOR BETWEEN TWO INTEGERS." << endl;
			break;
		case(INVALID_INPUT_INVALID_INTEGER):
			cerr << "ERROR " << error_code << ". INVALID INPUT. INPUT ONLY VALID INTEGERS (0-9) AND OPERATORS (+, -, x, *, /)." << endl;
			break;
		case(INVALID_INPUT_LEFT_PAREN):
			cerr << "ERROR " << error_code << ". INVALID INPUT. LEFT PAREN '(' MUST BE FOLLOWED BY AN INTEGER OR '-'." << endl;
			break;
		case(INVALID_INPUT_RIGHT_PAREN):
			cerr << "ERROR " << error_code << ". INVALID INPUT. RIGHT PAREN ')' MUST BE PRECEDED BY AN INTEGER." << endl;
			break;
		case(INVALID_INPUT_PARENTHESES_MISMATCH):
			cerr << "ERROR " << error_code << ". INVALID INPUT. PARENTHESIS SYMBOLS '(' AND ')' MUST MATCH." << endl;
			break;
	}
}