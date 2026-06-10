/*
* Name:		FileData.hpp
* Purpose:	Header file for FileData function declarations
*			FileData class declaration
* Author:	Peter Gortemaker
* Date:		August 12, 2025
*/

#pragma once

#include "Options.hpp"

// FileData class - holds information of the files collected
class FileData
{
public:
	std::string extensionName = "";
	unsigned long long extensionCount = 0;
	unsigned long long filesizeCount = 0;

	// Constructor for data collection
	FileData(std::string extName, unsigned long long extCount, unsigned long long size)
		: extensionName(extName), extensionCount(extCount), filesizeCount(size) {
	}
};

//------------- FileData functions

/*
\fn: collect_data
\brief: Options to check for recursive file search and folder name
\		Collect data and store in FileData vector 'data'
\		Extension names and size and count of those extentions
\param: Options object, Filedata vector
*/
template<typename Iterator>
void collect_data(const std::filesystem::path& folderPath, std::vector <FileData>& data);

/*
\fn: filter_list
\brief: Filters FileData vector 'data' based on 'Options o' into FileData vector 'report_data'
\param: Options object for filtering and 2 FileData vectors to filter from one to the other
*/
void filter_list(Options const& o, std::vector <FileData>& data, std::vector<FileData>& report_data);

//------------- PrintFileData functions

/*
\fn: format_commas
\brief: If options boolean commas has been switch to true, add 1000th commas to output
		Only realistically effects the filesizeCount member of FileData
		For use directly in print statements as it cannot be stored back in filesizeCount
\param: long long value - in this case exclusively filesizeCount
\return: String representing filesizeCount with 1000th comma seperators
*/
std::string format_commas(long long value);

/*
\fn: print
\brief: Simple print function for --help and --version arguments
\param: Options o to check boolean argument selectors
*/
void print(Options const& o);

/*
\fn: print_list
\brief: Prints vector FileData report_data based on Options o args
\param: Options object, FileData vector with report data
*/
void print_list(Options const& o, std::vector<FileData>& report_data);