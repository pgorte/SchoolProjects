/* ----- Modified by Peter Gortemaker for Project 2 ----- */

#include <ee/operator.hpp>
#include <ee/operand.hpp>
#include <ee/variable.hpp>
#include <stack>


// Helper function to dereference a variable if needed
static Operand::pointer_type dereference(Operand::pointer_type operand) {
	if (is<Variable>(operand)) {
		auto var = convert<Variable>(operand);
		if (!var->value()) {
			throw std::runtime_error("Error: variable not initialized");
		}
		return var->value();
	}
	return operand;
}




// Binary Operators
// Addition
Operand::pointer_type Addition::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_addition(right);
}

// Subtraction
Operand::pointer_type Subtraction::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_subtraction(right);
}

// Multiplication
Operand::pointer_type Multiplication::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_multiplication(right);
}

// Division
Operand::pointer_type Division::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_division(right);
}

// Modulus
Operand::pointer_type Modulus::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_modulus(right);
}

// Power
Operand::pointer_type Power::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_power(right);
}


// Assignment
Operand::pointer_type Assignment::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	// Left must be a variable (don't dereference!)
	if (!is<Variable>(left)) {
		throw std::runtime_error("Error: assignment to a non-variable.");
	}

	// Dereference right if it's a variable
	right = dereference(right);

	// Perform assignment
	auto var = convert<Variable>(left);
	var->set(right);

	return left;
}




// Boolean Operators
// And
Operand::pointer_type And::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_and(right);
}

// Or
Operand::pointer_type Or::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_or(right);
}

// Xor
Operand::pointer_type Xor::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_xor(right);
}

// Nand
Operand::pointer_type Nand::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_nand(right);
}

// Nor
Operand::pointer_type Nor::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_nor(right);
}

// Xnor
Operand::pointer_type Xnor::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_xnor(right);
}





// Relational Operators
// Equality
Operand::pointer_type Equality::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_equality(right);
}

// Inequality
Operand::pointer_type Inequality::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_inequality(right);
}

// Greater
Operand::pointer_type Greater::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_greater(right);
}

// GreaterEqual
Operand::pointer_type GreaterEqual::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_greater_equal(right);
}

// Less
Operand::pointer_type Less::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_less(right);
}

// LessEqual
Operand::pointer_type LessEqual::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	left = dereference(left);
	right = dereference(right);

	return left->perform_less_equal(right);
}




// Unary Operators
// Negation
Operand::pointer_type Negation::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.empty()) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto operand = operandStack.top(); operandStack.pop();

	operand = dereference(operand);

	return operand->perform_negation();
}

// Identity
Operand::pointer_type Identity::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.empty()) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto operand = operandStack.top(); operandStack.pop();

	operand = dereference(operand);

	return operand->perform_identity();
}

// Not
Operand::pointer_type Not::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.empty()) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto operand = operandStack.top(); operandStack.pop();

	operand = dereference(operand);

	return operand->perform_not();
}

// Factorial
Operand::pointer_type Factorial::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.empty()) {
		throw std::runtime_error("Error: insufficient operands");
	}
	auto operand = operandStack.top(); operandStack.pop();

	operand = dereference(operand);

	return operand->perform_factorial();
}