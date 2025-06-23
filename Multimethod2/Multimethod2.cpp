// Реализовать шаблонный класс Multimethod2, реализующий мультиметод от двух аргументов, который позволяет динамически добавлять новые реализации мультиметода для различных пар классов.
// Вам нужно будет реализовать три метода класса Multimethod2 :
// addImpl — добавляет реализацию мультиметода для двух типов, которые заданы через std::type_info.
// hasImpl — принимает два указателя и проверяет, есть ли реализация мультиметода для соответствующих типов.
// call — принимает два указателя и вызывает для них соответствующую реализацию.
// Реализация этих методов должна корректно обрабатывать ситуацию, когда мультиметод является коммутативным (т.е. если результат вызова мультиметода не зависит от порядка аргументов).

#include <iostream>
#include <typeinfo>
#include <typeindex>
#include <map>
#include <utility>
#include <functional>

// базовый класс фигуры (полиморфный)
struct Shape { 
    virtual ~Shape(){}
};

// прямоугольник
struct Rectangle : Shape {};

// треугольник
struct Triangle : Shape { };

// функция для проверки пересечения двух прямоугольников
bool is_intersect_r_r(Shape* a, Shape* b)
{
    std::cout << typeid(*a).name() << ' ' << typeid(*b).name() << std::endl;
    return true;
}

// функция для проверки пересечения прямоугольника и треугольника
bool is_intersect_r_t(Shape* a, Shape* b)
{
    std::cout << typeid(*a).name() << ' ' << typeid(*b).name() << std::endl;
    return true;
}

// Base - базовый класс иерархии
// Result - тип возвращаемого значения мультиметода
// Commutative - флаг, который показывает, что
// мультиметод коммутативный (т.е. f(x,y) = f(y,x)).
template<class Base, class Result, bool Commutative>
struct Multimethod2
{
    typedef std::type_index t_i;
    typedef std::function<Result(Base*, Base*)> func;
    typedef std::pair<const t_i, const t_i> my_key_type;
    std::map<my_key_type, func> map_f;
    // устанавливает реализацию мультиметода
    // для типов t1 и t2 заданных через typeid 
    // f - это функция или функциональный объект
    // принимающий два указателя на Base 
    // и возвращающий значение типа Result
    void addImpl(const t_i& t1, const t_i& t2, func f)
    {        
        map_f.emplace(std::make_pair(t1, t2), f);
    }

    // проверяет, есть ли реализация мультиметода
    // для типов объектов a и b
    bool hasImpl(Base* a, Base* b) const
    {
        t_i i1(typeid(*a)), i2(typeid(*b));
        bool res = false;        
        if (map_f.find(std::make_pair(i1, i2)) != map_f.end())
        {
            res = true;
        }
        if (Commutative) {
            if (!res && map_f.find(std::make_pair(i2, i1)) != map_f.end())
                res = true;
        }     
           
        //// возвращает true, если реализация есть
        //// если операция коммутативная, то нужно 
        //// проверить есть ли реализация для b и а 
        return res;
    }

    // Применяет мультиметод к объектам
    // по указателям a и b
    Result call(Base* a, Base* b) const
    {
        t_i i1(typeid(*a)), i2(typeid(*b));        
        if (map_f.find(std::make_pair(i1, i2)) != map_f.end())
        {
            return map_f.at(std::make_pair(i1, i2))(a, b);
        }
        else if (Commutative) {
            return map_f.at(std::make_pair(i2, i1))(b, a);
        }
        // возвращает результат применения реализации
        // мультиметода к a и b
        return 0;
    }
};

int main()
{
    // мультиметод для наследников Shape
    // возращающий bool и являющийся коммутативным 
    Multimethod2<Shape, bool, true> is_intersect;

    // добавляем реализацию мультиметода для двух прямоугольников
    is_intersect.addImpl(typeid(Rectangle), typeid(Rectangle), is_intersect_r_r);

    // добавляем реализацию мультиметода для прямоугольника и треугольника
    is_intersect.addImpl(typeid(Rectangle), typeid(Triangle), is_intersect_r_t);

    // создаём две фигуры    
    Shape* s1 = new Triangle();
    Shape* s2 = new Rectangle();

    // проверяем, что реализация для s1 и s2 есть
    if (is_intersect.hasImpl(s1, s2))
        std::cout << true << std::endl;
    // вызывается функция is_intersect_r_t(s2, s1)
    bool res = is_intersect.call(s1, s2);
    bool res1 = is_intersect.call(s2, s1);
    std::cout << res << ' ' << res1;
    // Замечание: is_intersect_r_t ожидает,
    // что первым аргументом будет прямоугольник
    // а вторым треугольник, а здесь аргументы
    // передаются в обратном порядке. 
    // ваша реализация должна самостоятельно 
    // об этом позаботиться    
    return 0;
}

