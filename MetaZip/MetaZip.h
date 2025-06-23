#pragma once
#include <iostream>
#include <tuple>
template<int ... T>
struct IntList;

template<int H, int... T>
struct IntList<H, T...> {
    static int const Head = H;
    using Tail = IntList<T...>;
    static const int Length = 1 + sizeof...(T);
};
template<>
struct IntList<> {
    static const int Length = 0;
};

template<typename IL>
struct Length
{
    static int const value = 1 + Length<typename IL::Tail>::value;
};

template<>
struct Length<IntList<>>
{
    static const int value = 0;
};

//Напишите две метафункции для работы c IntList:
// IntCons позволяет увеличить список на один элемент — он добавляется в начало списка.
// Generate позволяет сгенерировать список длины N с числами от 0 до N - 1.

template<int H, typename IL>
struct IntCons;

template<int H, int... Types>
struct IntCons<H, IntList<Types...>>
{
    using type = IntList<H, Types...>;
};

template<typename TL1, typename TL2>
struct Concat;

template<int... Ts1, int... Ts2>
struct Concat<IntList<Ts1...>, IntList<Ts2...>>
{
    using type = IntList<Ts1..., Ts2...>;
};

template<int N>
struct Generate;

template<int N>
struct Generate
{
    using type = typename Concat<typename Generate<N - 1>::type, IntList<N - 1>>::type;
};

template<>
struct Generate<0>
{
    using type = IntList<>;
};