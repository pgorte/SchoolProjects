/*
* Name:		FileData.cpp
* Purpose:	FileData functions for storing FileData and filtering FileData vectors based on Options
* Author:	Peter Gortemaker
* Date:		August 12, 2025
*/

#include "FileData.hpp"

// templates for collect_data (used to be if-else block with identical repeat code)
template void collect_data<directory_iterator>(const path&, std::vector<FileData>&);
template void collect_data<recursive_directory_iterator>(const path&, std::vector<FileData>&);

// collect and add FileData into vector based on specific folder or current folder
// checks whether the scan is done recursively or not
template <typename Iterator>
void collect_data(const std::filesystem::path& folderPath, std::vector<FileData>& data)
{
	try
	{
		// recursively search through current folder and subfolders
		for (const auto& d : Iterator(folderPath))
		{
			// if not regular file skip
			if (!is_regular_file(d.path()))
			{
				continue;
			}

			// get extension name or if not explicit
			string ext = d.path().has_extension() ? d.path().extension().string() : "";

			// bool for whether FileData vector already contains a specific ext
			bool foundExt = false;

			// for every file
			for (auto& file : data)
			{
				// if ext already in vector
				if (file.extensionName == ext)
				{
					file.extensionCount++;
					file.filesizeCount += file_size(d.path());
					foundExt = true;
					break;
				}
			}
			// if ext not found, add new FileData in data
			if (!foundExt)
			{
				data.emplace_back(ext, 1, file_size(d.path()));
			}

		}
	}
	catch (exception& e)
	{
		cerr << "Error: " << e.what() << endl;
	}
}

// this function could have been simplified with lambda functions. I did not end up replacing it
// filters 'data' based on Options boolean criteria and builds a filtered 'report_data' vector
void filter_list(Options const& o, std::vector <FileData>& data, std::vector<FileData>& report_data)
{
	for (const auto& file : data)
	{
		// for each: check boolean and ext suffixes that are part of that type
		// if both boolean and matching .ext name, add corresponding data to report_data
		if (o.c && (file.extensionName == ".c" || file.extensionName == ".h"))
		{
			report_data.emplace_back(file);
		}
		if (o.cpp && (file.extensionName == ".cc" || file.extensionName == ".cp" || file.extensionName == ".cpp" ||
			file.extensionName == ".cxx" || file.extensionName == ".c++" || file.extensionName == ".hpp" || file.extensionName == ".hxx"))
		{
			report_data.emplace_back(file);
		}
		if (o.java && (file.extensionName == ".class" || file.extensionName == ".j" || file.extensionName == ".jad" ||
			file.extensionName == ".jar" || file.extensionName == ".java" || file.extensionName == ".jsp" || file.extensionName == ".ser"))
		{
			report_data.emplace_back(file);
		}
		if (o.cSharp && (file.extensionName == ".cs" || file.extensionName == ".vb" || file.extensionName == ".jsl"))
		{
			report_data.emplace_back(file);
		}
		if (o.web && (file.extensionName == ".htm" || file.extensionName == ".html" || file.extensionName == ".html5" ||
			file.extensionName == ".js" || file.extensionName == ".jse" || file.extensionName == ".jsc"))
		{
			report_data.emplace_back(file);
		}
		// if boolean regex
		if (o.hasRegex)
		{
			try
			{
				// if extName matches add to report_data
				regex pattern(o.expr);
				if (regex_match(file.extensionName, pattern))
				{
					report_data.emplace_back(file);
				}
			}
			catch (const regex_error& err)
			{
				cerr << "Invalid regex pattern: " << o.expr << " " << err.what() << endl;
				break;
			}
		} // end if
	} // end for

	// if no explicit search args, use whole data set
	if (!(o.c || o.cpp || o.java || o.cSharp || o.web || o.hasRegex))
	{
		report_data = data;
	}

	// check for size check and then reverse check, sort according to outline rules
	if (o.size)
	{
		if (o.reverse)
		{
			// sort by extensionName desc (for same filesizeCount)
			sort(report_data.begin(), report_data.end(),
				[](const FileData& a, const FileData& b)
				{return a.extensionName > b.extensionName; });

			// then sort by filesizeCount desc after - preserves extensionName sorting for same filesizeCounts
			sort(report_data.begin(), report_data.end(),
				[](const FileData& a, const FileData& b)
				{return a.filesizeCount > b.filesizeCount; });
		}
		else
		{
			// same but asc
			sort(report_data.begin(), report_data.end(),
				[](const FileData& a, const FileData& b)
				{return a.extensionName < b.extensionName; });

			sort(report_data.begin(), report_data.end(),
				[](const FileData& a, const FileData& b)
				{return a.filesizeCount < b.filesizeCount; });
		}
	}
	// else alpha sort
	else
	{
		sort(report_data.begin(), report_data.end(),
			[](const FileData& a, const FileData& b)
			{return a.extensionName < b.extensionName; });
	}// end size check

	// if reverse but not size, reverse the alpha sort
	if (o.reverse && !o.size)
	{
		reverse(report_data.begin(), report_data.end());
	}
}// end filter_list