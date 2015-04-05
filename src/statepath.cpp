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


	std::vector<string> sttpath;
	double max = 0.0;
	int i = 0;
	string state;
	while (i != hmm.observations.size())
	{
		for (auto obs : hmm.numofOutputs())
		{
			for (auto stt : hmm.numofStates())
			{
				double prob = hmm.emission(hmm._statenames[stt], hmm.observations[i][obs]);
				if (prob > max)
				{
					max = prob; 
					state = hmm._statenames[stt];
				}
			}
		}
		i++;
	}

	return 0;
}


void help(char* program)
{
	cout << program << ": [model.hmm] [observation.obs ...]" << endl;
}
