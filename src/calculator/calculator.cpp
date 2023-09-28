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

#include "Calculator.hpp"

using bocan::Calculator;

// brief Singleton instance of the Calculator class object.
Calculator Calculator::s_instance; 

///
/// @brief initializes calculator member variables.
/// @brief sets all flags to false and prints initial user instructions.
/// @param
/// @return
/// @todo
///
void Calculator::Initialize() {

	m_flag.exit = false;
	m_flag.cli_arg = false;
	m_flag.solve_err = false;
	m_flag.left_neg = false;
	m_flag.right_neg = false;
	m_flag.modulus = false;
	m_flag.floating = false;

	cout << ">PROJECT CALCULATOR [2023] [MATTHEW BUCHANAN] [BOCAN SOFTWARE]" << endl;
	cout << ">INPUT EXPRESSION AND PRESS 'ENTER' OR PRESS 'Q'+'ENTER' TO EXIT." << endl;
}

///
/// @brief receives the expression to solve as user input as either a command line argument or from the input stream.
/// @param[in] integer is the number of command line arguments passed at runtime.
/// @param[in] char pointer pointer is the vector of command line arguments passed at runtime.
/// @return 0 if user input is valid and 1 if the input conains an error.
/// @todo
///
bool Calculator::Input(int argc, char** argv) {
	m_flag.solve_err = false;
	m_expression.clear();
	
	// receive command line argument. exits program after error or solution.
	if (argc > 1) {
		int x = 0;
		for(int i = 1; i < argc; i++) {			
			for (int j = 0; argv[i][j] != '\0'; j++) {				
				m_expression.insert(x, 1, argv[i][j]);
				x++;
			}
		}
		m_flag.cli_arg = true;
		cout << m_expression << endl;
		argc = 1;
	} 
	// receive input from input stream. loops program until user exits.
	else {
		cout << ">";
		getline(std::cin, m_expression);
		if(m_expression.empty()) { return 1; }
	}
	return ValidateInputString();
}

///
/// @brief solves the expression within the standard string using the order of operations PEMDAS.
/// @param
/// @return
/// @todo
///
void Calculator::Solve() {

	int left_paren_index = 0;

	// resolve parentheses operator expressions from innermost to outermost
	for(int i = 0; i < m_expression.size(); i++) {
		if(m_expression.at(i) == '(') { left_paren_index = i; }
		if(m_expression.at(i) == ')') {

			std::string paren_expr = m_expression.substr( (left_paren_index + 1), (i - left_paren_index) - 1);
			m_expression.erase(left_paren_index, (i - left_paren_index) + 1);

			// resolve parentheses expression exponent operators left to right
			ResolveExpSqrLoop(&paren_expr);			

			// resolve parentheses expression multiplication and division, operators left to right
			ResolveMulDivLoop(&paren_expr);

			// resolve parentheses expression addition and subtraction operators left to right
			ResolveAddSubLoop(&paren_expr);
		
			m_expression.insert(left_paren_index, paren_expr);
			i = -1;
			left_paren_index = 0;	
		}

	}

	// resolve exponent operator expressions left to right
	ResolveExpSqrLoop(&m_expression);

	// resolve multiplication and division operator expressions left to right
	ResolveMulDivLoop(&m_expression);

	// resolve addition and subtraction operator expressions left to right
	ResolveAddSubLoop(&m_expression);

}

///
/// @brief prints the solution or prints an error code.
/// @param
/// @return
/// @todo
///
void Calculator::Output() {
	if (!m_flag.solve_err) {
		cout << ">" << m_expression << endl;
		if(m_flag.modulus) {
			PrintError(INTEGER_DIVIDE_REMAINDER);
			m_flag.modulus = false;
		}
	} else {
		PrintError(SOLVE_ERROR);
	}
	if(m_flag.cli_arg) m_flag.exit = true;
	m_flag.floating = false;
}

///
/// @brief returns the value of the exit flag.
/// @param 
/// @return boolean 0 if the exit flag is false, 1 if the exit flag is true.
/// @todo
///
bool Calculator::CheckExitFlag() {
	return m_flag.exit;
}

///
/// @brief checks for valid integers, operators, and syntax, and prepares the expression for the solver.
/// @brief sets appropriate flags that tell the solver how to solve the expression.
/// @param
/// @return boolean 0 if expression passes all checks. returns 1 and prints error code if expression fails.
/// @todo
///
bool Calculator::ValidateInputString() {

	// check for user exit command
	if(m_expression.at(0) == 'Q' || m_expression.at(0) == 'q') {
		m_flag.exit = true;
		return 1;
	}

	int paren_counter = 0;

	for(int i = 0; i < m_expression.size(); i++) {

		// check for valid characters and syntax
		switch(m_expression.at(i)) {

			case '(':
				
				// check to ensure left paren is followed by an integer or '-' or another '(')
				if(i == (m_expression.size()-1) ||
				  ( i < (m_expression.size()-1) && IsOperator(m_expression.at(i+1)) && 
				  	!m_flag.left_neg && !(m_expression.at(i+1) == '(') ) ) {

					PrintError(INVALID_INPUT_LEFT_PAREN);
					if(m_flag.cli_arg) m_flag.exit = true;
					return 1;
				}

				// check if left paren is preceded by an integer and insert a 'x' operator into string
				if(i > 0 && IsInteger(m_expression.at(i-1))) {
					m_expression.insert(i, 1, 'x');
					i++;
				}
				m_flag.left_neg = false;
				paren_counter++;
				break;

			case ')':

				// check to ensure right paren is preceded by an integer or another ')'
				if((i == 0 || IsOperator(m_expression.at(i-1))) && !(m_expression.at(i-1) == ')')) {
					PrintError(INVALID_INPUT_RIGHT_PAREN);
					if(m_flag.cli_arg) m_flag.exit = true;
					m_flag.left_neg = false;
					return 1;
				}
				
				// check if right paren is followed by an integer and insert a 'x' operator into string
				if(i < (m_expression.size()-1) && IsInteger(m_expression.at(i+1))) {
					m_expression.insert(i+1, 1, 'x');
				  }

				m_flag.left_neg = false;
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
				if(	(i+1) < m_expression.size() &&
					(1+2) < m_expression.size() &&
					m_expression.at(i+1) == '-' &&
					m_expression.at(i+2) == '-'  ) {
					
					PrintError(INVALID_INPUT_DUAL_OPERATORS);
					if(m_flag.cli_arg) m_flag.exit = true;
					return 1;

				// check if '-' operator was passed as last character
				} else if (i == m_expression.size()-1) {
					PrintError(INVALID_INPUT_OPERATOR_LAST);
					if(m_flag.cli_arg) m_flag.exit = true;
					return 1;
				}

				break;

			case '/':

				// check for a divide by zero error
				if(m_expression.at(i+1) == '0') {
					PrintError(DIVIDE_BY_ZERO);
					if(m_flag.cli_arg) m_flag.exit = true;
					return 1;
				}

			case '^':

				// flag is set true here to handle any possible float answers
				m_flag.floating = true;

			case '+':
			case 'x':
			case '*':

				// check if operator was passed as first character
				if(i == 0) {
					PrintError(INVALID_INPUT_OPERATOR_FIRST);
					if(m_flag.cli_arg) m_flag.exit = true;
					return 1;
				
				// check if operator was passed as last character
				} else if (i == (m_expression.size()-1)) {
					PrintError(INVALID_INPUT_OPERATOR_LAST);
					if(m_flag.cli_arg) m_flag.exit = true;
					return 1;
				
				// check if two consecutive operators were passed
		 		} else if(	m_expression.at(i-1) == '-' ||
							m_expression.at(i-1) == '+' ||
							m_expression.at(i-1) == 'x' ||
							m_expression.at(i-1) == '*' ||
							m_expression.at(i-1) == '/' ||
							m_expression.at(i-1) == '^' ) {

					PrintError(INVALID_INPUT_DUAL_OPERATORS);
					if(m_flag.cli_arg) m_flag.exit = true;
					return 1;

				} else {
					break;
				}
			case ' ':

				// remove white spaces
				m_expression.erase(i, 1);
				i--;
				break;	
			
			case '.':

				// check if an operator precedes and follows '.'
				if( i > 0 && 
					i < (m_expression.size() - 1) && 
					IsOperator(m_expression.at(i-1)) && 
					IsOperator(m_expression.at(i+1)) ) {

					PrintError(INVALID_INPUT_RADIX_POINT);
					if(m_flag.cli_arg) m_flag.exit = true;
					return 1;
				}
				m_flag.floating = true;
				break;

			default:
				PrintError(INVALID_INPUT_INVALID_INTEGER);
				if(m_flag.cli_arg) m_flag.exit = true;
				return 1;
		}
	}

	// check to ensure all parenthesis symbols are paired
	if(paren_counter) {
		PrintError(INVALID_INPUT_PARENTHESES_MISMATCH);
		if(m_flag.cli_arg) m_flag.exit = true;
		return 1;
	}
	return 0;
}

///
/// @brief loop to solve for any exponent operator expressions within the string.
/// @brief function will perform operation for either a double or a long based on the floating point flag.
/// @param[in] string reference to the expression the function will loop through to solve.
/// @return none.
/// @todo
///
void Calculator::ResolveExpSqrLoop(std::string* expr) {

	int left_index = 0;
	int right_index = 0;

	if(!m_flag.floating) {
		long set = 0;
		long op1 = 0;
		long op2 = 0;
		long result = 0;

		for(int i = 0; i < expr->size(); i++) {
			if(expr->at(i) == '^') {
				op1 = GetLeftOperand(expr, i-1, &left_index, set);
				op2 = GetRightOperand(expr, i+1, &right_index, set);
				result = PerformMathOperation(op1, op2, expr->at(i));
				expr->erase(left_index, (right_index - left_index) +1);
				expr->insert(left_index, std::to_string(result));
				i = left_index;
			}
			left_index = 0; right_index = 0; set = 0; op1 = 0; op2 = 0; result = 0;
		}

	} else {
		double set = 0;
		double op1 = 0;
		double op2 = 0;
		double result = 0;

		for(int i = 0; i < expr->size(); i++) {
			if(expr->at(i) == '^') {
				op1 = GetLeftOperand(expr, i-1, &left_index, set);
				op2 = GetRightOperand(expr, i+1, &right_index, set);
				result = PerformMathOperation(op1, op2, expr->at(i));
				expr->erase(left_index, (right_index - left_index) +1);
				expr->insert(left_index, std::to_string(result));
				i = left_index;
			}
			left_index = 0; right_index = 0; set = 0; op1 = 0; op2 = 0; result = 0;
		}
	}
}

///
/// @brief loop to solve for any multiplication or division operator expressions within the string.
/// @brief function will perform operation for either a double or a long based on the floating point flag.
/// @param[in] string reference to the expression the function will loop through to solve.
/// @return none.
/// @todo
///
void Calculator::ResolveMulDivLoop(std::string* expr) {

	int left_index = 0;
	int right_index = 0;

	if(!m_flag.floating) {
		long set = 0;
		long op1 = 0;
		long op2 = 0;
		long result = 0;

		for(int i = 0; i < expr->size(); i++) {
			if( expr->at(i) == 'x' || expr->at(i) == '*' || expr->at(i) == '/'){
				op1 = GetLeftOperand(expr, i-1, &left_index, set);
				op2 = GetRightOperand(expr, i+1, &right_index, set);
				result = PerformMathOperation(op1, op2, expr->at(i));
				expr->erase(left_index, (right_index - left_index) + 1);
				expr->insert(left_index, std::to_string(result));
				i = left_index;
			}
			left_index = 0; right_index = 0; set = 0; op1 = 0; op2 = 0; result = 0;
		}
	} else {
		double set = 0;
		double op1 = 0;
		double op2 = 0;
		double result = 0;

		for(int i = 0; i < expr->size(); i++) {
			if( expr->at(i) == 'x' || expr->at(i) == '*' || expr->at(i) == '/'){
				op1 = GetLeftOperand(expr, i-1, &left_index, set);
				op2 = GetRightOperand(expr, i+1, &right_index, set);
				result = PerformMathOperation(op1, op2, expr->at(i));

				expr->erase(left_index, (right_index - left_index) + 1);
				expr->insert(left_index, std::to_string(result));
				i = left_index;
			}
			left_index = 0; right_index = 0; set = 0; op1 = 0; op2 = 0; result = 0;
		}
	}
}

///
/// @brief loop to solve for any addition or subtraction operator expressions within the string.
/// @brief function will perform operation for either a double or a long based on the floating point flag.
/// @param[in] string reference to the expression the function will loop through to solve.
/// @return none.
/// @todo
///
void Calculator::ResolveAddSubLoop(std::string* expr) {

	int left_index = 0;
	int right_index = 0;

	if(!m_flag.floating) {
		long set = 0;
		long op1 = 0;
		long op2 = 0;
		long result = 0;

		for(int i = 0; i < expr->size(); i++) {
			if(expr->at(i) == '+' || expr->at(i) == '-') {
				op1 = GetLeftOperand(expr, i-1, &left_index, set);
				op2 = GetRightOperand(expr, i+1, &right_index, set);
				result = PerformMathOperation(op1, op2, expr->at(i));
				expr->erase(left_index, (right_index - left_index) + 1);
				expr->insert(left_index, std::to_string(result));
				i = left_index;
			}
			left_index = 0; right_index = 0; set = 0; op1 = 0; op2 = 0; result = 0;
		}
	} else {
		double set = 0;
		double op1 = 0;
		double op2 = 0;
		double result = 0;

		for(int i = 0; i < expr->size(); i++) {
			if(expr->at(i) == '+' || expr->at(i) == '-') {
				op1 = GetLeftOperand(expr, i-1, &left_index, set);
				op2 = GetRightOperand(expr, i+1, &right_index, set);
				result = PerformMathOperation(op1, op2, expr->at(i));

				expr->erase(left_index, (right_index - left_index) + 1);
				expr->insert(left_index, std::to_string(result));
				i = left_index;
			}
			left_index = 0; right_index = 0; set = 0; op1 = 0; op2 = 0; result = 0;
		}
	}
}

///
/// @brief retreives the left operand of a math operation.
/// @param[in] string reference to the current expression being solved.
/// @param[in] integer is the positiion of the rightmost character of the left operand.
/// @param[out] integer pointer to the position of the leftmost character of the operand.
/// @param[in] long is a throwaway argument to trigger the correct overloaded function.
/// @return long (8 bytes) representing the left operand.
/// @todo
///
long Calculator::GetLeftOperand(std::string* expr, int index, int* left_index, long l) {

	long op = 0;
	int pos = 0;

	// resolve each digit of the left operand
	// set the left_index to the leftmost character of the operand
	for(int i = index; i >= 0 && !IsOperator(expr->at(i)); i--) {
		op = op + ( (expr->at(i) - '0') * std::pow(10, pos) );
		pos++;
		*left_index = i;
	}

	// check to see if operand is a negative number
	if(m_flag.left_neg && (*left_index-2) > 0 && IsOperator(expr->at(*left_index-2))) {
		m_flag.left_neg = false;
		return op = -op;
	} else if(m_flag.left_neg && (*left_index-1) == 0) {
		*left_index = 0;
		m_flag.left_neg = false;
		return op = -op;
	} else {
		return op;
	}
}

///
/// @brief retreives the left operand of a math operation.
/// @param[in] string reference to the current expression being solved.
/// @param[in] integer is the positiion of the rightmost character of the left operand.
/// @param[out] integer pointer to the position of the leftmost character of the operand.
/// @param[in] double is a throwaway argument to trigger the correct overloaded function.
/// @return double (8 bytes) representing the left operand.
/// @todo
///
double Calculator::GetLeftOperand(std::string* expr, int index, int* left_index, double d) {

	double op = 0;
	int pos = 0;

	bool decimal = false;
	int decimal_pos = 0;

	// find the position of the '.' if there is one
	for(int i = index; i >= 0 && !IsOperator(expr->at(i)); i--) {
		if(expr->at(i) == '.') {
			decimal = true;
			decimal_pos = i;
		}
	} 
	
	if(decimal) { index = decimal_pos - 1; }

	// resolve each digit left of the '.'
	// set the left_index to the leftmost character of the operand
	for(int i = index; i >= 0 && !IsOperator(expr->at(i)); i--) {
		op = op + ( (expr->at(i) - '0') * std::pow(10, pos) );
		pos++;
		*left_index = i;
	}

	if(decimal) {
		index = decimal_pos + 1;
		pos = -1;

		// resolve each digit right of the '.'
		for(int i = index; i < expr->size() && !IsOperator(expr->at(i)); i++) {
			op = op + ( (expr->at(i) - '0') * std::pow(10, pos) );
			pos--;
		}
	}

	// check to see if operand is a negative number
	if(m_flag.left_neg && (*left_index-2) > 0 && IsOperator(expr->at(*left_index-2))) {
		m_flag.left_neg = false;
		return op = -op;
	} else if(m_flag.left_neg && (*left_index-1) == 0) {
		*left_index = 0;
		m_flag.left_neg = false;
		return op = -op;
	} else {
		return op;
	}

}

///
/// @brief retreives the right operand of a math operation.
/// @param[in] string reference to the current expression being solved.
/// @param[in] integer is the position of the leftmost character of the right operand.
/// @param[out] integer pointer to the position of the rightmost character of the operand.
/// @param[in] long is a throwaway argument to trigger the correct overloaded function.
/// @return long (8 bytes) representing the right operand.
/// @todo
///
long Calculator::GetRightOperand(std::string* expr, int index, int* right_index, long l) {

	long op = 0;
	int pos = 0;

	// check to see if operand is a negative number
	if(expr->at(index) == '-') {

		m_flag.right_neg = true;
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

	if(m_flag.right_neg) {
		m_flag.right_neg = false;
		return op = -op;
	} else {
		return op;
	}
}

///
/// @brief retreives the right operand of a math operation.
/// @param[in] string reference to the current expression being solved.
/// @param[in] integer is the position of the leftmost character of the right operand.
/// @param[out] integer pointer to the position of the rightmost character of the operand.
/// @param[in] double is a throwaway argument to trigger the correct overloaded function.
/// @return double (8 bytes) representing the right operand.
/// @todo
///
double Calculator::GetRightOperand(std::string* expr, int index, int* right_index, double d) {

	double op = 0;
	int pos = 0;

	bool decimal = false;
	int decimal_pos = 0;

	// check to see if operand is a negative number
	if(expr->at(index) == '-') {

		m_flag.right_neg = true;
		index++;
	}

	// find the position of the '.' if there is one
	for(int i = index; i < expr->size() && !IsOperator(expr->at(i)); i++) {
		if(expr->at(i) == '.') {
			decimal = true;
			decimal_pos = i;
		}
		index = i; 
	}

	if(decimal)	index = decimal_pos - 1;
	else		*right_index = index;

	// resolve each digit left of the '.'
	for(int i = index; i >= 0 && !IsOperator(expr->at(i)); i--) {
		op = op + ( (expr->at(i) - '0') * std::pow(10, pos) );
		pos++;
	}

	if(decimal) {
		index = decimal_pos + 1;
		pos = -1;

		// resolve each digit right of the '.'
		for(int i = index; i < expr->size() && !IsOperator(expr->at(i)); i++) {
			op = op + ( (expr->at(i) - '0') * std::pow(10, pos) );
			pos--;
			index = i;
		}

		*right_index = index;
	}

	if(m_flag.right_neg) {
		m_flag.right_neg = false;
		return op = -op;
	} else {
		return op;
	}
}

///
/// @brief performs the specified math operation.
/// @param[in] long is the first (left) operand.
/// @param[in] long is the next (right) operand.
/// @param[in] char is the operator specifying which operation to execute.
/// @return solution of the operation as a long (8 bytes).
/// @todo
///
long Calculator::PerformMathOperation(long operand1, long operand2, char oper) {
	switch (oper) {
		case '^': return std::pow(operand1, operand2);
		case 'x': 
		case '*': return operand1 * operand2;
		case '/': 

			// check for a divide by zero error
			if(operand2 == 0) {
				PrintError(DIVIDE_BY_ZERO);
				m_flag.solve_err = true;
				return 0xFF;
			}
			
			// check for a division remainder
			if((operand1 % operand2) > 0) {
				m_flag.modulus = true;
			}
			
			return operand1 / operand2;

		case '+': return operand1 + operand2;
		case '-': return operand1 - operand2;
		default: 
			PrintError(INVALID_INPUT_INVALID_OPERATOR);
			m_flag.solve_err = true;	
			return 0xFF;
	}
}

///
/// @brief performs the specified math operation.
/// @param[in] double is the first (left) operand.
/// @param[in] double is the next (right) operand.
/// @param[in] char is the operator specifying which operation to execute.
/// @return solution of the operation as a double (8 bytes).
/// @todo
///
double Calculator::PerformMathOperation(double operand1, double operand2, char oper) {
	switch (oper) {
		case '^': return std::pow(operand1, operand2);
		case 'x': 
		case '*': return operand1 * operand2;
		case '/': 

			// check for a divide by zero error
			if(operand2 == 0) {
				PrintError(DIVIDE_BY_ZERO);
				m_flag.solve_err = true;
				return 0xFF;
			}

			return operand1 / operand2;

		case '+': return operand1 + operand2;
		case '-': return operand1 - operand2;
		default: 
			PrintError(INVALID_INPUT_INVALID_OPERATOR);
			m_flag.solve_err = true;	
			return 0xFF;
	}
}

///
/// @brief checks if character is a valid operator within the program.
/// @brief sets the flag for a negative left operand if the character is a '-'.
/// @param[in] char is the symbol being checked.
/// @return boolean true if argument is a valid operator. 
/// @todo
///
bool Calculator::IsOperator(char c) {
	switch (c) {
		case '-':
			m_flag.left_neg = true;
		case 'x':
		case '*':
		case '/':
		case '%':
		case '+':
		case '(':
		case ')':
		case '^':
			return true;
		default:
			return false;
	}
}

///
/// @brief checks if character is a valid integer within the program.
/// @param[in] char is the symbol being checked.
/// @return boolean true if argument is a valid integer.
/// @todo
///
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

///
/// @brief prints the error code and description for the specified error.
/// @param[in] enumerator corresponding to the error_code enum.
/// @return none.
/// @todo
///
void Calculator::PrintError(int error_code) {
	switch(error_code) {
		case(DIVIDE_BY_ZERO):
			cerr << ">ERROR " << error_code << ". DIVIDE BY ZERO." << endl;
			break;
		case(SOLVE_ERROR):
			cerr << ">ERROR " << error_code << ". UNABLE TO COMPUTE SOLUTION." << endl;
			break;
		case(INTEGER_DIVIDE_REMAINDER):
			cerr << ">WARNING. DIVISION OPERATION RESULTED IN A NONINTEGER SOLUTION. SOLUTION MAY NOT BE CORRECT." << endl;
			break;
		case(INVALID_INPUT_INVALID_OPERATOR):
			cerr << ">ERROR " << error_code << ". INVALID OPERATOR." << endl;
			break;
		case(INVALID_INPUT_OPERATOR_FIRST):
			cerr << ">ERROR " << error_code << ". INVALID INPUT. BEGIN THE EXPRESSION WITH A VALID INTEGER (0-9) OR NEGATIVE OPERATOR (-) IF NEGATIVE NUMBER." << endl;
			break;
		case(INVALID_INPUT_OPERATOR_LAST):
			cerr << ">ERROR " << error_code << ". INVALID INPUT. THE EXPRESSION MUST END WITH A VALID INTEGER (0-9)." << endl;
			break;
		case(INVALID_INPUT_DUAL_OPERATORS):
			cerr << ">ERROR " << error_code << ". INVALID INPUT. INPUT ONLY ONE VALID OPERATOR BETWEEN TWO INTEGERS." << endl;
			break;
		case(INVALID_INPUT_INVALID_INTEGER):
			cerr << ">ERROR " << error_code << ". INVALID INPUT. INPUT ONLY VALID INTEGERS (0-9) AND OPERATORS (+, -, x, *, /)." << endl;
			break;
		case(INVALID_INPUT_LEFT_PAREN):
			cerr << ">ERROR " << error_code << ". INVALID INPUT. LEFT PAREN '(' MUST BE FOLLOWED BY AN INTEGER OR '-'." << endl;
			break;
		case(INVALID_INPUT_RIGHT_PAREN):
			cerr << ">ERROR " << error_code << ". INVALID INPUT. RIGHT PAREN ')' MUST BE PRECEDED BY AN INTEGER." << endl;
			break;
		case(INVALID_INPUT_PARENTHESES_MISMATCH):
			cerr << ">ERROR " << error_code << ". INVALID INPUT. PARENTHESIS SYMBOLS '(' AND ')' MUST MATCH." << endl;
			break;
		case(INVALID_INPUT_RADIX_POINT):
		 	cerr << ">ERROR " << error_code << ". INVALID INPUT. RADIX POINT '.' MUST PRECEDE OR FOLLOW A NUMBER." << endl;
	}
}
