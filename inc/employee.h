#pragma once
#include <iostream>
#include <stdexcept>
#include <string>
class employee{
    protected:
        std::string name;
    public:
        employee(const std::string& name)
            :name(name){
                if(name.empty()){
                  throw std::invalid_argument("Name is empty");
                }
            }

    std::string getName() const {return name;};
    virtual double pay() const = 0;
    virtual std::string type() const = 0;
    virtual ~employee(){}
};

class SalariedEmployee :public employee{
    private:
       double yearlySalary;
    public:
    SalariedEmployee(const std::string& name , double mSalary):
        employee(name),yearlySalary(mSalary){
            if(yearlySalary<0){
                    throw std::invalid_argument("Salary cant be negative");
                }
        }
    double pay()const override{        
        return yearlySalary/12;
    }
    
    std::string type() const override {return "Salaried";};

};

class HourlyEmployee :public employee{
    private:
        double hour;
        double hourlyRate;
    public:
        HourlyEmployee(const std::string& name,double h ,double rate):
            employee(name),hour(h),hourlyRate(rate){
                if(hour<0){
                    throw std::invalid_argument("Hours cant be negative");
                }
            }
    void setHours(double h){
        if (h<0){
            throw std::invalid_argument("Hours cant be negative");    
        };
         hour=h;
    }

    double pay() const override {
      if(hour<=160){
        return hourlyRate * hour;
      } else {
        return hourlyRate * 160 + hourlyRate * 1.5 * (hour - 160);
      }
    }
    std::string type() const override {return "Hourly";};
};
class manager : public SalariedEmployee{
    private: 
    double bonus ;
    public:
    manager(const std::string& name , double mSalary, double bonus):
        SalariedEmployee(name , mSalary),bonus(bonus){
            if(bonus<0){
                throw std::invalid_argument("Bonus cant be negative");
            }
        }
    double pay()const override{
        return SalariedEmployee::pay() + bonus;
    };
    std::string type() const override { return "Manager"; }
};

class Intern : public employee{
    public:
        Intern(const std::string& name):
            employee(name){};

    double pay() const override {
      return 500;
    }
    std::string type() const override {return "Intern";};
};