// Напишите шаблонную функцию count_permutations, которая принимает некоторую последовательность
// и вычисляет количество перестановок этой последовательности (равные последовательности считаются
// одной перестановкой), в которых нет подряд идущих одинаковых элементов.
//
#include <iostream>
#include <array>
#include <deque>
#include <vector>
#include <list>
#include <algorithm>
#include <string>
#include <assert.h>

template<class Iterator>
size_t count_permutations(Iterator p, Iterator q)
{
    if (p == q) return 1;
    using T = typename std::iterator_traits<Iterator>::value_type;
    std::vector<T> v(p, q);
    size_t count = 0;    
    std::sort(v.begin(), v.end());
    do
    {        
        if (std::adjacent_find(v.begin(), v.end()) == v.end())
            ++count;
    } while (std::next_permutation(v.begin(), v.end()));
    return count;
}

template<class Cntr>
void print(Cntr&& c, std::string title = "")
{
    std::cout << title << ": ";
    for (auto& v : c)
        std::cout << v << ' ';
    std::cout << '\t' << typeid(c).name() << std::endl;
}

template<class Cntr>
void test(Cntr&& c, size_t ans, std::string title = "")
{
    print(c, title);
    auto ret = count_permutations(c.begin(), c.end());
    std::cout << "\t return: " << ret << "\t correct: " << ans << std::endl;
    assert(ret == ans);
}

template<class CntrNums>
void run_test_with_num()
{
    test(CntrNums({ 1, 2, 3 }), 6, "default 1");
    test(CntrNums({ 3, 1, 2 }), 6, "default 1 - unsorted");
    test(CntrNums({ 1, 2, 3, 4, 4 }), 36, "default 2");
    test(CntrNums({ 1, 4, 3, 2, 4 }), 36, "default 2 - unsorted");
    test(CntrNums({ 5, 3, 5 }), 1, "2 same + 1");
    test(CntrNums({ 5, 5, 3, 5 }), 0, "3 same + 1");
    test(CntrNums({ 5, 5, 5 }), 0, "3 same");
    test(CntrNums({ 5, 5 }), 0, "2 same");
    test(CntrNums({ 5 }), 1, "single");
    test(CntrNums({}), 1, "empty");
}

void test_arrays()
{
    std::array<int*, 3> pa1 = { new int(1), new int(2), new int(3) };
    test(pa1, 6, "array of pointers");

    std::array<int, 3> a1 = { 1,2,3 };
    test(a1, 6, "default array");

    /*const std::array<int, 3> ca1 = a1;
    test(ca1, 6, "default const array");*/

    std::array<int, 5> a2 = { 1,2,3,4,4 };
    test(a2, 36, "default array");
}

int main()
{
    std::cout << "Hello World!\n";
    run_test_with_num<std::vector<int>>();
    run_test_with_num<std::deque<int>>();
    run_test_with_num<std::list<int>>();

    test_arrays();
    std::cout << "Bye!\n";
}


//int main()
//{
//    const std::array<int, 3> a1 = { 3,1,2 };
//    size_t c1 = count_permutations(a1.begin(), a1.end()); // 6
//    std::cout << c1 << std::endl;
//    std::array<int, 5> a2 = { 1,2,3,4,4 };
//    size_t c2 = count_permutations(a2.begin(), a2.end());// 36
//    std::cout << c2 << std::endl;
//}

