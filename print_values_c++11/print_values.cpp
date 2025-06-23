// По аналогии с функцией printf с предыдущего степа напишите функцию print_values с переменным числом аргументов, 
// которая для каждого аргумента выводит его тип и значение в поток std::ostream, который ей передан в качестве первого аргумента. 
// Функция должна работать с произвольным числом аргументов.

#include <iostream>
#include <ostream>
#include <typeinfo>

void print_values(std::ostream& os) {}

// принимает произвольное число аргументов
template<typename T, typename... Args>
void print_values(std::ostream& os, T value, Args... args)
{
    os << typeid(value).name() << ": " << value << '\n';
    print_values(os, args...);
}

int main()
{
    print_values(std::cout, 0, 3.5, "Hello", true);
}