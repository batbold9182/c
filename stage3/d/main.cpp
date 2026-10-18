#include <iostream>
#include <stdexcept>
#include "../../inc/vehicle.h"
#include <vector>

void printVehicles(const std::vector<Vehicle*>& vehicles, int days) {
    std::cout << "=== " << days << " days ===\n";
    double total = 0;
    for (const Vehicle* v : vehicles){
        std::cout << v->getType() << "  " << v->getPlate() << "  "
                  << v->dailyPrice() << "  " << v->rentalCost(days) << "\n";
        total += v->rentalCost(days);
    }
    std::cout << "Total rental cost: " << total << "\n\n";
}

int main(){
    std::vector<Vehicle*> vehicles;
    vehicles.push_back(new Bike("BK-1"));
    vehicles.push_back(new Car("CR-1", 40));
    vehicles.push_back(new ElectricCar("EV-1", 40));
    vehicles.push_back(new Truck("TR-1", 60, 2));

    printVehicles(vehicles, 3);
    printVehicles(vehicles, 7);     // 7 days or more: 10% discount

    std::cout << "=== Invalid input ===\n";
    try{
        vehicles.push_back(new Bike(""));
    } catch(const std::invalid_argument& e){
        std::cout << "Empty plate:    REFUSED: " << e.what() << "\n";
    }

    try{
        vehicles.push_back(new Car("CR-2", -40));
    } catch(const std::invalid_argument& e){
        std::cout << "Negative rate:  REFUSED: " << e.what() << "\n";
    }

    try{
        vehicles.push_back(new Truck("TR-2", -40, 2));
    } catch(const std::invalid_argument& e){
        std::cout << "Negative truck rate: REFUSED: " << e.what() << "\n";
    }

    try{
        vehicles.push_back(new Truck("TR-2", 40, -2));
    } catch(const std::invalid_argument& e){
        std::cout << "Negative tons:  REFUSED: " << e.what() << "\n";
    }

    try{
        std::cout << vehicles[0]->rentalCost(0) << "\n";
    } catch(const std::invalid_argument& e){
        std::cout << "0 days:         REFUSED: " << e.what() << "\n";
    }

    for (Vehicle* v : vehicles) delete v;
    vehicles.clear();
    return 0;
}
