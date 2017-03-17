#include "utility.h"

int find_mutually_prime(const int & number)
{
	int p = 5;
	while (!is_prime(p) || nod(number, p) != 1) {
		p++;
	}

	return p;
}

int nod(int a, int b)
{
	while (a != b) {
		if (a > b) {
			a = a - b;
		}
		else {
			b = b - a;
		}
	}
	return a;
}

bool is_prime(const int & p)
{
	for (int i = 2; i < p; i++) {
		if (p % i == 0) {
			return false;
		}
	}
	return true;
}
