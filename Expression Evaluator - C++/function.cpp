/* ----- Modified by Peter Gortemaker for Project 2 ----- */

#include <ee/function.hpp>
#include <ee/integer.hpp>
#include <ee/real.hpp>
#include <ee/variable.hpp>
#include <map>
#include <functional>
#include <typeindex>
#include <stdexcept>

static_assert(sizeof(Abs) > 0, "Abs should be defined");
static_assert(sizeof(Max) > 0, "Max should be defined");
static_assert(sizeof(Min) > 0, "Min should be defined");
static_assert(sizeof(Pow) > 0, "Pow should be defined");


// Helper functions
// Convert an operand to Real value (handles both Real and Integer types)
static Real::value_type getRealValue(Operand::pointer_type const& operand) {
	if (is<Real>(operand)) return value_of<Real>(operand);
	else if (is<Integer>(operand)) return Real::value_type(value_of<Integer>(operand).str());
	throw std::runtime_error("Error: cannot convert to Real");
}


// Check if result should be Integer type
static bool shouldReturnInteger(Operand::pointer_type const& operand) {
	return is<Integer>(operand);
}

static bool shouldReturnInteger(Operand::pointer_type const& left, Operand::pointer_type const& right) {
	return is<Integer>(left) && is<Integer>(right);
}


// Convert Real result to Integer if needed, otherwise return as Real
static Operand::pointer_type makeResult(Real::value_type const& result, bool asInteger) {
	if (asInteger) {
		Integer::value_type intResult;
		if (result >= 0) {
			intResult = boost::multiprecision::floor(result).convert_to<Integer::value_type>();
		}
		else {intResult = boost::multiprecision::ceil(result).convert_to<Integer::value_type>();
		}
		return convert<Operand>(make<Integer>(intResult));
	}
	return convert<Operand>(make<Real>(result));
}

// Dereference a variable to get its value, or return the operand as-is
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



// Function type definitions for lookup tables
using OneArgFunc = std::function<Operand::pointer_type(Real::value_type, bool)>;
using TwoArgFunc = std::function<Operand::pointer_type(Real::value_type, Real::value_type, bool)>;



// Lookup table for one-argument functions
static const std::map<std::type_index, OneArgFunc> oneArgFunctions = {
	{typeid(Abs), [](Real::value_type val, bool asInt) {
		return makeResult(boost::multiprecision::abs(val), asInt);
	}},
	{typeid(Sqrt), [](Real::value_type val, bool) {
		if (val < 0) throw std::runtime_error("Error: square root of negative number");
		return convert<Operand>(make<Real>(boost::multiprecision::sqrt(val)));
	}},
	{typeid(Sin), [](Real::value_type val, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::sin(val)));
	}},
	{typeid(Cos), [](Real::value_type val, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::cos(val)));
	}},
	{typeid(Tan), [](Real::value_type val, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::tan(val)));
	}},
	{typeid(Arcsin), [](Real::value_type val, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::asin(val)));
	}},
	{typeid(Arccos), [](Real::value_type val, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::acos(val)));
	}},
	{typeid(Arctan), [](Real::value_type val, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::atan(val)));
	}},
	{typeid(Exp), [](Real::value_type val, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::exp(val)));
	}},
	{typeid(Ln), [](Real::value_type val, bool) {
		if (val <= 0) throw std::runtime_error("Error: ln of non-positive number");
		return convert<Operand>(make<Real>(boost::multiprecision::log(val)));
	}},
	{typeid(Log), [](Real::value_type val, bool) {
		if (val <= 0) throw std::runtime_error("Error: log of non-positive number");
		return convert<Operand>(make<Real>(boost::multiprecision::log10(val)));
	}},
	{typeid(Lb), [](Real::value_type val, bool) {
		if (val <= 0) throw std::runtime_error("Error: lb of non-positive number");
		return convert<Operand>(make<Real>(boost::multiprecision::log(val) / boost::multiprecision::log(Real::value_type(2))));
	}},
	{typeid(Ceil), [](Real::value_type val, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::ceil(val)));
	}},
	{typeid(Floor), [](Real::value_type val, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::floor(val)));
	}}
};



// Lookup table for two-argument functions
static const std::map<std::type_index, TwoArgFunc> twoArgFunctions = {
	{typeid(Max), [](Real::value_type left, Real::value_type right, bool asInt) {
		return makeResult(boost::multiprecision::max(left, right), asInt);
	}},
	{typeid(Min), [](Real::value_type left, Real::value_type right, bool asInt) {
		return makeResult(boost::multiprecision::min(left, right), asInt);
	}},
	{typeid(Pow), [](Real::value_type left, Real::value_type right, bool asInt) {
		if (right < 0) return convert<Operand>(make<Real>(boost::multiprecision::pow(left, right)));
		return makeResult(boost::multiprecision::pow(left, right), asInt);
	}},
	{typeid(Arctan2), [](Real::value_type left, Real::value_type right, bool) {
		return convert<Operand>(make<Real>(boost::multiprecision::atan2(left, right)));
	}}
};



// Function Implementations
// OneArgFunction::perform - via lookup table
Operand::pointer_type OneArgFunction::perform(std::stack<Operand::pointer_type>& operandStack) {
	// Check if we have enough operands
	if (operandStack.empty()) {
		throw std::runtime_error("Error: insufficient operands");
	}

	// Pop the single argument
	auto operand = operandStack.top();
	operandStack.pop();

	// Dereference if variable
	operand = dereference(operand);

	// Look up function in table and execute
	auto it = oneArgFunctions.find(typeid(*this));
	if (it != oneArgFunctions.end()) {
		auto val = getRealValue(operand);
		bool asInt = shouldReturnInteger(operand);
		return it->second(val, asInt);
	}

	throw std::runtime_error("Error: unsupported one-argument function");
}



// TwoArgFunction::perform - via lookup table
Operand::pointer_type TwoArgFunction::perform(std::stack<Operand::pointer_type>& operandStack) {
	// Check if we have enough operands
	if (operandStack.size() < 2) {
		throw std::runtime_error("Error: insufficient operands");
	}

	// Pop arguments (right then left)
	auto right = operandStack.top(); operandStack.pop();
	auto left = operandStack.top(); operandStack.pop();

	// Dereference if variables
	left = dereference(left);
	right = dereference(right);

	// Look up function in table and execute
	auto it = twoArgFunctions.find(typeid(*this));
	if (it != twoArgFunctions.end()) {
		auto leftVal = getRealValue(left);
		auto rightVal = getRealValue(right);
		bool asInt = shouldReturnInteger(left, right);
		return it->second(leftVal, rightVal, asInt);
	}

	throw std::runtime_error("Error: unsupported two-argument function");
}



// ThreeArgFunction::perform
Operand::pointer_type ThreeArgFunction::perform(std::stack<Operand::pointer_type>& operandStack) {
	throw std::runtime_error("Error: three-argument functions not yet implemented");
}