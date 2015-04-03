#ifndef GUARD_HMM_HPP
#define GUARD_HMM_HPP

#include <string>
#include <vector>


class HiddenMarkovModel
{
public:
	HiddenMarkovModel(char* filename);

	std::vector<std::string> states() const { return _stateNames; };
	std::vector<std::string> outputs() const { return _outputNames; };

private:
	std::vector<std::string> _stateNames;
	std::vector<std::vector<double> > _stateMatrix;

	std::vector<std::string> _outputNames;
	std::vector<std::vector<double> > _outputMatrix;

	std::vector<double> _initStateMatrix;
};


#endif
