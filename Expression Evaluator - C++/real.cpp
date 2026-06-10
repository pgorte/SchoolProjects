/* ----- Modified by Peter Gortemaker for Project 2 ----- */

#include <ee/real.hpp>
#include <ee/boolean.hpp>
#include <ee/function.hpp>
#include <ee/operator.hpp>
#include <ee/integer.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iomanip>
#include <sstream>
using namespace std;


[[nodiscard]] Real::string_type Real::str() const {
	ostringstream oss;
	oss << /*"Real: " << */fixed << setprecision(numeric_limits<value_type>::digits10) << value_;
	return oss.str();
}

// Helper function to convert Integer to Real
static Real::value_type toReal(Integer::value_type const& intVal) {
	return Real::value_type(intVal.str());
}

// Macros for generating repetitive operations
// Binary Macro
#define REAL_BINARY_OP(OP_NAME, OP_SYMBOL) \
Operand::pointer_type Real::perform_##OP_NAME(Operand::pointer_type rhs) { \
	if (is<Real>(rhs)) { \
		return convert<Operand>(make<Real>(value_ OP_SYMBOL value_of<Real>(rhs))); \
	} \
	else if (is<Integer>(rhs)) { \
		return convert<Operand>(make<Real>(value_ OP_SYMBOL toReal(value_of<Integer>(rhs)))); \
	} \
	throw std::runtime_error("Error: invalid operand type for " #OP_NAME); \
}

// Relational Macro
#define REAL_RELATIONAL_OP(OP_NAME, OP_SYMBOL) \
Operand::pointer_type Real::perform_##OP_NAME(Operand::pointer_type rhs) { \
	if (is<Real>(rhs)) { \
		return convert<Operand>(make<Boolean>(value_ OP_SYMBOL value_of<Real>(rhs))); \
	} \
	else if (is<Integer>(rhs)) { \
		return convert<Operand>(make<Boolean>(value_ OP_SYMBOL toReal(value_of<Integer>(rhs)))); \
	} \
	throw std::runtime_error("Error: invalid operand type for " #OP_NAME); \
}

// Binary Arithmetic Operators
REAL_BINARY_OP(addition, +)
REAL_BINARY_OP(subtraction, -)
REAL_BINARY_OP(multiplication, *)

// Binary Arithmetic Operators (special cases)
Operand::pointer_type Real::perform_division(Operand::pointer_type rhs) {
	if (is<Real>(rhs)) {
		auto rhsVal = value_of<Real>(rhs);
		if (rhsVal == 0) {
			throw std::runtime_error("Error: division by zero");
		}
		return convert<Operand>(make<Real>(value_ / rhsVal));
	}
	else if (is<Integer>(rhs)) {
		auto rhsVal = toReal(value_of<Integer>(rhs));
		if (rhsVal == 0) {
			throw std::runtime_error("Error: division by zero");
		}
		return convert<Operand>(make<Real>(value_ / rhsVal));
	}
	throw std::runtime_error("Error: invalid operand type for division");
}

Operand::pointer_type Real::perform_power(Operand::pointer_type rhs) {
	if (is<Real>(rhs)) {
		return convert<Operand>(make<Real>(boost::multiprecision::pow(value_, value_of<Real>(rhs))));
	}
	else if (is<Integer>(rhs)) {
		return convert<Operand>(make<Real>(boost::multiprecision::pow(value_, toReal(value_of<Integer>(rhs)))));
	}
	throw std::runtime_error("Error: invalid operand type for power");
}

// Relational Operators
REAL_RELATIONAL_OP(equality, == )
REAL_RELATIONAL_OP(inequality, != )
REAL_RELATIONAL_OP(greater, > )
REAL_RELATIONAL_OP(greater_equal, >= )
REAL_RELATIONAL_OP(less, < )
REAL_RELATIONAL_OP(less_equal, <= )

// Unary Operators
Operand::pointer_type Real::perform_negation() {
	return convert<Operand>(make<Real>(-value_));
}

Operand::pointer_type Real::perform_identity() {
	return convert<Operand>(make<Real>(value_));
}