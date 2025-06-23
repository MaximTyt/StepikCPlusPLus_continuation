//Напишите функцию map_reduce с использованием std::thread, которая принимает на вход:
//  1. итератор на начало последовательности,
//  2. итератор на конец последовательности,
//  3. унарный функтор, который можно применить к элементам последовательности,
//  4. бинарный функтор, который можно применить к результатам применения первого функтора,
//  5. количество потоков.
//Результат вызова map_reduce
//    auto res = map_reduce(p, q, f1, f2, num);
//  должен быть эквивалентен результату следующего кода :
//  
//    auto res = f1(*p);
//    while (++p != q)
//        res = f2(res, f1(*p));
//только выполнение этого кода должно быть разбито на num потоков.
// Для этого можно разбить диапазон[p, q) на num частей, выполнить вычисление для каждой части
// в отдельном потоке и объединить результаты из всех потоков.

#include <iostream>
#include <future>
#include <thread>
#include <list>
#include <vector>
template<class It, class F1, class F2>
auto map_reduce_async(It p, It q, F1 f1, F2 f2) -> decltype(f2(f1(*p), f1(*p)))
{
    auto res = f1(*p);
    while (++p != q)
        res = f2(res, f1(*p));
    return res;
}
template<class It, class F1, class F2>
auto map_reduce(It p, It q, F1 f1, F2 f2, size_t threads = 1) -> decltype(f2(f1(*p), f1(*p)))
{
    using TRes = decltype(f2(f1(*p), f1(*p)));
    int dist = std::distance(p, q) / threads;
    It next = p, prev;    
    std::vector<TRes> vecRes(threads);
    std::vector<std::thread> vecThread;
    for (int i = 0; i < threads; ++i) {
        prev = next;
        if (i == threads - 1) next = q;
        else std::advance(next, dist);
        vecThread.push_back(std::thread([=, &vecRes]() {vecRes[i] = map_reduce_async(prev, next, f1, f2); }));
    }
    for (auto& i : vecThread)
        i.join();
    auto result = vecRes[0];
    for (int i = 1; i < vecRes.size(); ++i)
        result = f2(result, vecRes[i]);
    return result;
}
int main()
{

    std::list<int> l = { 1,2,3,4,5,6,7,8,9,10 };
    // параллельное суммирование в 3 потока
    auto sum = map_reduce(l.begin(), l.end(),
        [](int i) {return i; },
        std::plus<int>(), 3);
    std::cout << "parallel sum with 3 threads:\n" << "sum = " << sum << std::endl;

    // проверка наличия чётных чисел в четыре потока
    auto has_even = map_reduce(l.begin(), l.end(),
        [](int i) {return i % 2 == 0; },
        std::logical_or<bool>(), 4);
    std::cout << "has even with 4 threads:\n" << "has_even = " << has_even << std::endl;

    std::list<int> l1 = { 1,3,3,5,5,7,7,9,9,9 };
    // проверка наличия чётных чисел в четыре потока
    auto has_even1 = map_reduce(l1.begin(), l1.end(),
        [](int i) {return i % 2 == 0; },
        std::logical_or<bool>(), 5);
    std::cout << "has even with 5 threads:\n" << "has_even1 = " << has_even1 << std::endl;
}
