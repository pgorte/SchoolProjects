/*
* Name:		PrintFileData.cpp
* Purpose:	Print functions for output of selected Options from a FileData vector and help/version prints
* Author:	Peter Gortemaker
* Date:		August 12, 2025
*/

#include "FileData.hpp"

// takes filesizeCounts and adds 1000th commas
std::string format_commas(long long value)
{
	string s = to_string(value);

	// get first comma position and each next (-3) while not at beginning (0)
	int pos = (int)(s.length() - 3);
	while (pos > 0)
	{
		s.insert(pos, ",");
		pos -= 3;
	}
	return s;

}// end format_commas

// help and version prints
// could be strictly formatted but this way it's easy to see what output would look like
void print(Options const& o)
{
	if (o.help)
	{
		cout << "fileusage {v1.0} (c) 2016-25, Peter Gortemaker\n\n";
		cout << "		Usage: fileusage [--help] [--version] [-,c+j#wsrRS(x regularexpression)] [folder]" << endl;
		cout << "		switches:" << endl;
		cout << "				,		commas" << endl;
		cout << "				c		filter c files" << endl;
		cout << "				+		filter c++ files" << endl;
		cout << "				j		filter java files" << endl;
		cout << "				#		filter C# files" << endl;
		cout << "				x		filter with a regular expression" << endl;
		cout << "				w		filter web files" << endl;
		cout << "				s		summary" << endl;
		cout << "				r		recursively visit subfolders" << endl;
		cout << "				R		reverse the order of the listing" << endl;
		cout << "				S		sort by file sizes" << endl;
		cout << "				v		verbose mode\n" << endl;
		cout << "		folder" << endl;
		cout << "				starting folder or current folder if not provided" << endl;
	}

	// Only worked if I copied Garth's
	if (o.version)
		//cout << "PG_Project3 1.0" << endl << "Made by Peter Gortemaker" << endl;
		cout << "4.1.0";
}// end print

// prints report_data unless summary bool specified
// adds commas if options bool
// size_t's were used to add spacing based on length of output
void print_list(Options const& o, std::vector<FileData>& report_data)
{
	// for summary output
	unsigned long long totalFiles = 0;
	unsigned long long totalSize = 0;
	size_t extTypeCount = 0;

	// get totals
	for (const auto& d : report_data)
	{
		totalFiles += d.extensionCount;
		totalSize += d.filesizeCount;
		extTypeCount++;
	}

	// print totals
	if (o.summary)
	{
		// set formatting based on commas or not
		size_t tsWidth = 0;

		if (o.commas)
			tsWidth = format_commas(totalSize).length();
		else
			tsWidth = to_string(totalSize).length();

		size_t tfWidth = to_string(totalFiles).length();

		// print with formatting based on commas or not
		if (o.commas)
			cout << setw(tsWidth) << right << format_commas(totalSize);

		else
			cout << setw(tsWidth) << right << totalSize;

		cout << " " << setw(tfWidth) << right << totalFiles;
		cout << " " << extTypeCount << endl;
	}// end summary print

	// print specified report_data
	else
	{
		size_t fsWidth = 0;
		size_t extWidth = 0;

		// set formatting based on commas or not
		for (const auto& d : report_data)
		{
			if (o.commas)
			{
				if (fsWidth < format_commas(d.filesizeCount).length())
					fsWidth = format_commas(d.filesizeCount).length();
			}
			else
			{
				if (fsWidth < to_string(d.filesizeCount).length())
					fsWidth = to_string(d.filesizeCount).length();
			}

			if (extWidth < to_string(d.extensionCount).length())
				extWidth = to_string(d.extensionCount).length();
		}

		// print with formatting based on commas or not
		for (const auto& d : report_data)
		{
			if (o.commas)
				cout << setw(fsWidth) << right << format_commas(d.filesizeCount);

			else
				cout << setw(fsWidth) << right << d.filesizeCount;

			cout << " " << setw(extWidth) << right << d.extensionCount;
			cout << " " << d.extensionName << endl;
		}
	}// end regular print
}// end print_list