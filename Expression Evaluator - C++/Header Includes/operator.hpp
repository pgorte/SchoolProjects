#pragma once

/* ----- Modified by Peter Gortemaker for Project 2 ----- 
	Added virtual operator precedence and perform functions to each class */

#include <ee/operand.hpp>
#include <ee/operation.hpp>
#include <ee/function.hpp>
#include <stack>


/*! Operator Precedence values. */
enum class Precedence { MIN = 0,
	ASSIGNMENT, LOGOR, LOGXOR, LOGAND, BITOR,
	BITXOR, BITAND, EQUALITY, RELATIONAL, BITSHIFT,
	ADDITIVE, MULTIPLICATIVE, UNARY, POWER, POSTFIX,
	MAX };




/*! Operator token base class. */
class Operator : public Operation {
public:
	DEF_POINTER_TYPE(Operator)

	// Get operator precedence
	[[nodiscard]] virtual Precedence precedence() const = 0;

	// Override Operation's perform method - uses Double Dispatch
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override = 0;
};

	/*! Binary operator token base class. */
	class BinaryOperator : public Operator {};

		/*! Right-associative operator base class. */
		class RAssocOperator : public BinaryOperator {};

			/*! Power token. */
			class Power : public RAssocOperator {
			public:
				[[nodiscard]] Precedence precedence() const override { return Precedence::POWER; }
				[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
			};

			/*! Assignment token. */
			class Assignment : public RAssocOperator {
			public:
				[[nodiscard]] Precedence precedence() const override { return Precedence::ASSIGNMENT; }
				[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
			};

/*! Left-associative operator base class. */
class LAssocOperator : public BinaryOperator {};

		/*! Addition token. */
class Addition : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::ADDITIVE; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! And token. */
class And : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::LOGAND; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Division token. */
class Division : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::MULTIPLICATIVE; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Equality token. */
class Equality : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::EQUALITY; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Greater than token. */
class Greater : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::RELATIONAL; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Greater than or equal to token. */
class GreaterEqual : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::RELATIONAL; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Inequality operator token. */
class Inequality : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::EQUALITY; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Less than operator token. */
class Less : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::RELATIONAL; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Less than equal-to operator token. */
class LessEqual : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::RELATIONAL; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Multiplication operator token. */
class Multiplication : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::MULTIPLICATIVE; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Modulus operator token. */
class Modulus : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::MULTIPLICATIVE; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Nand operator token. */
class Nand : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::LOGAND; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Nor operator token. */
class Nor : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::LOGAND; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Or operator token. */
class Or : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::LOGOR; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Subtraction operator token. */
class Subtraction : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::ADDITIVE; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! XOR operator token. */
class Xor : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::LOGXOR; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! XNOR operator token. */
class Xnor : public LAssocOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::LOGXOR; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};


/*! Non-associative operator token base class. */
class NonAssociative : public Operator {};

		/*! Unary operator token base class. */
class UnaryOperator : public NonAssociative {};

		/*! Identity operator token. */
class Identity : public UnaryOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::UNARY; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Negation operator token. */
class Negation : public UnaryOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::UNARY; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Not operator token. */
class Not : public UnaryOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::UNARY; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};

/*! Postfix Operator token base class. */
class PostfixOperator : public UnaryOperator {};

		/*! Factorial token base class. */
class Factorial : public PostfixOperator {
public:
	[[nodiscard]] Precedence precedence() const override { return Precedence::POSTFIX; }
	[[nodiscard]] Operand::pointer_type perform(std::stack<Operand::pointer_type>& operandStack) override;
};
