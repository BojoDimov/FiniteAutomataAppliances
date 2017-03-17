#pragma once
template<typename TRes, typename ...Types>
TRes Intercept(TRes(*func)(Types...args), Types...args);
