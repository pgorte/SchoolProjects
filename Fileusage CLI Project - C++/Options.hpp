/*
* Name:		Options.hpp
* Purpose:	Options header for the Options function and Options class declaration
*			- Includes for fileusage project
* Author:	Peter Gortemaker
* Date:		August 12, 2025
*/

#pragma once

#include <iostream>
#include <string>
#include <deque>
#include <algorithm>
#include <regex>
#include <vector>
#include <iomanip>
#include <map>
#include <set>

#include <filesystem>
using namespace std::filesystem;
using namespace std;

// Options class - booleans for collecting/filtering/printing selection
//					Regex expression string
//					Folder path
class Options
{
public:
	bool c = false;
	bool cpp = false;
	bool java = false;
	bool cSharp = false;
	bool web = false;
	bool hasRegex = false;

	bool recursion = false;

	bool size = false;
	bool reverse = false;
	bool summary = false;
	bool commas = false;

	bool help = false;
	bool version = false;

	std::string expr = "";
	std::filesystem::path folder = "";
};

/*
\fn: parse_command_line
\brief: Parses the command line for arguments that determine which information should be
		collected, from where, and what should be reported back to user
\param: Command line arguments
\return: An Options object
*/
Options parse_command_line(int argc, char* argv[]);