#include <algorithm>
#include <fstream>
#include <iostream>
#include "HiddenMarkovModel.hpp"

using namespace std;


void help(char*);
vector<double> forward(const HiddenMarkovModel& hmm, const string& filename);


int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		help(argv[0]);
		return 1;
	}

	/* Parse arguments. We accept only one .hmm file but allow multiple .obs files. */
	string hmmFilename;
	vector<string> obsFilenames;

	for (int i = 1; i < argc; ++i)
	{
		string arg(argv[i]);

		if (arg.find(".hmm") != string::npos)
			hmmFilename = arg;
		else if (arg.find(".obs") != string::npos)
			obsFilenames.push_back(arg);
	}

	if (hmmFilename.empty())
	{
		cerr << "no .hmm file found" << endl;
		return 1;
	}

	HiddenMarkovModel hmm(hmmFilename);

	/* Evaluate forward algorithm for each .obs file. Each file may have multiple sequences. */
	for (auto i = obsFilenames.begin(); i != obsFilenames.end(); ++i)
	{
		cout << *i << ":" << endl;

		/* Print the evaluation results for each observation in this file. */
		for (auto result : forward(hmm, *i))
			cout << result << endl;
	}

	return 0;
}


void help(char* program)
{
	cout << program << ": [model.hmm] [observation.obs ...]" << endl;
}


vector<double> forward(const HiddenMarkovModel& hmm, const string& filename)
{
	ifstream file(filename);
	if (!file.is_open())
		throw runtime_error("file not found: " + string(filename));

	int count;
	file >> count;
	file.ignore(numeric_limits<streamsize>::max(), '\n');

	vector<vector<string> > observations(count);

	for (int i = 0; i < count; ++i)
	{
		file.ignore(numeric_limits<streamsize>::max(), '\n');

		string line;
		getline(file, line);

		observations[i] = split<string>(line);
	}

	vector<double> ret;

	// TODO

	return ret;
}
