// В стандартной библиотеке есть семейство функций to_string для преобразования чисел в строки.
// Однако обратное преобразование не такое удобное — для каждого числового типа есть своя 
// функция (например, strtoi для int). В данном задании вам предлагается написать шаблонную 
// функцию from_string, которая умеет преобразовывать строку в разные типы.Для реализации 
// from_string предлагается воспользоваться классом std::istringstream, который представляет 
// собой поток ввода из строки, т.е.для преобразования строки в тип T предлагается прочитать 
// значение типа T из потока при помощи оператора >> .В случае неудачного преобразования 
// функция должна бросать исключение bad_from_string, класс которого вам нужно реализовать 
// самостоятельно.
//
//#define NDEBUG
#include <iostream>
#include <string>
#include <sstream>
#include <exception>
#include <cassert>

// описание класса исключения bad_from_string
class bad_from_string : public std::exception 
{
    std::string str;
public:
    bad_from_string(std::string e = "bad_from_string") : str(e) {}
    const char* what() const noexcept {
        return str.c_str();
    }    
};

// функция from_string
template<class T>
T from_string(std::string const& s)
{
    T t;
    std::istringstream iss(s);
    iss >> std::noskipws >> t;
    if (iss.fail())
        throw bad_from_string("bad_from_string");
    iss.get();
    if (iss.good())
        throw bad_from_string("bad_from_string");
    return t;
}
int main()
{
    //case1
    //std::string s1(" 123");
    //int    a1 = from_string<int>(s1); // a1 = 123
    //assert(a1 == 123, "a1 not equil 123");    
    //double b1 = from_string<double>(s1); // b1 = 123.0
    //assert(b1 == 123.0, "b1 not equil 123.0");    
    //std::string c1 = from_string <std::string> (s1); // c1 = "123"
    //assert(c1 == "123", "c1 not equil '123'");    
    
    //case2
    //std::string s2("12.3");
    //int    a2 = from_string<int>(s2); // исключение
    //assert(a2 == 12.3, "a1 not equil 12.3");
    //double b2 = from_string<double>(s2); // b2 = 12.3
    //std::string c2 = from_string<std::string>(s2); // c2 = "12.3"

    //std::string s3("abc");
    //int    a3 = from_string<int>(s3); // исключение
    //double b3 = from_string<double>(s3); // исключение
    //std::string c3 = from_string<std::string>(s3); // c3 = "abc"

    from_string<char>(" "); // пробел
    from_string<std::string>(" "); // исключение
    from_string<std::string>(""); // исключение
    from_string<std::string>("a "); // исключение
    from_string<std::string>(" a"); // исключение
    from_string<std::string>("a b"); // исключение
}

