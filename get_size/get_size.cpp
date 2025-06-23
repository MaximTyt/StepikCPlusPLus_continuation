// Напишите шаблонную функцию get_size, которая принимает значение некоторого типа, у которого есть либо константный метод size()
// возвращающий size_t, либо поле size типа size_t, и возвращает соответствующее значение.
//

#include <iostream>
#include <vector>
#include <type_traits>


//template<class T>
//struct is_size_defined
//{
//    typedef char YES;
//    struct NO { YES m[2]; };
//    // обёртка, которая позволит проверить
//    // наличие метода size с заданой сигнатурой
//    template<class Z, size_t (Z::*)() const = &Z::size>
//    struct wrapper {};
//    template<class C>
//    static YES check(wrapper<C>* p);
//    template<class C>
//    static NO check(...);
//    static bool const value =
//        sizeof(YES) == sizeof(check<T>(0));
//};
//
//template<class T, class E = typename std::enable_if<is_size_defined<T>::value, T>::type>
//size_t get_size(const T& t)
//{
//    return t.size();
//}
//
//template<class T>
//size_t get_size(const T& t, typename std::enable_if<
//    is_size_defined<T>::value == false, T>::type* = 0)
//{
//    return t.size;
//}

template<class T, size_t(T::*)() const = &T::size>
size_t get_size(const T& t)
{
    return t.size();
}

template<class T, size_t(T::*) = &T::size>
size_t get_size(const T& t)
{
    return t.size;
}

int main()
{
    std::string s{ "Hello" };
    size_t s_size = get_size(s);   // 5, вызывается метод size()
    std::cout << "s_size == " << s_size << std::endl;
    struct Struct
    {
        size_t size = 0;
    };

    Struct x{ 10 };    
    size_t x_size = get_size(x);  // 10, читается поле size
    std::cout << "x_size == " << x_size << std::endl;
}

