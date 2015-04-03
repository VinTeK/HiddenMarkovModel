#ifndef GUARD_HMM_HPP
#define GUARD_HMM_HPP

#include <map>
#include <string>
#include <vector>


class HiddenMarkovModel
{
public:
	HiddenMarkovModel(const std::string& filename);

	/** Returns probability of an output sequence based on a given state sequence. */
	double evaluate(const std::string& filename) const;
	double evaluate(const std::vector<int>& outputSeq, const std::vector<int>& stateSeq) const;

private:
	std::map<std::string, int> _states;
	std::vector<std::vector<double> > _stateMatrix;

	std::map<std::string, int> _outputs;
	std::vector<std::vector<double> > _outputMatrix;

	std::vector<double> _initStateMatrix;
};


#endif
