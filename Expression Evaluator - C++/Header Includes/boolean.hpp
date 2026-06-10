#pragma once

#include <ee/operand.hpp>
#include <ee/operator.hpp>


/*! Boolean token. */
class Boolean : public Operand {
public:
	using value_type = bool;
	DEF_POINTER_TYPE(Boolean)
private:
	value_type	value_;
public:
	Boolean(value_type const& value) : value_(value) {}

	[[nodiscard]] value_type	value() const { return value_; }
	[[nodiscard]] string_type	str() const override;

	// Double Dispatch methods for boolean operators
	[[nodiscard]] Operand::pointer_type perform_and(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_or(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_xor(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_nand(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_nor(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_xnor(Operand::pointer_type rhs) override;

	// Double Dispatch methods for relational operators
	[[nodiscard]] Operand::pointer_type perform_equality(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_inequality(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_greater(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_greater_equal(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_less(Operand::pointer_type rhs) override;
	[[nodiscard]] Operand::pointer_type perform_less_equal(Operand::pointer_type rhs) override;

	// Double Dispatch methods for unary operators
	[[nodiscard]] Operand::pointer_type perform_not() override;
};


/*! Boolean True literal. */
class True : public Boolean {
public:
	True() : Boolean(true) {}
};

/*! Boolean False literal. */
class False : public Boolean {
public:
	False() : Boolean(false) {}
};