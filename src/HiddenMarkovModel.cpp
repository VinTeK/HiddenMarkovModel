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


HiddenMarkovModel::HiddenMarkovModel(const string& filename)
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

	getline(file, line);
	vector<int> sizes = split<int>(line);

	// parse first line
	//int N = sizes[0], M = sizes[1], T = sizes[2];

	// get state names
	getline(file, line);
	vector<string> stateNames = split<string>(line);

	// get output names
	getline(file, line);
	vector<string> outputNames = split<string>(line);

	// consume "a:"
	file.ignore(numeric_limits<streamsize>::max(), '\n');

	// initialize state transition probability matrix
	for (auto i = stateNames.begin(); i != stateNames.end(); ++i)
	{
		getline(file, line);
		vector<double> curLine = split<double>(line);
		int col = 0;

		for (auto j = stateNames.begin(); j != stateNames.end(); ++j)
			_transitions[*i][*j] = curLine[col++];
	}

	// consume "b:"
	file.ignore(numeric_limits<streamsize>::max(), '\n');

	// initialize output emission probability matrix
	for (auto i = stateNames.begin(); i != stateNames.end(); ++i)
	{
		getline(file, line);
		vector<double> curLine = split<double>(line);
		int col = 0;

		for (auto j = outputNames.begin(); j != outputNames.end(); ++j)
			_emissions[*i][*j] = curLine[col++];
	}

	// consume "pi:"
	file.ignore(numeric_limits<streamsize>::max(), '\n');

	// set initial state probabilties
	getline(file, line);
	vector<double> tmp = split<double>(line);
	int col = 0;
	for (auto i = stateNames.begin(); i != stateNames.end(); ++i)
	{
		_initStates[*i] = tmp[col++];
	}
}


double HiddenMarkovModel::transition(const std::string& stt1, const std::string& stt2)
{
	// check if this state name exists as a key in our map
	if (_transitions.find(stt1) == _transitions.end())
		throw runtime_error("No such state: " + stt1);
	if (_transitions[stt1].find(stt2) == _transitions[stt1].end())
		throw runtime_error("No such state: " + stt2);

	return _transitions[stt1][stt2];
}


double HiddenMarkovModel::emission(const std::string& stt, const std::string& out)
{
	if (_emissions.find(stt) == _emissions.end())
		throw runtime_error("No such state: " + stt);
	if (_emissions[stt].find(out) == _emissions[stt].end())
		throw runtime_error("No such output: " + out);

	return _emissions[stt][out];
}


double HiddenMarkovModel::initState(const std::string& stt)
{
	if (_emissions.find(stt) == _emissions.end())
		throw runtime_error("No such state: " + stt);

	return _initStates[stt];
}


double HiddenMarkovModel::initEval(const string& out, const string& stt)
{
	return _initStates[stt] * _emissions[stt][out];
}


double HiddenMarkovModel::eval(const string& out, const string stts[2])
{
	return transition(stts[0], stts[1]) * emission(stts[1], out);
}


double HiddenMarkovModel::eval(const vector<string>& out, const vector<string>& stt)
{
	if (out.size() != stt.size())
		return 0;

	vector<string>::const_iterator curStt = stt.begin(), curOut = out.begin();
	double ret = initEval(*curOut++, *curStt);

	while (curOut != out.end())
	{
		string tmp[2] = {*curStt, *(++curStt)};
		ret *= eval(*curOut++, tmp);
	}

	return ret;
}
