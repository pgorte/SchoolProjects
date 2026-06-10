#pragma once

#include <ee/operand.hpp>
#include <boost/multiprecision/cpp_int.hpp>

/* ----- Modified by Peter Gortemaker for Project 2 ----- */

/*! Integer token. */
class Integer : public Operand {
public:
	using value_type = boost::multiprecision::cpp_int;
	DEF_POINTER_TYPE(Integer)
private:
	value_type	value_;
public:
	Integer(value_type value = 0)
		: value_(value) {}

	[[nodiscard]] value_type	value() const { return value_; }
	[[nodiscard]] string_type	str() const override;

	// Double Dispatch methods for binary operators
	[[nodiscard]] Operand::pointer_type perform_addition(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_subtraction(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_multiplication(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_division(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_modulus(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_power(Operand::pointer_type rhs) override;

	// Double Dispatch methods for relational operators
	[[nodiscard]] Operand::pointer_type perform_equality(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_inequality(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_greater(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_greater_equal(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_less(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_less_equal(Operand::pointer_type rhs) override;

	// Double Dispatch methods for unary operators
	[[nodiscard]] Operand::pointer_type perform_negation() override;
	[[nodiscard]] Operand::pointer_type perform_identity() override;
	[[nodiscard]] Operand::pointer_type perform_factorial() override;
};