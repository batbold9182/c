#include <iostream>
#include "../../inc/shape.h"
#include <cmath>
#include <stdexcept>

const double PI = 3.14159265358979;

Circle::Circle(double radius) : r(radius) {
    if (r <= 0) throw std::invalid_argument("Circle: radius must be positive");
}
double Circle::area() const { return PI * r * r; }
double Circle::perimeter() const { return 2 * PI * r; }


Rectangle::Rectangle(double w, double h) : a(w) , b(h){
    if (a <= 0 || b <= 0) throw std::invalid_argument("Rectangle: sides must be positive");
}
double Rectangle::area() const {return a*b;}
double Rectangle::perimeter() const {return (a+b)*2;}


Triangle::Triangle(double first , double second , double third) : a(first) , b(second) ,c(third){
    if (first <= 0 || second <= 0 || third <=0 ) throw std::invalid_argument("Triangle: sides must be positive");
    if (first+second <= third || first + third <= second || second + third <= first){
        throw std::invalid_argument("Not a valid triangle");
    }
}
double Triangle::area() const {
    double s = (a+b+c)/2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));}
double Triangle::perimeter() const {return a+b+c;}


