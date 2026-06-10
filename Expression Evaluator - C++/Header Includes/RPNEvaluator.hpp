#pragma once

/* ----- Modified by Peter Gortemaker for Project 2 ----- */

#include <ee/operand.hpp>
#include <ee/operation.hpp>
#include <vector>

class RPNEvaluator {
	RPNEvaluator(RPNEvaluator const&) = delete;
	RPNEvaluator& operator = (RPNEvaluator const&) = delete;
public:
	RPNEvaluator() = default;

	// Main evaluation function - accepts results vector from ExpressionEvaluator
	[[nodiscard]] Operand::pointer_type evaluate(
		TokenList const& container,
		std::vector<Operand::pointer_type>& results);

	// Overload for direct calls (AutoMarker tests) - uses temporary results vector
	[[nodiscard]] Operand::pointer_type evaluate(TokenList const& container);
};