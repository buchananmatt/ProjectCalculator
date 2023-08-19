// 
// CALCULATOR.HPP [PROJECT CALCULATOR]
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

#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP


class string;

namespace bocan {

class Calculator {

public: 
	Calculator();
	~Calculator();

	bool 	Input(int, char**);
	void 	Solve();
	void 	Output();
	bool 	CheckExitFlag();
	void	Exit();

private: // class private attributes

	std::string	expression;

	struct flags {
		bool exit;
		bool cli_arg;
		bool solve_err;
		bool left_neg;
		bool right_neg;
		bool modulus;
	} flag;

	enum errors {
		DIVIDE_BY_ZERO,
		SOLVE_ERROR,
		INTEGER_DIVIDE_REMAINDER,
		INVALID_INPUT_INVALID_OPERATOR,
		INVALID_INPUT_THREE_MINUS,
		INVALID_INPUT_MINUS_LAST,
		INVALID_INPUT_OPERATOR_FIRST,
		INVALID_INPUT_OPERATOR_LAST,
		INVALID_INPUT_DUAL_OPERATORS,
		INVALID_INPUT_INVALID_INTEGER
	} error_code;

private: // class private methods

	bool	ValidateInputString();
	int		GetLeftOperand(int, int*);
	int		GetRightOperand(int, int*);
	int 	PerformIntegerOperation(int, int, char);
	bool 	IsOperator(char);
	void	PrintError(int);

// future implementation to handle floating point numbers
//	float	GetLeftFloatOperand(int, int*);
//	float	GetRightFloatOperand(int, int*);
//	float	PerformFloatOperation(float, float, char);
};

} // NAMESPACE BOCAN	

#endif	//CALCULATOR_HPP