//
//NDFSA ndfsa(
//{ 0,1,2,3,4,5 },
//{ false,true, false, false, false, false },
//{ false, false, false, false, true, true },
//{
//	{},
//	{ { 'a',{ 2,3,5 } } },
//	{ { 'b',{ 3 } },{ 'c',{ 4 } } },
//	{},
//	{},
//	{}
//});
//
//NDFSA_Delta_Split ndfsaSplit(
//{ 1,2,3,4 },
//{ true,true, false, false },
//{ false, false, true, false },
//{
//	{ { 'm',{ 2 } } },
//	{ { 'a',{ 3 } } },
//	{ { 'a',{ 2,3 } },{ 'm',{ 2 } } },
//	{ { 'm',{ 2 } } }
//});
//
//NDFSA_Simple simple({
//	{ 5,{ false, true } },
//	{ 2,{ false, false } },
//	{ 3,{ false, false } },
//	{ 1,{ true, false } },
//	{ 0,{ true, false } }
//}, {
//	{ 0, 'd', 1 },
//	{ 0, 'b', 2 },
//	{ 0, 'b', 3 },
//	{ 1, 'c', 3 },
//	{ 0, 'a', 5 },
//	{ 1, 'a', 5 }
//});
//
////bool st1= ndfsa.traverse("ac");
//bool st2 = ndfsaSplit.isInLanguage("mamamaaaaamamm");
//
//class TarjanTable {
//private:
//	dynamic_array state_start_index;
//	dynamic_array transitions_dest;
//	dynamic_array transitions_source;
//	int displacement;
//	int alphabet_size;
//
//	int canPlaceAtPos(int * local_trans_symbols, int size, int pos) {
//		//we know size < transitions.size
//		int i = 0;
//
//		//we are checking for each symbol if the position in transitions is not taken
//		//local_trans_symbols[i] + pos < transitions_source.size if we have only symbols this is not a problem
//		while (i < size) {
//			if (transitions_source[pos + local_trans_symbols[i]] == -1) {
//				i++;
//			}
//			else {
//				return 0;
//			}
//		}
//
//		return 1;
//	}
//
//	int findFreePost(int * local_trans_symbols, int size) {
//		int pos = transitions_source.size - displacement * alphabet_size;
//
//		if (pos < 0) {
//			pos = 0;
//		}
//
//		while (pos + size >= transitions_source.size) {
//			resize(transitions_dest);
//			resize(transitions_source);
//		}
//
//		while (!canPlaceAtPos(local_trans_symbols, size, pos)) {
//			pos++;
//			//TODO: change 'z' to alphabet_size becuase every symbol should be mapped 
//			//to its position in the alphabet not its value when casted to int
//			if (pos + 'z' >= transitions_source.size) {
//				resize(transitions_dest);
//				resize(transitions_source);
//			}
//		}
//		return pos;
//	}
//
//public:
//	TarjanTable() {
//		init(state_start_index, -1);
//		init(transitions_source, -1);
//		displacement = 4;
//		alphabet_size = 26;
//	}
//
//	~TarjanTable() {
//
//	}
//
//	int next(int source, int symbol) {
//		int index = state_start_index[source] + symbol;
//		if (index < transitions_dest.size && transitions_source[index] == source) {
//			return transitions_dest[index];
//		}
//
//		return -1;
//	}
//
//	//states must be sorted so that for each source all the transitions are contiguous
//	void construct(int *source, int *ch, int *dest, int size) {
//		dynamic_array current_dest;
//		dynamic_array current_symbol;
//		int current = 0;
//		for (int i = 0; i < size; i++) {
//			current = source[i];
//			while (current == source[i]) {
//				//collect states with the same source
//				insert(current_dest, dest[i]);
//				insert(current_symbol, ch[i]);
//				i++;
//			}
//			//decrement i because last time i was incremented current was not equal to source[i]
//			i--;//NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO
//
//				//calculate state starting index
//			int pos = findFreePost(current_symbol.arr, current_symbol.size);
//
//			//this is dangerous becuase current might be larger than the capacity of the arr
//			state_start_index.arr[current] = pos;
//
//			//insert the destination and source in transitions table
//			for (int j = 0; j < current_symbol.size; j++) {
//				transitions_source.arr[pos + current_symbol[j]] = current;
//				transitions_dest.arr[pos + current_symbol[j]] = current_dest[j];
//			}
//
//			//reset the temp containers
//			soft_reset(current_dest);
//			soft_reset(current_symbol);
//		}
//	}
//};
//
////this is when we have Delta-function split between states
//struct State {
//	bool is_starting;
//	bool is_final;
//	map<int, vector<int>> Delta;
//};
//
//class NDFSA_Delta_Split {
//	map<int, State> Q;
//
//	bool traverse(const State& state, const string& word, int startIndex)const {
//		if (startIndex == word.size()) {
//			return state.is_final;
//		}
//
//		auto destinations = state.Delta.find(word[startIndex]);
//		if (destinations == state.Delta.end()) {
//			return false;
//		}
//
//		for (auto& s : destinations->second) {
//			if (traverse(Q.find(s)->second, word, startIndex + 1)) {
//				return true;
//			}
//		}
//
//		return false;
//	}
//public:
//	NDFSA_Delta_Split(
//		vector<int> states,
//		vector<bool> start,
//		vector<bool> finals,
//		vector<map<int, vector<int>>> transitions
//	) {
//		for (int i = 0; i < size(states); i++) {
//			Q[states[i]] = { start[i], finals[i], transitions[i] };
//		}
//	}
//
//	bool isInLanguage(const string& word)const {
//		for (auto it = Q.cbegin(); it != Q.cend(); it++) {
//			if ((*it).second.is_starting && traverse((*it).second, word, 0)) {
//				return true;
//			}
//		}
//
//		return false;
//	}
//};
//
//class NDFSA {
//	vector<int> Q;	//this maybe should be std::map<int, int> because we want to have various states but we cant afford to use their vaulue as label
//	vector<bool> S;
//	vector<bool> F;
//	vector<map<int, vector<int>>> Delta;
//
//	bool traverse(int state, const string& word, int begin) {
//		if (begin == size(word)) {
//			return F[state];
//		}
//
//		//auto sth = Delta[state][word[begin]];
//		//there might be a little problem if there is no such transition with the speciffic word
//		for (auto s : Delta[state][word[begin]]) {
//			if (traverse(s, word, begin + 1)) {
//				return true;
//			}
//		}
//
//		return false;
//	}
//
//public:
//	NDFSA(
//		vector<int> states,
//		vector<bool> start,
//		vector<bool> finals,
//		vector<map<int, vector<int>>> transitions
//	) {
//		for (int i = 0; i < size(states); i++) {
//			Q.push_back(states[i]);
//		}
//		S = start;
//		F = finals;
//		Delta = transitions;
//	}
//
//	bool isInLanguage(const string& word) {
//		for (int i = 0; i < Q.size(); i++) {
//			if (S[i]) {
//				if (traverse(i, word, 0)) {
//					return true;
//				}
//			}
//		}
//
//		return false;
//	}
//};
//
//struct SimpleState {
//	bool is_starting;
//	bool is_final;
//};
//
//struct AutomataState {
//	bool is_starting;
//	bool is_final;
//	int index;
//};
//
//struct Transition {
//	int source;
//	int symbol;
//	int destination;
//};
//
//bool lexCmp(const Transition& a, const Transition& b) {
//	return a.symbol < b.symbol;
//}
//
//bool stateCmp(const Transition& a, const Transition& b) {
//	return a.source < b.source;
//}
//
//class NDFSA_Simple {
//public:
//	vector<Transition> transitions;
//	vector<AutomataState> states;//use map here?
//
//
//public:
//	NDFSA_Simple(map<int, SimpleState> st, vector<Transition> trn)
//		: transitions(trn)
//	{
//		sort(transitions.begin(), transitions.end(), stateCmp);
//		auto currentState = transitions[0].source;
//		set<int> insertedStates;
//		states.push_back({ st[currentState].is_starting, st[currentState].is_final, 0 });
//		insertedStates.insert(currentState);
//
//		auto it_start = transitions.begin();
//		int index = 0;
//
//		for (auto it = transitions.begin(); it != transitions.end(); it++, index++) {
//			if (it_start->source != it->source) {
//				currentState = it->source;
//				states.push_back({ st[currentState].is_starting, st[currentState].is_final, index });
//				insertedStates.insert(currentState);
//				sort(it_start, it, lexCmp);
//				it_start = it;
//			}
//		}
//
//		//inserting those states which does not have entry in the transition table
//		for (auto it = st.begin(); it != st.end(); it++) {
//			if (insertedStates.find(it->first) == insertedStates.cend()) {
//				//may aswell consider this state cant be starting
//				states.push_back({ it->second.is_starting,it->second.is_final,-1 });
//			}
//		}
//	}
//};
//
//void determinize(const NDFSA_Simple& N) {
//	set<set<int>> Q0;
//	set<int> startSet;
//	for (int i = 0; i < N.states.size(); i++) {
//		if (N.states[i].is_starting) {
//			startSet.insert(N.transitions[N.states[i].index].source);
//		}
//	}
//}