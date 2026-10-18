#include "test.helper.h"
#include "../inc/employee.h"
#include <stdexcept>

int main(){
    HourlyEmployee h("hourly",160,20);
    HourlyEmployee edge("edge",161,20);
    HourlyEmployee bat("bat",0,30);
    check("Salary pay",h.pay(),3200);
    check("Salary edge",edge.pay(),3230);
    checkThrows("Negative hours" , [&] {HourlyEmployee{"bat",-150,20};});
    bat.setHours(180);
    check("hourly after setHours(180)", bat.pay(), 5700);
    checkStr("Hourly type",h.type(),"Hourly");
    checkStr("Name check",h.getName(),"hourly");
    checkThrows("Empty name check", [&] {HourlyEmployee("",160,20);});
    checkNoThrow("negative rate allowed (intentional)", [&]{ HourlyEmployee{"Bat", 150, -10}; });
    
    SalariedEmployee s("salaried",60000);
    checkThrows("Negative salary", [&] {SalariedEmployee{"Salaried",-60000};});
    check("Salaried monthly Pay check" , s.pay(),5000);
    checkStr("Salaried type",s.type(),"Salaried");
    checkStr("Name check",s.getName(),"salaried");

    manager m("Manager",84000,1000);
    checkThrows("Negative bonus manager", [&] {manager{"Manager" ,84000, -1000};});
    check("Manager salary check", m.pay(),8000);
    checkStr("Manager type",m.type(),"Manager");
    checkStr("Name check",m.getName(),"Manager");

    Intern i("intern");
    check("Intern salary check", i.pay() , 500);
    checkStr("Inter type",i.type(),"Intern");
    checkStr("Name check",i.getName(),"intern");

    const employee& e = m;
    check("manager pay through employee&", e.pay(), 8000);
    return report();
}