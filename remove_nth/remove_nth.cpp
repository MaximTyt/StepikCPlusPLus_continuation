// Напишите алгоритм remove_nth, который удаляет элемент из последовательности по номеру этого элемента.

#include <iostream>
#include <algorithm>
#include <vector>

// алгоритм должен работать с forward итераторами
// и возвращать итератор, который потом будет передан
// в метод erase соответствующего контейнера

struct ElementN
{
    explicit ElementN(size_t n)
        : n(n), pi(new size_t(0))
    {}

    template<class T>
    bool operator()(T const& t) { return ((*pi)++ == n); }

    size_t n;
    std::shared_ptr<size_t> pi;
};

template<class FwdIt>
FwdIt remove_nth(FwdIt p, FwdIt q, size_t n)
{
    /*if (n % 3 == 0) {
        auto pred = ElementN(n);
        FwdIt s = find_if(p, q, pred);
        if (s == q)
            return q;
        FwdIt out = s;
        s++;
        return std::remove_copy_if(s, q, out, pred);
    }*/
    if (n % 2 == 0)
        return std::remove_if(p, q, ElementN(n));
    else
        return std::remove_if(p, q, [&n](typename FwdIt::value_type) {return !(n--); });
}
int main()
{
    std::vector<int> v = { 0,1,2,3,4,5,6,7,8,9,10 };
    v.erase(remove_nth(v.begin(), v.end(), 0), v.end());
    // теперь в v = {0,1,2,3,4,6,7,8,9,10};
    for(int i: v)
        std::cout << i << ' ';
    return 0;
}