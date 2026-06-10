/*
* Name:		fileusage_main.cpp
* Purpose:	Reads arguments from user to search through a given folder for all files
*			Stores files into vectors
*			Reports contents based on arguements
* Author:	Peter Gortemaker
* Date:		August 12, 2025
*/

#include "FileData.hpp"

// main entry point into the program
// take text or input that will be typed at the command line

int main(int argc, char* argv[])
{
	// collect the input from the command line
	Options opt = parse_command_line(argc, argv);

	// print help, exit
	if (opt.help)
	{
		print(opt);
		return EXIT_SUCCESS;
	}

	// print version, exit
	if (opt.version)
	{
		print(opt);
		return EXIT_SUCCESS;
	}

	vector<FileData> data;
	vector<FileData> report_data;

	// collect from folderPath
	// if not defined by user, use current path
	path folderPath = opt.folder.empty() ? current_path() : path(opt.folder);

	// recursive or non-recursive search based on options bool
	if (opt.recursion)
		collect_data<recursive_directory_iterator>(folderPath, data);
	else
		collect_data<directory_iterator>(folderPath, data);

	// data is filtered into report_data based on options
	filter_list(opt, data, report_data);

	// print report data according to options
	print_list(opt, report_data);

	return EXIT_SUCCESS;

}// end main