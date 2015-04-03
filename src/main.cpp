#include <iostream>
#include "HiddenMarkovModel.hpp"

using namespace std;


void help(char*);


int main(int argc, char** argv)
{
	if (argc <= 1 || argc > 3)
	{
		help(argv[0]);
		return 1;
	}

	HiddenMarkovModel hmm(argv[1]);
	// parse observations
	
	for (auto x : hmm.states())
		cout << x << endl;
	cout << endl;
	for (auto x : hmm.outputs())
		cout << x << endl;

	return 0;
}


void help(char* program)
{
	cout << program << " : [model.hmm] [observation.obs]" << endl;
}
