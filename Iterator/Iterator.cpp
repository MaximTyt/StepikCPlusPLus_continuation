// Iterator.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <list>

int main()
{
    std::list<int> l = { 1,2,3 };
    l.insert(l.begin(), 4);
    l.insert(l.end(), 5);

    auto it = l.begin();
    ++it;
    ++it;

    l.insert(it, 6);

    auto rit1 = l.rbegin();
    ++rit1;
    ++rit1;
    l.insert(rit1.base(), 7);

    auto rit2 = l.rbegin();
    ++rit2;
    ++rit2;
    l.insert(rit2.base(), 8);
    for(auto i = l.begin(); i != l.end(); ++i)
        std::cout << *i << ' ';
    std::cout << std::endl;     
}

