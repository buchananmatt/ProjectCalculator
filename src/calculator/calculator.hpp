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

namespace calc {

class Calculator {

public: 
	static Calculator& Get() { return s_instance; }

	void	Initialize();
	bool 	Input(int, char**);
	void 	Solve();
	void 	Output();
	bool 	CheckExitFlag();

private: 
	static Calculator s_instance;

	std::string	m_expression;

	struct flags {
		bool 	exit;
		bool 	cli_arg;
		bool 	solve_err;
		bool 	left_neg;
		bool 	right_neg;
		bool 	modulus;
		bool	floating;
	} m_flag;

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
		INVALID_INPUT_INVALID_INTEGER,
		INVALID_INPUT_LEFT_PAREN,
		INVALID_INPUT_RIGHT_PAREN,
		INVALID_INPUT_PARENTHESES_MISMATCH,
		INVALID_INPUT_RADIX_POINT
	} m_error_code;

private:

	Calculator() {}
	Calculator(const Calculator&) = delete;
	~Calculator() {}

	bool	ValidateInputString();

	void 	ResolveExpSqrLoop(std::string*);
	void	ResolveMulDivLoop(std::string*);
	void	ResolveAddSubLoop(std::string*);

	long	GetLeftOperand(std::string*, int, int*, long);
	double	GetLeftOperand(std::string*, int, int*, double);

	long	GetRightOperand(std::string*, int, int*, long);
	double  GetRightOperand(std::string*, int, int*, double);

	long 	PerformMathOperation(long, long, char);
	double  PerformMathOperation(double, double, char);

	bool 	IsOperator(char);
	bool	IsInteger(char);

	void	PrintError(int);

};

} // NAMESPACE CALC

} // NAMESPACE BOCAN	

#endif	// CALCULATOR_HPP