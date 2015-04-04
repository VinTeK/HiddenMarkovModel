#ifndef GUARD_HMM_HPP
#define GUARD_HMM_HPP

#include <map>
#include <string>
#include <vector>


class HiddenMarkovModel
{
public:
	HiddenMarkovModel(const std::string& filename);

	/** Returns evaluation of a .obs file. */
	double eval(const std::string& filename) const;
	/** Returns probability of an output sequence based on a given state sequence. */
	double eval(const std::vector<std::string>& out, const std::vector<std::string>& stt) const;

private:
	std::map<std::string, std::map<std::string, double> > _transitions;
	std::map<std::string, std::map<std::string, double> > _emissions;
	std::map<std::string, double> _initStates;
};


#endif
