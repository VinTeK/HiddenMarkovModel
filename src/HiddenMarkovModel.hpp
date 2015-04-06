#ifndef GUARD_HMM_HPP
#define GUARD_HMM_HPP

#include <map>
#include <string>
#include <vector>


class HiddenMarkovModel
{
public:
	HiddenMarkovModel(const std::string& filename);

	size_t numOfStates() const { return _numOfStates; }
	size_t numOfOutputs() const { return _numOfOutputs; }

	/**
	 * Return state transition probability from states stt1 to stt2.
	 * @param stt1 source state
	 * @param stt2 destination state
	 */
	double transition(const std::string& stt1, const std::string& stt2);
	/**
	 * Return observation emission probability of output out in state stt.
	 * @param stt current state
	 * @param out the output symbol observed at this state
	 */
	double emission(const std::string& stt, const std::string& out);
	/**
	 * Return initial state probability of state stt.
	 * @param stt current state
	 */
	double initState(const std::string& stt);

	/**
	 * Returns initial probability of starting in a state.
	 */
	double initEval(const std::string& out, const std::string& stt);
	/**
	 * Returns probability of a single output symbol and a state transition.
	 */
	double eval(const std::string& out, const std::string stts[2]);
	/**
	 * Returns probability of an output sequence based on a given state sequence.
	 */
	double eval(const std::vector<std::string>& out, const std::vector<std::string>& stt);

	/**
	 * Returns the forward variables for each observation sequence in a given .obs file.
	 */
	std::vector<double> forward(const std::string& filename);
	/**
	 * Returns the backward variables for each observation sequence in a given .obs file.
	 */
	std::vector<double> backward(const std::string& filename);
	/**
	 * Returns the pair of the most likely state sequence probability and its actual state path
	 * for each observation sequence in a given .obs file.
	 */
	std::vector<std::pair<double, std::vector<std::string> > > viterbi(const std::string& filename);

private:
	double forwardHelper(const std::vector<std::string>&, int, const std::string&);
	double backwardHelper(const std::vector<std::string>&, int, const std::string&);
	std::pair<double, std::vector<std::string> > viterbiHelper(const std::vector<std::string>&);

private:
	size_t _numOfStates, _numOfOutputs;

	std::vector<std::string> _stateNames;

	std::map<std::string, std::map<std::string, double> > _transitions;
	std::map<std::string, std::map<std::string, double> > _emissions;
	std::map<std::string, double> _initStates;
};


#endif
