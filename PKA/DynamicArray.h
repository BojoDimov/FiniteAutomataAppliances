#include <iostream>
#define DA_INITIAL_CAP 256

struct dynamic_array {
	int size;
	int capacity;
	int* arr;

	dynamic_array() {
		capacity = DA_INITIAL_CAP;
		size = 0;
		arr = new int[capacity];
	}

	int operator[](int pos) {
		if (pos >= size) {
			throw "Index out of bounds";
		}

		return arr[pos];
	}

	~dynamic_array() {
		delete[] arr;
	}
};

void resize(dynamic_array&);
void insert(dynamic_array&, int);
void init(dynamic_array&, int);

void soft_reset(dynamic_array& da) {
	da.size = 0;
}

void hard_reset(dynamic_array& da) {
	da.size = 0;
	da.capacity = DA_INITIAL_CAP;
	delete[] da.arr;
	da.arr = new int[DA_INITIAL_CAP];
}

void init(dynamic_array& arr, int x) {
	for (int i = 0; i < arr.capacity; i++) {
		arr.arr[i] = x;
	}
}

void insert(dynamic_array& data, int x) {
	if (data.size == data.capacity) {
		resize(data);
	}

	data.arr[data.size] = x;
	data.size++;
}

void resize(dynamic_array& data) {
	//std::cout << "resizing array: " << data.arr << std::endl << data.capacity << " - old capacity\n";
	int new_capacity = 2 * data.capacity;
	int* new_arr = new int[new_capacity];
	for (int i = 0; i < data.size; i++) {
		new_arr[i] = data.arr[i];
	}
	delete[] data.arr;
	data.arr = new_arr;
	data.capacity = new_capacity;
}

void print(const dynamic_array& data) {
	for (int i = 0; i < data.size; i++) {
		std::cout << data.arr[i] << std::endl;
	}
}