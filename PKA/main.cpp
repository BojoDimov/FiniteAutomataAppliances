#include <iostream>
#include "DynamicArray.h"

class TarjanTable {
private:
	dynamic_array state_start_index;
	dynamic_array transitions_dest;
	dynamic_array transitions_source;
	int displacement;
	int alphabet_size;

	int canPlaceAtPos(int * local_trans_symbols, int size, int pos) {
		//we know size < transitions.size
		int i = 0;

		//we are checking for each symbol if the position in transitions is not taken
		//local_trans_symbols[i] + pos < transitions_source.size if we have only symbols this is not a problem
		while (i < size) {
			if (transitions_source[pos + local_trans_symbols[i]] == -1) {
				i++;
			}
			else {
				return 0;
			}
		}

		return 1;
	}

	int findFreePost(int * local_trans_symbols, int size) {
		int pos = transitions_source.size - displacement * alphabet_size;

		if (pos < 0) {
			pos = 0;
		}

		while (pos + size >= transitions_source.size) {
			resize(transitions_dest);
			resize(transitions_source);
		}

		while (!canPlaceAtPos(local_trans_symbols, size, pos)) {
			pos++;
			//TODO: change 'z' to alphabet_size becuase every symbol should be mapped 
			//to its position in the alphabet not its value when casted to int
			if (pos + 'z' >= transitions_source.size) {
				resize(transitions_dest);
				resize(transitions_source);
			}
		}
		return pos;
	}

public:
	TarjanTable() {
		init(state_start_index, -1);
		init(transitions_source, -1);
		displacement = 4;
		alphabet_size = 26;
	}

	~TarjanTable() {

	}

	int next(int source, int symbol) {
		int index = state_start_index[source]+ symbol;
		if (index < transitions_dest.size && transitions_source[index] == source) {
			return transitions_dest[index];
		}

		return -1;
	}

	//states must be sorted so that for each source all the transitions are contiguous
	void construct(int *source, int *ch, int *dest, int size) {
		dynamic_array current_dest;
		dynamic_array current_symbol;
		int current = 0;
		for (int i = 0; i < size; i++) {
			current = source[i];
			while (current == source[i]) {
				//collect states with the same source
				insert(current_dest, dest[i]);
				insert(current_symbol, ch[i]);
				i++;
			}
			//decrement i because last time i was incremented current was not equal to source[i]
			i--;
			
			//calculate state starting index
			int pos = findFreePost(current_symbol.arr, current_symbol.size);

			//this is dangerous becuase current might be larger than the capacity of the arr
			state_start_index.arr[current] = pos;

			//insert the destination and source in transitions table
			for (int j = 0; j < current_symbol.size; j++) {
				transitions_source.arr[pos + current_symbol[j]] = current;
				transitions_dest.arr[pos + current_symbol[j]] = current_dest[j];
			}

			//reset the temp containers
			soft_reset(current_dest);
			soft_reset(current_symbol);
		}
	}
};

int main() {
	TarjanTable transition_table;
	transition_table.construct(
		new int[4]{	0,	0,	0,	1 }, 
		new int[4]{	'a','b','c','a'}, 
		new int[4]{ 0,	1,	1,	1}, 
		4
	);
	return 0;
}