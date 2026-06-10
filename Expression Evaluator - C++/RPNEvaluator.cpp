/* ----- Modified by Peter Gortemaker for Project 2 ----- 
	Used Double Dispatch and Lookup Tables to majorly reduce code */

#include <ee/RPNEvaluator.hpp>
#include <ee/integer.hpp>
#include <ee/operation.hpp>
#include <ee/function.hpp>
#include <ee/variable.hpp>
#include <ee/real.hpp>
#include <stack>
#include <stdexcept>


// Helper to convert to Real value
static Real::value_type getRealValue(Operand::pointer_type const& operand) {
	if (is<Real>(operand)) return value_of<Real>(operand);
	else if (is<Integer>(operand)) return Real::value_type(value_of<Integer>(operand).str());
	throw std::runtime_error("Error: cannot convert to Real");
}

// Helper to dereference variables
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

// Store current results for Result function access
static std::vector<Operand::pointer_type>* current_results = nullptr;

// processes RPN token list and returns result
[[nodiscard]] Operand::pointer_type RPNEvaluator::evaluate(TokenList const& rpnExpression,
														   std::vector<Operand::pointer_type>& results) {
	// Set current results for Result function to access
	current_results = &results;

	std::stack<Operand::pointer_type> operandStack;

	// Process each token in the RPN expression
	for (auto const& token : rpnExpression) {
		if (is<Operand>(token)) {
			// Operands are pushed directly onto the stack
			operandStack.push(convert<Operand>(token));
		}
		else if (is<Operation>(token)) {
			// Operations use polymorphic perform() method
			auto operation = convert<Operation>(token);
			auto result = operation->perform(operandStack);
			operandStack.push(result);
		}
	}

	// Validate final stack state
	if (operandStack.size() > 1) {
		throw std::runtime_error("Error: too many operands");
	}
	if (operandStack.empty()) {
		throw std::runtime_error("Error: insufficient operands");
	}

	return operandStack.top();
}

// Overload for direct calls without results vector
[[nodiscard]] Operand::pointer_type RPNEvaluator::evaluate(TokenList const& container) {
	// Create temporary results vector for this evaluation
	std::vector<Operand::pointer_type> temp_results;
	auto result = evaluate(container, temp_results);
	// Store result in temp vector (for result() function to work)
	temp_results.push_back(result);
	return result;
}

// Result::perform override - needs access to RPNEvaluator's results
Operand::pointer_type Result::perform(std::stack<Operand::pointer_type>& operandStack) {
	if (operandStack.empty()) {
		throw std::runtime_error("Error: insufficient operands");
	}

	auto operand = operandStack.top();
	operandStack.pop();

	// Dereference if variable
	operand = dereference(operand);

	// Get the result index
	auto index_val = getRealValue(operand);
	int index = static_cast<int>(index_val);

	// Access results from current_results pointer
	if (!current_results) {
		throw std::runtime_error("Error: Result function called outside of evaluator context");
	}

	if (index < 1 || index > static_cast<int>(current_results->size())) {
		throw std::runtime_error("Error: result index out of range");
	}

	// Return previous result
	return (*current_results)[index - 1];
}