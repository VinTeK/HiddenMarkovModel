#include <algorithm>
#include <iostream>
#include "HiddenMarkovModel.hpp"

using namespace std;


void help(char*);


int main(int argc, char** argv)
{
	if (argc <= 1)
	{
		help(argv[0]);
		return 1;
	}

	// arg parsing
	string hmmFile;
	vector<string> obsFiles;

	while (argc != 0)
	{
		string arg(argv[--argc]);

		if (arg.find(".hmm"))
			hmmFile = arg;
		else if (arg.find(".obs"))
			obsFiles.push_back(arg);
	}

	if (hmmFile.empty())
	{
		cerr << "no .hmm file found" << endl;
		return 1;
	}

	HiddenMarkovModel hmm(hmmFile);

	/*
	for (auto i = obsFiles.begin(); i != obsFiles.end(); ++i)
	{
		cout << *i << " = ";
		hmm.evaluate(*i);
		cout << endl;
	}
	*/

	return 0;
}


void help(char* program)
{
	cout << program << ": [model.hmm] [observation.obs ...]" << endl;
}
