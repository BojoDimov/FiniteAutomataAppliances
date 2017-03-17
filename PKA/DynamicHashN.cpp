#include "DynamicHashN.h"
#include "utility.h"
#include <iostream>



DynamicHashN::DynamicHashN()
{
	capacity = INITIAL_CAPACITY_DYNAMIC_NATURAL_NUMBERS_HASH;
	size = 0;
	data = new int[capacity];
	init(data, -1, capacity);
	p = find_mutually_prime(capacity);
}


DynamicHashN::~DynamicHashN()
{
	delete[] data;
}

int DynamicHashN::insert(int x)
{
	if ((float)size >= capacity * CAPACITY_TRESHOLD) {
		_resize();
	}

	int result = _insert(data, x, p, capacity);
	size++;
	return result;
}

int DynamicHashN::search(int x) const
{
	int i = h(x, p, capacity);
	while (data[i] != -1) {
		if (data[i] == x)
			return i;
		else {
			i += p % capacity;
		}
	}
	return -1;
}

void DynamicHashN::_resize()
{
	int new_capacity = 2 * capacity;
	int new_p = find_mutually_prime(new_capacity);
	int* new_data = new int[new_capacity];

	init(new_data, -1, new_capacity);

	for (int i = 0; i < capacity; i++) {
		if (data[i] != -1) {
			_insert(new_data, data[i], new_p, new_capacity);
		}
	}

	delete[] data;
	data = new_data;
	p = new_p;
	capacity = new_capacity;
}

int DynamicHashN::_insert(int * arr, int x, const int & p, const int & capacity)
{
	int i = h(x, p, capacity);
	while (arr[i] != -1) {
		i += p % capacity;
	}

	arr[i] = x;
	return i;
}

int DynamicHashN::h(int x, int p, int capacity) const
{
	return  p * x % capacity;
}

void DynamicHashN::print() const {
	for (int i = 0; i < capacity; i++) {
		std::cout << data[i] << std::endl;
	}
}