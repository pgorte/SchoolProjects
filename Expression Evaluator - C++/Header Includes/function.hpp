#pragma once

/* ----- Modified by Peter Gortemaker for Project 2 ----- */

#include <ee/operation.hpp>
#include <ee/integer.hpp>
#include <vector>
#include <stack>

// Function token base class
class Function : public Operation {};

// One argument function token base class
class OneArgFunction : public Function {
public:
	// Performs one-argument function using lookup table
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

// One argument function tokens
class Abs : public OneArgFunction {};
class Arccos : public OneArgFunction {};
class Arcsin : public OneArgFunction {};
class Arctan : public OneArgFunction {};
class Ceil : public OneArgFunction {};
class Cos : public OneArgFunction {};
class Exp : public OneArgFunction {};
class Floor : public OneArgFunction {};
class Lb : public OneArgFunction {};
class Ln : public OneArgFunction {};
class Log : public OneArgFunction {};
class Sin : public OneArgFunction {};
class Sqrt : public OneArgFunction {};
class Tan : public OneArgFunction {};

// previous result token
// Special case: handled in RPNEvaluator due to dependency on results storage
class Result : public OneArgFunction {
public:
	// Override to provide handling for result
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};



// Two argument function token base class
class TwoArgFunction : public Function {
public:
	// Performs two-argument function using lookup table
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

class Arctan2 : public TwoArgFunction {};
class Max : public TwoArgFunction {};
class Min : public TwoArgFunction {};
class Pow : public TwoArgFunction {};


// Three argument function token base class
class ThreeArgFunction : public Function {
public:
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};
