#include "test.helper.h"
#include "../inc/shape.h"

const double PI = 3.14159265358979;

int main(){
    Square sq(2);
    const Shape& s = sq;  
    
    check("check square area",s.area(), 4);
    check("check square perimeter",s.perimeter(), 8);
    Triangle tr(3,4,5);
    const Shape& t = tr;

    check("check triangle area", t.area(), 6);
    check("check triangle perimeter", t.perimeter(), 12);
    checkThrows("triangle impossible", [&]{ Triangle(1, 2, 10); });
    checkThrows("triangle flat",       [&]{ Triangle(1, 2, 3); });   // boundary: a + b == c
    checkThrows("triangle zero side",  [&]{ Triangle(0, 4, 5); });
    checkThrows("triangle negative",   [&]{ Triangle(3, -4, 5); });
    Circle c(5);
    const Shape& circle = c;

    check("check circle area", circle.area(), 25*PI) ;
    check("check circle perimeter", circle.perimeter(), 10*PI);
    checkThrows("invalid r",[&]{Circle(-3);});

    Rectangle rec(4,5);
    const Shape& r = rec;
    
    check("check rectangle area", r.area(),20);
    check("check rectangle perimeter", r.perimeter(),18);
    checkThrows("invalid numbers",[&]{Rectangle(-3,-4);});
    checkThrows("rect negative width",  [&]{ Rectangle(-3, 4); });
    checkThrows("rect negative height", [&]{ Rectangle(3, -4); });   // ← catches the && bug
    checkThrows("rect zero width",      [&]{ Rectangle(0, 5); });    // boundary: 0
    
    
    std::cout << "Passed: " << passed << ", Failed: " << failed << "\n";
    return report();
    return failed == 0 ? 0 : 1;
}