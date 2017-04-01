#pragma once
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <string>
#include <iostream>

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

		for (const auto& s : states) {
			st_map[s.state] = k;
			starting[counter] = s.is_starting;
			accepting[counter] = s.is_final;
			counter++;
			k++;
		}

		for (auto&& t : trn) {
			t.source = st_map[t.source];
			t.dest = st_map[t.dest];
		}
	}

	bool _check(int state, vector<string>& input, int index) {
		/*	naive implementation need to check whole trn array to get the right transitions */
		if (index == input.size()) {
			if (accepting[state - disposition]) {
				return true;
			}
			for (const auto& t : trn) {
				//e-transitions
				if (t.source == state && t.word == "" && _check(t.dest, input, index)) {
					return true;
				}
			}

			return false;
		}

		for (const auto& t : trn) {
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

		for (auto&& t : trn) {
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

void tripleSort(vector<ATransition>& trn) {
	auto less_s = [](const ATransition& a, const ATransition& b)
	{
		return a.source < b.source;
	};

	auto less_w = [](const ATransition& a, const ATransition& b)
	{
		return a.word < b.word;
	};

	auto less_d = [](const ATransition& a, const ATransition& b)
	{
		return a.dest < b.dest;
	};

	//sort by source
	sort(trn.begin(), trn.end(), less_s);

	auto st_start = trn.begin();

	//partial sort by word
	for (auto it = trn.begin()+1; it != trn.end(); it++) {
		if (it->source != st_start->source) {
			sort(st_start, it, less_w);
			st_start = it;
		}
	}

	st_start = trn.begin();

	//partial sort by destination
	for (auto it = trn.begin() + 1; it != trn.end(); it++) {
		if (it->source == st_start->source &&  it->word != st_start->word) {
			sort(st_start, it, less_d);
			st_start = it;
		}

		if (it->source != st_start->source) {
			sort(st_start, it, less_d);
			st_start = it;
		}
	}
}

void deltaETClosureForward(int initial, int current, vector<ATransition>& trn, vector<ATransition>& additional, set<int>& closure) {
	//for every state we are traversing the whole transition table - TODO:// sort the table by e-transitions
	//and index the start of each state
	for (const auto& t : trn) {
		if (t.source == current && t.word == "" && closure.find(t.dest) == closure.end()) {
			closure.insert(t.dest);
			deltaETClosureForward(initial, t.dest, trn, additional, closure);
		}
		else if (t.source == current && t.word != "") {
			additional.push_back({ initial, t.word, t.dest });
		}
	}
}

void deltaETClosureBackward(int initial, int current, vector<ATransition>& trn, vector<ATransition>& additional, set<int>& closure) {
	//for every state we are traversing the whole transition table - TODO:// sort the table by e-transitions
	//and index the start of each state
	for (const auto& t : trn) {
		if (t.dest == current && t.word == "" && closure.find(t.source) == closure.end()) {
			closure.insert(t.source);
			deltaETClosureBackward(initial, t.source, trn, additional, closure);
		}
		else if (t.dest == current && t.word != "") {
			additional.push_back({ t.source, t.word, initial });
		}
	}
}

vector<ATransition> epsillonRemoval(int stateSize, int disposition, vector<ATransition> trn) {
	//O(5*nlogn + n*O(deltaETClosure))
	//it is not working properly
	vector<ATransition> result;
	set<int> closure;

	//sort by e-transitions and source

	for(int i = 0 ; i < stateSize; i++){
		closure.clear();
		//the forwards closure is state---(e)--->b---word--->a
		//resulting in state---word--->a
		deltaETClosureForward( i + disposition, i + disposition, trn, result, closure);
		//now we need the backward closure when we have a---word--->b---(e)--->state
		//resulting in a---word--->state
		closure.clear();
		deltaETClosureBackward(i + disposition, i + disposition, trn, result, closure);
	}

	tripleSort(result);

	auto equal = [](const ATransition& a, const ATransition& b)
	{
		return a.source == b.source && a.word == b.word && a.dest == b.dest;
	};

	for (auto it = result.begin() + 1; it != result.end(); it++) {
		if (equal(*it, *(it - 1))) {
			auto before = it-1;
			result.erase(it);
			it = before;
		}
	}

	return result;
}

void _recursiveStepUnreachable(vector<ATransition>& trn, bool *marked, int disposition) {
	for (const auto& t : trn) {
		if (!marked[t.source - disposition]) {
			marked[t.source - disposition] = true;
			_recursiveStepUnreachable(trn, marked, disposition);
		}
	}
}

void removeUnreachable(vector<ATransition>& trn, int stateSize, int disposition) {
	bool *marked = new bool[stateSize];
	for (int i = 0; i < stateSize; i++) {
		marked[i] = false;
	}

	for (const auto& t : trn) {
		if (!marked[t.source - disposition]) {
			marked[t.source - disposition] = true;
			_recursiveStepUnreachable(trn, marked, disposition);
		}
	}

	auto temp_it = trn.begin();
	for (auto it = trn.begin(); it != trn.end(); it++) {
		if (!marked[it->source - disposition]) {
			temp_it = it - 1;
			trn.erase(it);
			it = temp_it;
		}
	}
}




