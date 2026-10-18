#include <iostream>
#include <vector>
#include "../../inc/employee.h"
#include <exception>
void printEmployee(const std::vector<employee*>& employees) {
    for (const employee* a : employees)
        std::cout << a->type() << "  " << a->getName() << "  " << a->pay() << "\n";
}
int main(){
    std::vector<employee*> employees;

    
    employees.push_back(new SalariedEmployee("Anna", 60000));
    employees.push_back(new HourlyEmployee("bat",150,20));
    employees.push_back(new manager("Mia", 84000, 1000));
    employees.push_back(new Intern("Ken"));
    printEmployee(employees);

    std::cout << "\n=== Invalid Employees ===\n";
    try{
        employees.push_back(new HourlyEmployee("HOURLY", -150,10));
    } catch(const std::invalid_argument& e){
        std::cout<<":REFUSED " << e.what()<< "\n";
    }
    try{
        employees.push_back(new manager("MANAGER", 84000,-1000));
    } catch(const std::invalid_argument& e){
        std::cout<<":REFUSED " << e.what()<< "\n";
        
    }
    try{
        employees.push_back(new SalariedEmployee("SALARIED",-84000));
    } catch(const std::invalid_argument& e){
        std::cout<<":REFUSED " << e.what()<< "\n";
    }
    for (employee* a : employees) delete a;
    employees.clear();
    return 0;   
}