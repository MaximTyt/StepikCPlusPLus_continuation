// Напишите функцию apply, которая принимает некоторую функцию / функциональный объект, а так же аргументы для вызова этого объекта, и вызывает его, 
// используя perfect forwarding.

#include <iostream>
#include <utility> // для std::move и std::forward


// реализация функции apply
template<typename T, typename ...Args>
auto apply(T t, Args&&... args) -> decltype(t(std::forward<Args>(args)...))
{
    return t(std::forward<Args>(args)...);
}

int main()
{
    auto fun = [](std::string a, std::string const& b) { return a += b; };

    std::string s("world!");

    // s передаётся по lvalue ссылке,
    // а временный объект по rvalue ссылке 
    s = apply(fun, std::string("Hello, "), s);

    std::cout << s;
}