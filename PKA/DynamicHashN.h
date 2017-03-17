#pragma once
#define INITIAL_CAPACITY_DYNAMIC_NATURAL_NUMBERS_HASH 64
#define CAPACITY_TRESHOLD 0.9

/**	This hash is for natural numbers in the range from 0 to capacity -1 **/
class DynamicHashN
{
public:
	DynamicHashN();
	~DynamicHashN();

	int insert(int x);

	int search(int x) const;

	int h(int x, int p, int capacity) const;

	void print()const;

private:
	int* data;
	int capacity;
	int size;
	int p;

	void _resize();

	int _insert(int* arr, int x, const int& p, const int& capacity);

};

