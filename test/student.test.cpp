#include "../inc/student.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cmath>

int passed = 0;
int failed = 0;


void check(const std::string& name, double actual, double expected) {
    if (std::fabs(actual - expected) < 1e-9) {
        passed++;
    } else {
        failed++;
        std::cout << "FAIL: " << name << "  expected " << expected
                  << " but got " << actual << "\n";
    }
}
template <typename Func>
void checkThrows(const std::string& name, Func f) {
    try {
        f();
        failed++;
        std::cout << "FAIL: " << name << "  expected an exception, none thrown\n";
    } catch (const std::invalid_argument&) {
        passed++;
    }
}
int main(){
    Student student;

    std::vector<double> scores = {100,90,60,100};
    std::vector<double> cur = {90,0.5,63};
    std::vector<double> neg = {30.1};
    std::vector<double> mm = {72, 95, 60, 88};
    std::vector<double> empty;

    double bonus = 20;
    //findScore
    double* p = student.findScore(scores,90); 
    check("findScore found", p != nullptr, 1);
    check("findScore right element", p == &scores[1], 1); 

    *p = 99;
    check("findScore edits vector", scores[1], 99);     
    check("findScore missing", student.findScore(scores, 42) == nullptr, 1);
    //average
    check("Average whole", student.average({90,80,55}),75);
    check("Average zeros", student.average({0,0,0}),0);
    check("Average float", student.average({90.5,80.3,55.1}),75.3);

    //findMinMax
    double min = -1, max = -1;
    student.findMinMax(mm, min, max);
    check("findMinMax min", min, 60);   
    check("findMinMax max", max, 95);
    checkThrows("findMinMax empty", [&] { student.findMinMax(empty, min, max); });

    //curveScores
    student.curveScores(cur,bonus);
    check("curve max", cur[0],100);
    check("curvefloat", cur[1],20.5);
    student.curveScores(neg,-80);
    check("curve negative",neg[0],0);

    //letterGrade
    check("A grade", student.letterGrade(95), 'A');
    check("B grade", student.letterGrade(85), 'B');
    check("C grade", student.letterGrade(75), 'C');
    check("D grade", student.letterGrade(65), 'D');
    check("F grade", student.letterGrade(55), 'F');
    check("90 is A",    student.letterGrade(90), 'A');
    check("89.9 is B",  student.letterGrade(89.9), 'B');
    check("60 is D",    student.letterGrade(60), 'D');
    check("59.9 is F",  student.letterGrade(59.9), 'F');

    checkThrows("Invalid negative score", [&] { student.letterGrade(-5); });
    checkThrows("Invalid score over 100", [&] { student.letterGrade(105); });

    std::cout << "Passed: " << passed << ", Failed: " << failed << "\n";
    return failed == 0 ? 0 : 1;
}