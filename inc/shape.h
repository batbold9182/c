#pragma once
#include <string>
class Shape {
  public:
    virtual double area() const = 0;        // = 0 → "pure virtual": each subclass MUST write it
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
    virtual ~Shape() {}                     // virtual destructor (explained below)
};
class Circle: public Shape {
    private:
        double r;
    public:
        Circle(double radius);
        double area() const override;
        double perimeter() const override;
        std::string name() const override { return "Circle"; }
};
class Rectangle: public Shape{
    private:
        double a,b;
    public:       
        Rectangle(double w,double h);
        double area() const override;
        double perimeter() const override;
        std::string name() const override { return "Rectangle"; }
};
class Triangle: public Shape{
    private:
        double a,b,c;
    public:
        Triangle(double first, double second,double third);  
        double area() const override;
        double perimeter() const override;
        std::string name() const override { return "Triangle"; }
        
};

class Square : public Rectangle{
    public:
        Square(double side):Rectangle(side,side){};
        std::string name() const override {return "Square";}
}; 