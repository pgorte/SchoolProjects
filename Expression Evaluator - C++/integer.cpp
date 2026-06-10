/* ----- Modified by Peter Gortemaker for Project 2 ----- */

#include <ee/integer.hpp>
#include <ee/function.hpp>
#include <ee/operator.hpp>
#include <ee/boolean.hpp>
#include <ee/real.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <array>
using namespace std;



[[nodiscard]] Integer::string_type Integer::str() const {
	return value().str();
}

// Helper function to convert to Real for mixed arithmetic
static Real::value_type toReal(Integer::value_type const& intVal) {
	return Real::value_type(intVal.str());
}


// Macros 
// Binary Macro
#define INTEGER_BINARY_OP(OP_NAME, OP_SYMBOL) \
Operand::pointer_type Integer::perform_##OP_NAME(Operand::pointer_type rhs) { \
	if (is<Integer>(rhs)) { \
		return convert<Operand>(make<Integer>(value_ OP_SYMBOL value_of<Integer>(rhs))); \
	} \
	else if (is<Real>(rhs)) { \
		return convert<Operand>(make<Real>(toReal(value_) OP_SYMBOL value_of<Real>(rhs))); \
	} \
	throw std::runtime_error("Error: invalid operand type for " #OP_NAME); \
}

// Relational Macro
#define INTEGER_RELATIONAL_OP(OP_NAME, OP_SYMBOL) \
Operand::pointer_type Integer::perform_##OP_NAME(Operand::pointer_type rhs) { \
	if (is<Integer>(rhs)) { \
		return convert<Operand>(make<Boolean>(value_ OP_SYMBOL value_of<Integer>(rhs))); \
	} \
	else if (is<Real>(rhs)) { \
		return convert<Operand>(make<Boolean>(toReal(value_) OP_SYMBOL value_of<Real>(rhs))); \
	} \
	throw std::runtime_error("Error: invalid operand type for " #OP_NAME); \
}


// Binary Operators
INTEGER_BINARY_OP(addition, +)
INTEGER_BINARY_OP(subtraction, -)
INTEGER_BINARY_OP(multiplication, *)


// Binary Operators (special cases)
Operand::pointer_type Integer::perform_division(Operand::pointer_type rhs) {
	if (is<Integer>(rhs)) {
		auto rhsVal = value_of<Integer>(rhs);
		if (rhsVal == 0) {
			throw std::runtime_error("Error: division by zero");
		}
		// Integer division
		return convert<Operand>(make<Integer>(value_ / rhsVal));
	}
	else if (is<Real>(rhs)) {
		auto rhsVal = value_of<Real>(rhs);
		if (rhsVal == 0) {
			throw std::runtime_error("Error: division by zero");
		}
		// Real division
		return convert<Operand>(make<Real>(toReal(value_) / rhsVal));
	}
	throw std::runtime_error("Error: invalid operand type for division");
}

Operand::pointer_type Integer::perform_modulus(Operand::pointer_type rhs) {
	if (is<Integer>(rhs)) {
		auto rhsVal = value_of<Integer>(rhs);
		if (rhsVal == 0) {
			throw std::runtime_error("Error: modulus by zero");
		}
		return convert<Operand>(make<Integer>(value_ % rhsVal));
	}
	throw std::runtime_error("Error: modulus requires integer operands");
}

Operand::pointer_type Integer::perform_power(Operand::pointer_type rhs) {
	if (is<Integer>(rhs)) {
		auto rhsVal = value_of<Integer>(rhs);

		// Negative exponent always returns Real
		if (rhsVal < 0) {
			auto leftReal = toReal(value_);
			auto rightReal = toReal(rhsVal);
			return convert<Operand>(make<Real>(boost::multiprecision::pow(leftReal, rightReal)));
		}

		// Positive integer exponent returns Integer
		// Convert to unsigned for pow
		auto exponent = static_cast<unsigned>(rhsVal);
		return convert<Operand>(make<Integer>(boost::multiprecision::pow(value_, exponent)));
	}
	else if (is<Real>(rhs)) {
		auto rhsVal = value_of<Real>(rhs);
		return convert<Operand>(make<Real>(boost::multiprecision::pow(toReal(value_), rhsVal)));
	}
	throw std::runtime_error("Error: invalid operand type for power");
}

// Relational Operators
INTEGER_RELATIONAL_OP(equality, == )
INTEGER_RELATIONAL_OP(inequality, != )
INTEGER_RELATIONAL_OP(greater, > )
INTEGER_RELATIONAL_OP(greater_equal, >= )
INTEGER_RELATIONAL_OP(less, < )
INTEGER_RELATIONAL_OP(less_equal, <= )


// Unary Operators
Operand::pointer_type Integer::perform_negation() {
return convert<Operand>(make<Integer>(-value_));
}

Operand::pointer_type Integer::perform_identity() {
	return convert<Operand>(make<Integer>(value_));
}

Operand::pointer_type Integer::perform_factorial() {
	if (value_ < 0) {
		throw std::runtime_error("Error: factorial of negative number");
	}

	Integer::value_type result = 1;
	for (Integer::value_type i = 2; i <= value_; ++i) {
		result *= i;
	}

	return convert<Operand>(make<Integer>(result));
}