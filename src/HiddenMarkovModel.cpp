#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include "HiddenMarkovModel.hpp"

using namespace std;


template <typename T>
vector<T> split(const string& line)
{
	vector<T> ret;
	string::const_iterator i = line.begin(), j, end = line.end();

	while (i != end && j != end)
	{
		while (isspace(*i) && i != end) ++i;
		j = i;
		while (!isspace(*j) && j != end) ++j;

		if (i != j)
			ret.push_back(string(i, j));
		i = j;
	}
	return ret;
}

template <>
vector<int> split(const string& line)
{
	vector<int> ret;
	string::const_iterator i = line.begin(), j, end = line.end();

	while (i != end && j != end)
	{
		while (isspace(*i) && i != end) ++i;
		j = i;
		while (!isspace(*j) && j != end) ++j;

		if (i != j)
		{
			string str(i, j);
			ret.push_back(strtol(str.c_str(), NULL, 10));
		}
		i = j;
	}
	return ret;
}

template <>
vector<double> split(const string& line)
{
	vector<double> ret;
	string::const_iterator i = line.begin(), j, end = line.end();

	while (i != end && j != end)
	{
		while (isspace(*i) && i != end) ++i;
		j = i;
		while (!isspace(*j) && j != end) ++j;

		if (i != j)
		{
			string str(i, j);
			ret.push_back(strtod(str.c_str(), NULL));
		}
		i = j;
	}
	return ret;
}

vector<vector<double> > getMatrix(ifstream& file, int height, int width)
{
	// initialize matrix dims
	vector<vector<double> > ret(height);

	for (int row = 0; row < height; ++row)
	{
		string line;
		getline(file, line);

		ret[row] = split<double>(line);
	}

	return ret;
}


HiddenMarkovModel::HiddenMarkovModel(char* filename)
{
	ifstream file(filename);
	if (!file.is_open())
		throw runtime_error("file not found: " + string(filename));

	/* Create HMM based on input file, which is formatted like so:
	 *
	 * The first line contains integers N (number of states), M (number of observation symbols),
	 * and T (number of time steps or length of oberservation sequences). 
	 *
	 * The second contains each individual HMM state.
	 *
	 * The third line contains each possible observation symbol. */
	string line;

	// set vectors to their designated sizes
	getline(file, line);
	vector<int> sizes = split<int>(line);

	int N = sizes[0], M = sizes[1], T = sizes[2];

	_stateNames = vector<string>(sizes[0]);
	_outputNames = vector<string>(sizes[1]);

	// enumerate HMM states
	getline(file, line);
	_stateNames = split<string>(line);

	// enumerate HMM observations
	getline(file, line);
	_outputNames = split<string>(line);

	// set state transition probabilities
	file.ignore(numeric_limits<streamsize>::max(), '\n'); // consume until next line
	_stateMatrix = getMatrix(file, N, N);

	// set observations emission probabilties
	file.ignore(numeric_limits<streamsize>::max(), '\n');
	_outputMatrix = getMatrix(file, N, M);

	// set initial state probabilties
	file.ignore(numeric_limits<streamsize>::max(), '\n');
	_initStateMatrix = getMatrix(file, 1, N);
}
