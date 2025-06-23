// Напишите метафункцию Zip (аналог std::transform), которая принимает два списка целых чисел одинаковой длины, а так же бинарную метафункцию,
//  и возвращает список, получившийся в результате поэлементного применения метафункции к соответствующим элементам исходных списков.
//

#include "MetaZip.h"

template<typename, typename, template<int, int> class Func>
struct Zip;

template<int... Ts1, int... Ts2, template<int, int> class Func>
struct Zip<IntList<Ts1...>, IntList<Ts2...>, Func>
{
    //using type = typename IntCons<Func<IntList<Ts1...>::Head, IntList<Ts2...>::Head>::value, typename Zip<typename IntList<Ts1...>::Tail, typename IntList<Ts2...>::Tail, Func>::type>::type;
    using type = IntList<Func<Ts1, Ts2>::value...>;
};

template<template<int, int> class F>
struct Zip<IntList<>, IntList<>, F>
{
    using type = IntList<>;
};

//template<typename IL1, typename IL2, template<int, int> class Func>
//struct Zip
//{
//    using type = typename IntCons<Func<IL1::Head, IL2::Head>::value, typename Zip< typename IL1::Tail, typename IL2::Tail, Func>::type>::type;
//};
//
//template<template<int, int> class F>
//struct Zip<IntList<>, IntList<>, F>
//{
//    using type = IntList<>;
//};

// бинарная метафункция
template<int a, int b>
struct Plus
{
    static int const value = a + b;
};
template<int a, int b>
struct Multiply
{
    static int const value = a * b;
};

int main()
{
    // два списка одной длины
    using L1 = IntList<1, 2, 3, 4, 5>;
    using L2 = IntList<1, 3, 7, 7, 2>;

    // результат применения — список с поэлементными суммами
    using L3 = Zip<L1, L2, Plus>::type;  // IntList<2, 5, 10, 11, 7>
    using L4 = Zip<L1, L2, Multiply>::type; // IntList<1, 6, 21, 28, 10>
}