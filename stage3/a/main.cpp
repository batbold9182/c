#include <iostream>
#include "../../inc/shape.h"
#include <vector>
int main(){  
    std::vector<Shape*> shapes;
    shapes.push_back(new Circle(2));
    shapes.push_back(new Rectangle(3, 4));
    shapes.push_back(new Square(5));
    shapes.push_back(new Triangle(3, 4, 5));

    for (Shape* s : shapes){
        std::cout << s->name() << ": area " << s->area() << ": perimeter" <<s->perimeter() << "\n";
    }
    return 0;
}
