// MyError.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <stdexcept>

extern bool X = false;

struct MyError : std::runtime_error
{
    MyError(char const* s)
        : std::runtime_error(s)
    {
        if (X)
            throw std::logic_error("Logic error");
    }
};

void foo()
{
    throw MyError("My error");
}
int main()
{
    try {        
        foo();
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    std::cout << "Hello World!\n";
}

//Если при вызове foo() X = true, то за пределы функции foo будет выброшено исключение std::logic_error.
//Если при вызове foo() X = false, то за пределы функции foo будет выброшено исключение MyError.
//Если при вызове foo() X = true, то будет вызван деструктор std::runtime_error.