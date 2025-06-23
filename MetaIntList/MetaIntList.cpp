// 1) По аналогии со списком типов определите список целых чисел (int) времени компиляции IntList. 
// Внутри списка должна быть константа Head, соответствующая числу в голове списка, и имя типа Tail, соответствующее хвосту списка.
// 2) Напишите метафункцию Length для вычисления длины списка IntList.

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
struct IntList<>{
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

// 3) Напишите две метафункции для работы c IntList:
// -IntCons позволяет увеличить список на один элемент — он добавляется в начало списка.
// -Generate позволяет сгенерировать список длины N с числами от 0 до N - 1.

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

// 4) Воспользуйтесь IntList и метафункцией Generate для того, чтобы научиться "раскрывать" кортежи.
// От вас потребуется написать функцию apply, которая принимает функтор и кортеж с аргументами для вызова этого функтора
// и вызывает функтор от этих аргументов.

template<typename F, typename T, int ...args>
auto _apply(F f, T t, IntList<args...>) -> decltype(f(std::get<args>(t)...)) {
    return f(std::get<args>(t)...);
}

template<typename F, typename ...Args>
auto apply(F f, std::tuple<Args...>const& t) -> decltype(_apply(f, t, typename Generate<sizeof...(Args)>::type()))
{    
    return _apply(f, t, typename Generate<sizeof...(Args)>::type());
}

// 5) Напишите метафункцию Zip (аналог std::transform), которая принимает два списка целых чисел одинаковой длины, а так же бинарную метафункцию,
//  и возвращает список, получившийся в результате поэлементного применения метафункции к соответствующим элементам исходных списков.


template<typename IL1, typename IL2, template<int, int> class Func>
struct Zip
{
    using type = typename IntCons<Func<IL1::Head, IL2::Head>::value, typename Zip< typename IL1::Tail, typename IL2::Tail, Func>::type>::type;
};

template<template<int, int> class F>
struct Zip<IntList<>, IntList<>, F>
{
    using type = IntList<>;
};

// бинарная метафункция
template<int a, int b>
struct Plus
{
    static int const value = a + b;
};
template<int a, int b>
struct Minus
{
    static int const value = a - b;
};
template<int a, int b>
struct Reverse
{
    static int const value = -a;
};
template<int a, int b>
struct Multiply
{
    static int const value = a * b;
};

// 6) Определите класс Quantity, которые хранит вещественное число и его размерность в системе СИ (размерность хранится как список IntList длины 7).
// Для этого класса нужно определить операторы сложения, вычитания, умножения и деления.

template<int m = 0, int kg = 0, int s = 0, int A = 0, int K = 0, int mol = 0, int cd = 0>
using Dimension = IntList<m, kg, s, A, K, mol, cd>;

template<class Dim>
class Quantity
{
    double _value = 0.0;
public:
    Quantity() = default;
    explicit Quantity(double x) :_value(x) {};    
    Quantity<Dim> operator+(const Quantity<Dim>& y) {
        return Quantity(_value + y._value);
    }
    Quantity<Dim> operator-(const Quantity<Dim>& y) {
        return Quantity(_value - y._value);
    }    
    Quantity<Dim> operator*(const double y) {
        return Quantity(_value * y);
    }    
    Quantity<Dim> operator/(const double y) {
        return Quantity(_value / y);
    } 

    template<class Dim1>
    Quantity<typename Zip<Dim, Dim1, Plus>::type> operator*(const Quantity<Dim1>& y) {
        return Quantity<typename Zip<Dim, Dim1, Plus>::type>(_value * y.value());
    }
    template<class Dim1>
    Quantity<typename Zip<Dim, Dim1, Minus>::type> operator/(const Quantity<Dim1>& y) {
        return Quantity<typename Zip<Dim, Dim1, Minus>::type>(_value / y.value());
    }    
    const double value() const { return _value; };
};

template<class Dim>
Quantity<typename Zip<Dim, Dim, Reverse>::type> operator/(const double y, const Quantity<Dim>& x) {
    return Quantity<typename Zip<Dim, Dim, Reverse>::type>(y / x.value());
}

template<class Dim>
Quantity<Dim> operator*(const double y, const Quantity<Dim>& x) {
    return Quantity<Dim>(y * x.value());
}


int main()
{
    using primes = IntList<2, 3, 5, 7, 11, 13>;
    using _primes = IntList<-1, -2, -3>;
    constexpr int head = primes::Head;
    using odd_primes = primes::Tail;    
    std::cout << "head == " << head << std::endl;    
    std::cout << "head->next == " << odd_primes::Head << std::endl;
    std::cout << "head->next->next == " << odd_primes::Tail::Head << std::endl;
    std::cout << "Length<primes> == " << Length<primes>::value << std::endl;
    std::cout << "primes::Length == " << primes::Length << std::endl;
    using _L2 = IntCons<1, primes>::type;
    using L3 = Generate<4>::type;
    using L4 = Concat<_primes, primes>::type;
    std::cout << "Generate<4>::type::Lenght == " << L3::Length << std::endl;

    auto f = [](int x, double y, double z) { return x + y + z; };
    auto t = std::make_tuple(30, 5.0, 1.6);  // std::tuple<int, double, double>
    auto res = apply(f, t);
    std::cout << "res == " << res << std::endl;


    // два списка одной длины
    using L1 = IntList<1, 2, 3, 4, 5>;
    using L2 = IntList<1, 3, 7, 7, 2>;

    // результат применения — список с поэлементными суммами
    using L5 = Zip<L1, L2, Plus>::type;  // IntList<2, 5, 10, 11, 7>
    using L6 = Zip<L1, L2, Multiply>::type; // IntList<1, 6, 21, 28, 10>


    using NumberQ = Quantity<Dimension<>>;          // число без размерности
    using LengthQ = Quantity<Dimension<1>>;          // метры
    using MassQ = Quantity<Dimension<0, 1>>;       // килограммы
    using TimeQ = Quantity<Dimension<0, 0, 1>>;    // секунды
    using FrequencyQ = Quantity<Dimension<0, 0, -1>>;    // секунда в -1 или частота
    using VelocityQ = Quantity<Dimension<1, 0, -1>>;   // метры в секунду
    using AccelQ = Quantity<Dimension<1, 0, -2>>;   // ускорение, метры в секунду в квадрате
    using ForceQ = Quantity<Dimension<1, 1, -2>>;   // сила в ньютонах
    using SquareQ = Quantity<Dimension<2, 0, 0>>;

    LengthQ   l{ 30000 };      // 30 км
    TimeQ     time{ 10 * 60 };    // 10 минут
    // вычисление скорости
    VelocityQ velocity = l / time;     // результат типа VelocityQ, 50 м/с
    std::cout << "velocity == " << velocity.value() << std::endl;

    AccelQ    a{ 9.8 };        // ускорение свободного падения
    MassQ     m{ 80 };         // 80 кг
    // сила притяжения, которая действует на тело массой 80 кг
    ForceQ    forse = m * a;     // результат типа ForceQ
    std::cout << "forse == " << forse.value() << std::endl;

    FrequencyQ freq = 1.0 / time;
    std::cout << "freq == " << freq.value() << std::endl;
    SquareQ   square = l * l;
    std::cout << "square(m^2) == " << square.value() / 1000.0 << std::endl;
    std::cout << "len == " << (l - l).value() << std::endl;    
    std::cout << "square(km^2) == " << (l / 1000.0 * l / 1000.0).value() << std::endl;
    std::cout << "len == " << (2.0 * l).value() << std::endl;
}

//#include <locale.h>
//int main() {
//    setlocale(LC_ALL, "RUS");
//    using NumberQ = Quantity<Dimension<>>;          // число без размерности
//    using LengthQ = Quantity<Dimension<1>>;          // метры
//    using MassQ = Quantity<Dimension<0, 1>>;       // килограммы
//    using TimeQ = Quantity<Dimension<0, 0, 1>>;    // секунды
//    using VelocityQ = Quantity<Dimension<1, 0, -1>>;   // метры в секунду
//    using AccelQ = Quantity<Dimension<1, 0, -2>>;   // ускорение, метры в секунду в квадрате
//    using ForceQ = Quantity<Dimension<1, 1, -2>>;   // сила в ньютонах
//
//
//    NumberQ d1{ 1.5 };
//    NumberQ d2{ 2 };
//    LengthQ l1{ 2.5 };
//    TimeQ t1{ 2 };
//
//    std::cout << "Сложение одинаковых типов" << std::endl;
//    std::cout << (d1 + d2).value() << " == 3.5" << std::endl;
//    //std::cout<< (l1+d2).value(); // error
//    std::cout << "Вычитание одинаковых типов" << std::endl;
//    std::cout << (d2 - d1).value() << " == 0.5" << std::endl;
//
//    std::cout << (d1 / 2).value() << " == 0.75" << std::endl;
//    std::cout << (d1 * 2).value() << " ==3.0" << std::endl;
//
//    std::cout << (3.0 / d1).value() << "==2.0" << std::endl;
//    std::cout << (3.0 * d1).value() << "==4.5" << std::endl;
//    std::cout << (l1 * d2).value() << "==5.0" << std::endl;
//
//
//
//    //test mult
//    std::cout << "Умножение" << std::endl;
//    std::cout << (typeid(l1 * d2) == typeid(LengthQ)) << "==1" << std::endl;
//    auto tmp = l1 * d2;
//    using TEST1 = Zip<Dimension<>, Dimension<1>, Plus>::type;
//    // test div
//    std::cout << "Деление" << std::endl;
//    std::cout << (typeid(l1 / d2) == typeid(LengthQ)) << "==1" << std::endl;
//    auto tmp2 = l1 / d2;
//    using TEST2 = Zip<Dimension<>, Dimension<1>, Plus>::type;
//
//    std::cout << "Деление double на время" << std::endl;
//    std::cout << (typeid(d1 / t1) == typeid(Quantity<Dimension<0, 0, -1>>)) << " == 1" << std::endl;
//
//
//    std::cout << "Скорость проверка типов" << std::endl;
//    // test l/t=speed
//    auto tmp3 = l1 / TimeQ(1);
//    std::cout << (typeid(tmp3) == typeid(VelocityQ)) << "==1" << std::endl;
//
//    // тесты из примера
//    std::cout << "тесты из примера" << std::endl;
//    std::cout << "Скорость" << std::endl;
//    LengthQ   l{ 30000 };      // 30 км
//    TimeQ     t{ 10 * 60 };    // 10 минут
//    VelocityQ v = l / t;
//    std::cout << (v.value() == 50.0) << "==1" << std::endl;
//
//    std::cout << "Сила" << std::endl;
//    AccelQ    a{ 9.8 };        // ускорение свободного падения
//    MassQ     m{ 80 };         // 80 кг
//    ForceQ    f = m * a;
//    std::cout << (f.value() == 784.0) << "==1" << std::endl;
//
//    // ускорение
//    std::cout << "ускорение" << std::endl;
//    auto vs2 = v / t1;
//    std::cout << (typeid(vs2) == typeid(AccelQ)) << "==1" << std::endl;
//    std::cout << (vs2.value() == 25) << "==1" << std::endl;
//
//    using Q1 = double;
//    using Q2 = NumberQ;
//
//    NumberQ nn(2.0);
//    NumberQ nn2 = nn;
//    NumberQ nn3 = nn / 3;
//    //NumberQ nn4=1.0d; // non explicit constr - error
//};