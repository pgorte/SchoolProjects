/* ----- Modified by Peter Gortemaker for Project 2 ----- */

#include <ee/parser.hpp>
#include <ee/function.hpp>
#include <ee/operand.hpp>
#include <ee/operator.hpp>
#include <ee/pseudo_operation.hpp>
#include <stack>

// Following the Shunting Yard Algorithm
[[nodiscard]] TokenList Parser::parse(TokenList const& infixTokens) {
	
	TokenList outputQueue; // Step 1: Set output-queue to empty
	std::stack<Token::pointer_type> operationStack; // Step 2

	for (auto const& token : infixTokens) { // Step 3

		// Step 4-5
		if (is<Operand>(token)) {
			outputQueue.push_back(token);
		}

		// Step 6-7
		else if (is<Function>(token)) {
			operationStack.push(token);
		}

		// Step 8-11
		else if (is<ArgumentSeparator>(token)) {
			while (!operationStack.empty() && !is<LeftParenthesis>(operationStack.top())) { // Step 9
				// Step 10-11
				outputQueue.push_back(operationStack.top());
				operationStack.pop();
			}
		}

		// Step 12-13
		else if (is<LeftParenthesis>(token)) {
			operationStack.push(token);
		}

		// Step 14-23
		else if (is<RightParenthesis>(token)) {
			while (!operationStack.empty() && !is<LeftParenthesis>(operationStack.top())) { // Step 15
				// Step 16-17
				outputQueue.push_back(operationStack.top());
				operationStack.pop();
			}

			// Step 18-19
			if (operationStack.empty()) {
				throw std::runtime_error("Right parenthesis has no matching left parenthesis");
			}

			operationStack.pop(); // Step 20

			// Step 21-23
			if (!operationStack.empty() && is<Function>(operationStack.top())) {
				outputQueue.push_back(operationStack.top());
				operationStack.pop();
			}
		}

		// Step 24-37
		else if (is<Operator>(token)) {
			auto currentOp = convert<Operator>(token);

			while (!operationStack.empty()) { // Step 25
				// Step 26-27
				if (!is<Operator>(operationStack.top())) {
					break;
				}

				auto stackOp = convert<Operator>(operationStack.top());

				// Step 28-29
				if (is<NonAssociative>(token)) {
					break;
				}

				// Step 30-31
				if (is<LAssocOperator>(token) && currentOp->precedence() > stackOp->precedence()) {
					break;
				}

				// Step 32-33
				if (is<RAssocOperator>(token) && currentOp->precedence() >= stackOp->precedence()) {
					break;
				}

				// Step 34-35
				outputQueue.push_back(operationStack.top());
				operationStack.pop();

			} // Step 36: End while

			operationStack.push(token); // Step 37
		}

		// Step 38-39
		else {
			throw std::runtime_error("Unknown token");
		}
	} // Step 40: End for-each

	// Step 41
	while (!operationStack.empty()) {
		// Step 42-43
		if (is<LeftParenthesis>(operationStack.top())) {
			throw std::runtime_error("Missing right-parenthesis");
		}

		// Step 44-45
		outputQueue.push_back(operationStack.top());
		operationStack.pop();
	}

	// Step 46
	return outputQueue;
}
