// Напишите constexpr бинарные операторы +, - и * (скалярное произведение) для структуры Point.

#include <iostream>
// определение структуры Point уже подключено
 struct Point
{
    constexpr Point(double x, double y)
        : x(x), y(y)
    {}

    double x = 0;
    double y = 0;
};


// сложение
constexpr Point operator+(Point const& p1, Point const& p2)
{
    return Point(p1.x + p2.x, p1.y + p2.y);
}

// вычитание
constexpr Point operator-(Point const& p1, Point const& p2)
{
    return Point(p1.x - p2.x, p1.y - p2.y);
}

// скалярное произведение
constexpr double operator*(Point const& p1, Point const& p2)
{
    return p1.x * p2.x + p1.y * p2.y;
}
int main()
{
    constexpr size_t size = static_cast<size_t>(Point(2, 4) * Point(4, 3));
    constexpr int x = (Point(1, 1) + Point(2, 2)).x;
    constexpr int x2 = (Point(1, 1) - Point(2, 2)).x;
    int m[size];
    std::cout << "Hello World!\n";
}