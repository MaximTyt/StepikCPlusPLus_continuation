// Напишите шаблонную функцию to_pair, которая принимает произвольный std::tuple и два индекса внутри и возвращает std::pair, 
// содержащий элементы переданного std::tuple с соответствующими индексами.
#include <iostream>
#include <tuple>

// принимает std::tuple произвольного размера
template<size_t i1, size_t i2, typename... Args>
auto to_pair(std::tuple<Args...> t) -> decltype(std::make_pair(std::get<i1>(t), std::get<i2>(t)))
{
    return std::make_pair(std::get<i1>(t), std::get<i2>(t));
}

int main()
{
    auto t = std::make_tuple(0, 3.5, "Hello");
    std::pair<double, char const*> p = to_pair<1, 2>(t);
}