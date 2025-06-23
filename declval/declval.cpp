// declval.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <utility>
#include <iostream>
template<class T>
void do_math() noexcept(
    noexcept(T(std::declval<T&>()))
    && noexcept(std::declval<T&>() = std::declval<T&>()) 
    && noexcept(std::declval<T>() + std::declval<T>())
    && noexcept(T(std::declval<T>()))
    && noexcept(std::declval<T&>() = std::declval<T>()))
{
}

//template<class T>
//void do_math() noexcept(noexcept(std::declval<T&>() = T(std::declval<T&>()) + std::declval<T>() + T(T(std::declval<T&>()))))
//{
//}

template<class T>
void test(bool ans)
{
    std::cout
        << noexcept(do_math<T>()) << " <-> " << ans
        << " Type: " << typeid(T).name()
        << std::endl;
    do_math<T>();
}

struct NoExceptAll
{
    NoExceptAll() = delete;
    NoExceptAll(NoExceptAll&&) noexcept {};
    NoExceptAll& operator=(const NoExceptAll&&) = delete;

    NoExceptAll(const NoExceptAll&) noexcept = default;
    ~NoExceptAll() noexcept = default;
    NoExceptAll& operator=(const NoExceptAll&) noexcept = default;
    NoExceptAll& operator=(NoExceptAll&&) noexcept {};
    NoExceptAll& operator+(const NoExceptAll&) noexcept {};
};

struct ThrowCopy
{
    ThrowCopy() = delete;
    ThrowCopy(ThrowCopy&&) noexcept {};
    ThrowCopy& operator=(const ThrowCopy&&) = delete;

    ThrowCopy(const ThrowCopy&) noexcept(false) {};
    ~ThrowCopy() noexcept = default;
    ThrowCopy& operator=(const ThrowCopy&) noexcept = default;
    ThrowCopy& operator=(ThrowCopy&&) noexcept {};
    ThrowCopy& operator+(const ThrowCopy&) noexcept {};
};

struct ThrowAssign
{
    ThrowAssign() = delete;
    ThrowAssign(ThrowAssign&&) noexcept {};
    ThrowAssign& operator=(const ThrowAssign&&) = delete;

    ThrowAssign(const ThrowAssign&) noexcept = default;
    ~ThrowAssign() noexcept = default;
    ThrowAssign& operator=(const ThrowAssign&) noexcept(false) {};
    ThrowAssign& operator=(ThrowAssign&&) noexcept {};
    ThrowAssign& operator+(const ThrowAssign&) noexcept {};
};
struct ThrowMove
{
    ThrowMove() = delete;
    ThrowMove(ThrowMove&&) noexcept(false) {};
    ThrowMove& operator=(const ThrowMove&&) = delete;

    ThrowMove(const ThrowMove&) noexcept = default;
    ~ThrowMove() noexcept = default;
    ThrowMove& operator=(const ThrowMove&) noexcept = default;
    ThrowMove& operator=(ThrowMove&&) noexcept {};
    ThrowMove& operator+(const ThrowMove&) noexcept {};
};
struct ThrowAssignMove
{
    ThrowAssignMove() = delete;
    ThrowAssignMove(ThrowAssignMove&&) noexcept {};
    ThrowAssignMove& operator=(const ThrowAssignMove&&) = delete;

    ThrowAssignMove(const ThrowAssignMove&) noexcept = default;
    ~ThrowAssignMove() noexcept = default;
    ThrowAssignMove& operator=(const ThrowAssignMove&) noexcept = default;
    ThrowAssignMove& operator=(ThrowAssignMove&&) noexcept(false) {};
    ThrowAssignMove& operator+(const ThrowAssignMove&) noexcept {};
};


struct ThrowPlus
{
    ThrowPlus() = delete;
    ThrowPlus(ThrowPlus&&) noexcept {};
    ThrowPlus& operator=(const ThrowPlus&&) = delete;

    ThrowPlus(const ThrowPlus&) noexcept = default;
    ~ThrowPlus() noexcept = default;
    ThrowPlus& operator=(const ThrowPlus&) noexcept = default;
    ThrowPlus& operator=(ThrowPlus&&) noexcept {};
    ThrowPlus& operator+(const ThrowPlus&) noexcept(false) {};
};

int main()
{
    std::cout << "Hello World!\n";
    bool b1 = noexcept(do_math<int>()); // true
    bool b2 = noexcept(do_math<std::string>()); // false
    std::cout << "b1=" << b1 << " should be 1\t" << " b2=" << b2 << " should be 0\n";

    test<int>(true);
    test<std::string>(false);
    test<NoExceptAll>(true);
    test<ThrowCopy>(false);
    test<ThrowAssign>(false);
    test<ThrowMove>(false);
    test<ThrowAssignMove>(false);
    test<ThrowPlus>(false);
    std::cout << "Bye!\n";
}