/*
* Name:		Options.cpp
* Purpose:	Reads arguments from the command line to booleans that
*				will trigger which types of data to report and or folders to search
*				as well as a help and version switch
* Author:	Peter Gortemaker
* Date:		August 12, 2025
*/

#include "FileData.hpp"

// Parses command line for arguments to form booleans that decide what is printed
Options parse_command_line(int argc, char* argv[])
{
	// instantiate Options class
	Options options;

	// deque to hold args that can be iterated through
	deque<string> args(argv + 1, argv + argc);

	// checks for help switch
	deque<string>::iterator found = find(args.begin(), args.end(), "--help");
	if (found != args.end())
	{
		// if true, report and end
		options.help = true;
		return options;
	}

	// check for version switch
	found = find(args.begin(), args.end(), "--version");
	if (found != args.end())
	{
		// if true, report and end
		options.version = true;
		return options;
	}

	// check for leading '-'
	// while there are args that follow '-'
	while (!args.empty() && args.front()[0] == '-')
	{
		// switches contains the bools/regex expression/folder
		// remove '-' so only actionable arguments remain
		string switches = args.front().substr(1);
		args.pop_front();

		// look for what switches are present and make switch
		// switches c through w take this form
		//c switch
		size_t c = switches.find('c');
		if (c != string::npos)
		{
			// set bool and remove argument from string 
			options.c = true;
			switches.erase(c, 1);
		}

		//+ - c++ switch
		size_t cpp = switches.find('+');
		if (cpp != string::npos)
		{
			options.cpp = true;
			switches.erase(cpp, 1);
		}

		//j - java switch
		size_t j = switches.find('j');
		if (j != string::npos)
		{
			options.java = true;
			switches.erase(j, 1);
		}

		//# - c# switch
		size_t cSharp = switches.find('#');
		if (cSharp != string::npos)
		{
			options.cSharp = true;
			switches.erase(cSharp, 1);
		}

		//w - web switch
		size_t web = switches.find('w');
		if (web != string::npos)
		{
			options.web = true;
			switches.erase(web, 1);
		}

		//x - regex switch
		//if x is present, next element in the deque must be a string expression
		size_t x = switches.find('x');
		if (x != string::npos)
		{
			// remove x
			options.hasRegex = true;
			switches.erase(x, 1);
			// expression is next string 
			options.expr = args.front();
			// remove expression string
			args.pop_front();
		}

		//r - recursion switch
		size_t rec = switches.find('r');
		if (rec != string::npos)
		{
			options.recursion = true;
			switches.erase(rec, 1);
		}

		//s - summary switch
		size_t sum = switches.find('s');
		if (sum != string::npos)
		{
			options.summary = true;
			switches.erase(sum, 1);
		}

		//R - reverse order switch
		size_t rev = switches.find('R');
		if (rev != string::npos)
		{
			options.reverse = true;
			switches.erase(rev, 1);
		}

		//S - file size switch
		size_t size = switches.find('S');
		if (size != string::npos)
		{
			options.size = true;
			switches.erase(size, 1);
		}

		//, - comma switch
		size_t com = switches.find(',');
		if (com != string::npos)
		{
			options.commas = true;
			switches.erase(com, 1);
		}

		if (!switches.empty())
		{
			// if no arguements
			exit(1);
		}
	}

	// get folder string if provided
	if (!args.empty())
	{
		// add remaining string if any
		options.folder = args.front();
		args.pop_front();
	}

	return options;

}// end options