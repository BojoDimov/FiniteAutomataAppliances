int find_mutually_prime(const int& number);

int nod(int a, int b);

bool is_prime(const int& p);

template<typename T1>
void init(T1* arr, T1 element, const int& size);

template<typename T1>
inline void init(T1 * arr, T1 element, const int & size)
{
	for (int i = 0; i < size; i++) {
		arr[i] = element;
	}
}
