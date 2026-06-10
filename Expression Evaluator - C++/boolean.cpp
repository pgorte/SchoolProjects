/* ----- Modified by Peter Gortemaker for Project 2 ----- */

#include <ee/boolean.hpp>
#include <ee/function.hpp>
#include <algorithm>


/*! Print a boolean token's value. */
Boolean::string_type Boolean::str() const {
	return value_ ? string_type("True") : string_type("False");
}


// Macros for repetitive operations

// Boolean Binary Macro
#define BOOLEAN_BINARY_OP(OP_NAME, EXPR) \
Operand::pointer_type Boolean::perform_##OP_NAME(Operand::pointer_type rhs) { \
	if (is<Boolean>(rhs)) { \
		return convert<Operand>(make<Boolean>(EXPR)); \
	} \
	throw std::runtime_error("Error: " #OP_NAME " requires boolean operands"); \
}

// Boolean Relational Macro
#define BOOLEAN_RELATIONAL_OP(OP_NAME, OP_SYMBOL) \
Operand::pointer_type Boolean::perform_##OP_NAME(Operand::pointer_type rhs) { \
	if (is<Boolean>(rhs)) { \
		return convert<Operand>(make<Boolean>(value_ OP_SYMBOL value_of<Boolean>(rhs))); \
	} \
	throw std::runtime_error("Error: invalid operand type for " #OP_NAME); \
}

// Boolean Operator Macros
BOOLEAN_BINARY_OP(and, value_&& value_of<Boolean>(rhs))
BOOLEAN_BINARY_OP(or , value_ || value_of<Boolean>(rhs))
BOOLEAN_BINARY_OP(xor, value_ != value_of<Boolean>(rhs))
BOOLEAN_BINARY_OP(nand, !(value_&& value_of<Boolean>(rhs)))
BOOLEAN_BINARY_OP(nor, !(value_ || value_of<Boolean>(rhs)))
BOOLEAN_BINARY_OP(xnor, value_ == value_of<Boolean>(rhs))

// Relational Operator Macros
BOOLEAN_RELATIONAL_OP(equality, == )
BOOLEAN_RELATIONAL_OP(inequality, != )
BOOLEAN_RELATIONAL_OP(greater, > )
BOOLEAN_RELATIONAL_OP(greater_equal, >= )
BOOLEAN_RELATIONAL_OP(less, < )
BOOLEAN_RELATIONAL_OP(less_equal, <= )

// Unary Operators
Operand::pointer_type Boolean::perform_not() {
	return convert<Operand>(make<Boolean>(!value_));
}