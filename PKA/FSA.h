#pragma once
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

using namespace std;
struct ATransition {
	int source;
	string word;
	int dest;
};

struct StateDescriptor {
	int state;
	bool is_starting;
	bool is_final;
};
class FSA
{
	/* default disposition is 1 so all automata start their states from 1*/
	/* state = i + disposition, for i=0 to stateSize */
	void _remap(vector<StateDescriptor> states, int Q = 0) {
		/* n - |trn|, m - |Q|; 
		*  O(nlogn)-time with map
		*  O(n) time with unordered_map
		*  O(m)-memory 
		*/

		int k = disposition =  Q + 1;
		int counter = 0;
		unordered_map<int, int> st_map;

		for (auto& s : states) {
			st_map[s.state] = k;
			starting[counter] = s.is_starting;
			accepting[counter] = s.is_final;
			counter++;
			k++;
		}

		for (auto& t : trn) {
			t.source = st_map[t.source];
			t.dest = st_map[t.dest];
		}
	}

	bool _check(int state, vector<string>& input, int index) {
		/*	naive implementation need to check whole trn array to get the right transitions */
		if (index == input.size()) {
			return accepting[state - disposition];
		}

		for (auto& t : trn) {
			//normal transitions
			if (t.source == state && t.word == input[index] && _check(t.dest, input, index + 1)) {
				return true;
			}

			//e-transitions
			if (t.source == state && t.word == "" && _check(t.dest, input, index)) {
				return true;
			}
		}

		return false;
	}

public:
	vector<ATransition> trn;
	bool* starting;
	bool* accepting;

	int disposition;
	int stateSize;

	bool check(vector<string> input) {
		for (int i = 0; i < stateSize; i++) {
			if (starting[i] && _check(i + disposition, input, 0)) {
				return true;
			}
		}

		return false;
	}

	FSA() : starting(NULL), accepting(NULL) {
		disposition = 1;
		stateSize = 0;
	}

	FSA(vector<StateDescriptor> states, vector<ATransition> transitions, int disposition = 0)
		:starting(NULL), accepting(NULL), trn(transitions)
	{
		stateSize = states.size();
		starting = new bool[stateSize];
		accepting = new bool[stateSize];
		_remap(states, disposition);
	}

	FSA(const FSA& A) {
		if (this == &A) {
			return;
		}

		trn = A.trn;
		stateSize = A.stateSize;
		disposition = A.disposition;
		starting = new bool[stateSize];
		accepting = new bool[stateSize];

		for (int i = 0; i < stateSize; i++) {
			starting[i] = A.starting[i];
			accepting[i] = A.accepting[i];
		}
	}

	~FSA()
	{
		delete[] starting;
		delete[] accepting;
	}

	void remap(int Q) {
		//state = i+disposition

		int k = Q + 1;
		unordered_map<int, int> st_map;

		for (int i = 0; i < stateSize; i++) {
			st_map[i + disposition] = k;
			k++;
		}

		for (auto& t : trn) {
			t.source = st_map[t.source];
			t.dest = st_map[t.dest];
		}

		disposition = Q + 1;
	}

	static FSA Union(const FSA& N, const FSA& M) {
		FSA result(N);

		FSA B(M);//TODO: optimize this so we avoid copy-constructor of M
		B.remap(N.stateSize-1 + N.disposition);

		bool* new_starting = new bool[N.stateSize + B.stateSize];
		bool* new_accepting = new bool[N.stateSize + B.stateSize];

		for (int i = 0; i < N.stateSize; i++) {
			new_starting[i] = N.starting[i];
			new_accepting[i] = N.accepting[i];
		}

		for (int i = N.stateSize; i < N.stateSize + B.stateSize; i++) {
			new_starting[i] = B.starting[i - N.stateSize];
			new_accepting[i] = B.accepting[i - N.stateSize];
		}

		delete[] result.starting;
		delete[] result.accepting;
		result.starting = new_starting;
		result.accepting = new_accepting;
		result.stateSize = N.stateSize + B.stateSize;
		result.disposition = N.disposition;

		for (auto& t : B.trn) {
			result.trn.push_back(t);
		}

		return result;
	}

	static FSA Concat(const FSA& N, const FSA& M) {
		//this concatenation is creating e-transitions
		FSA result(N);
		FSA M_temp(M);//TODO: optimize this so we avoid copy-constructor of M

		M_temp.remap(N.stateSize - 1 + N.disposition);

		bool* new_starting = new bool[N.stateSize + M_temp.stateSize];
		bool* new_accepting = new bool[N.stateSize + M_temp.stateSize];

		vector<int> prev_start_states;
		vector<int> prev_accepting_states;

		for (int i = 0; i < N.stateSize; i++) {
			new_starting[i] = N.starting[i];
			new_accepting[i] = false;

			if (N.accepting[i]) {
				//was accepting
				//state=i+disposition
				prev_accepting_states.push_back(i + N.disposition);
			}
		}

		for (int i = N.stateSize; i < N.stateSize + M_temp.stateSize; i++) {
			new_starting[i] = false;
			new_accepting[i] = M_temp.accepting[i - N.stateSize];

			if (M_temp.starting[i - N.stateSize]) {
				//was starting
				//collecting the states which are converted starting->non staring
				prev_start_states.push_back(i + N.disposition);
			}
		}

		delete[] result.starting;
		delete[] result.accepting;
		result.starting = new_starting;
		result.accepting = new_accepting;
		result.stateSize = N.stateSize + M_temp.stateSize;
		result.disposition = N.disposition;

		for (auto& t : M_temp.trn) {
			result.trn.push_back(t);
		}

		//creating the e-links between accepting<->starting states
		for (auto a : prev_accepting_states) {
			for (auto s : prev_start_states) {
				result.trn.push_back({ a, "", s });
			}
		}

		return result;
	}

	static FSA Star(const FSA& N, bool positive = false) {
		FSA result(N);

		bool* new_starting = new bool[result.stateSize + 1];
		bool* new_accepting = new bool[result.stateSize + 1];
		vector<int> prev_start_states;
		vector<int> prev_accepting_states;

		//N.stateSize = 1+ actual last state
		int new_state = result.stateSize + result.disposition;

		new_starting[result.stateSize] = true;
		new_accepting[result.stateSize] = positive? false : true;

		for (int i = 0; i < result.stateSize; i++) {
			if (result.starting[i]) {
				prev_start_states.push_back(i + result.disposition);
				new_starting[i] = false;
			}
			else {
				new_starting[i] = false;
			}

			if (result.accepting[i]) {
				prev_accepting_states.push_back(i + result.disposition);
				if (!positive) {
					new_accepting[i] = false;
				}
			}
			else {
				new_accepting[i] = false;
			}
		}

		for (auto s : prev_start_states) {
			result.trn.push_back({ new_state, "", s });
		}

		for (auto a : prev_accepting_states) {
			result.trn.push_back({ a, "", new_state });
		}

		delete[] result.starting;
		delete[] result.accepting;
		result.starting = new_starting;
		result.accepting = new_accepting;
		result.stateSize = result.stateSize + 1;

		return result;
	}
};

