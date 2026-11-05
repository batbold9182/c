#include <iostream>
#include "../../inc/shape.h"
#include <vector>
int main(){  
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2));  
    shapes.push_back(std::make_unique<Rectangle>(3, 4));
    shapes.push_back(std::make_unique<Square>(5));
    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));

    for (const auto& s : shapes){
        std::cout << s->name() << ": area " << s->area() << ": perimeter" <<s->perimeter() << "\n";
    }
    return 0;
}
