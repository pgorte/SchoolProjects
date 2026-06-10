#pragma once

/* ----- Modified by Peter Gortemaker for Project 2 ----- */

#include <ee/operand.hpp>
#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/math/constants/constants.hpp>


/*! Real number token. */
class Real : public Operand {
public:
	DEF_POINTER_TYPE(Real)
		using value_type = boost::multiprecision::number<boost::multiprecision::cpp_dec_float<1000, int32_t, void>>;
private:
	value_type	value_;
public:
	Real(value_type value = value_type(0)) : value_(value) {}
	[[nodiscard]] value_type	value() const { return value_; };
	[[nodiscard]] string_type	str() const override;

	// Double Dispatch methods for binary operators
	[[nodiscard]] Operand::pointer_type perform_addition(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_subtraction(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_multiplication(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_division(Operand::pointer_type rhs) override;
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
};


/*! Pi constant token. */
class Pi : public Real {
public:
	Pi() : Real(boost::math::constants::pi<value_type>()) {}
};


/*! Euler constant token. */
class E : public Real {
public:
	E() : Real(boost::math::constants::e<value_type>()) {}
};