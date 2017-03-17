template<typename TRes, typename ...Types>
TRes Intercept(TRes(*func)(Types...args), Types...args) {
	return (*func)(args);
}

//
//template<typename TRes, typename CallingFunction, typename...Types>
//std::function<TRes(Types...)> Intercept(CallingFunction func, void(*before)(Types...), void(*after)(Types...)) {
//	//create function closure
//	return [=](Types...args)-> TRes {
//		//call the interceptor before function call
//		(*before)(args...);
//
//		//capture function execution result
//		auto res = (*func)(args...);
//
//		//call the interceptor after function call
//		(*after)(args...);
//
//		return res;
//	};
//}
//
////overloading Intercept function for cases where TRes is void
//template<typename...Types>
//std::function<void(Types...)> Intercept(void(*func)(Types...), void(*before)(Types...), void(*after)(Types...)) {
//	//create function closure
//	return [=](Types...args)-> void {
//		//call the interceptor before function call
//		(*before)(args...);
//
//		//make the function call
//		(*func)(args...);
//
//		//call the interceptor after function call
//		(*after)(args...);
//	};
//}
//
//
//
//int sum(float a, double b) {
//	return (int)a + b;
//}
//
//void greeting() {
//	std::cout << "Hello from the fancy world of Closures!\n";
//}
//
//void insertIntoHash(DynamicHashN& hash, int x) {
//	hash.insert(x);
//}
//
//template<typename...Types>
//void doNothing(Types...args) { }
//
//
//template<typename TRes, typename...Types>
//std::function<void(DynamicHashN&, int)> Wrap(TRes(*f)()) {
//	return [=](Types...args) mutable -> void {
//		(*f)();
//	};
//}