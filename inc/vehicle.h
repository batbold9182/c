#pragma once
#include <stdexcept>
#include <string>

class Vehicle {
    protected:
        std::string Plate;
    public:
        Vehicle(const std::string& Plate)
            :Plate(Plate){
                if(Plate.empty()){
                    throw std::invalid_argument("Plate number cannot be empty");
                }
            }
        std::string getPlate() const { return Plate; } 
        virtual double dailyPrice() const = 0;
        virtual std::string getType() const = 0;
        double rentalCost(int days) const {
            if(days<1){
                throw std::invalid_argument("Days must be at least 1");
            }
            if(days>=7){
                return (days*dailyPrice())*0.9;
            } else {
                return dailyPrice() * days;
            }
        }
        virtual ~Vehicle(){}
};

class Car : public Vehicle {
    private:
        double dailyRate;
    public:
        Car(const std::string& Plate, double rate)
        :Vehicle(Plate),dailyRate(rate){
            if(dailyRate<0){
                throw std::invalid_argument("Daily rate cannot be negative");
            }
        }
        
        double dailyPrice()const override{
            return dailyRate;
        }

        std::string getType() const override {return "Car";}
        
};

class Bike : public Vehicle {
    public:
        Bike(const std::string& Plate)
            :Vehicle(Plate){}

        double dailyPrice()const override{
                return 10;
            }

        std::string getType() const override {return "Bike";}

};

class ElectricCar : public Car {
    public:
        ElectricCar(const std::string& Plate, double rate)
            :Car(Plate,rate){}

        double dailyPrice()const override{
            return Car::dailyPrice() + 5;
        }

        std::string getType() const override {return "ElectricCar";}

};

class Truck : public Vehicle {
    private:
        double dailyRate;
        double tons;
    public:
        Truck(const std::string& Plate, double rate , double t)
        :Vehicle(Plate),dailyRate(rate),tons(t){
            if(rate<0){
                throw std::invalid_argument("Daily rate cannot be negative");
            }
            if(t<0){
                throw std::invalid_argument("Tons cannot be negative");
            }
        }

        double dailyPrice()const override{
            return dailyRate + 15 * tons;
        }

        std::string getType() const override {return "Truck";}

};
