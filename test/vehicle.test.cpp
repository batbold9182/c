#include "test.helper.h"
#include "../inc/vehicle.h"
#include <stdexcept>

int main(){
    //BIKE
    Bike b("BK-1");
    check("BIKE price 3 day" , b.rentalCost(3), 30);
    check("BIKE price 7 day" , b.rentalCost(7), 63);
    check("BIKE daily price", b.dailyPrice(), 10);
    checkStr("BIKE type", b.getType(), "Bike");
    checkStr("BIKE plate", b.getPlate(), "BK-1");
    checkThrows("BIKE empty plate", [&]{ Bike{""}; });

    //CAR
    Car c("CR-1", 40);
    check("CAR price 3 day",c.rentalCost(3),120);
    check("CAR price 7 day",c.rentalCost(7),252);
    check("CAR daily price", c.dailyPrice(), 40);
    checkStr("CAR type", c.getType(), "Car");
    checkStr("CAR plate", c.getPlate(), "CR-1");
    checkThrows("car negative rate", [&]{ Car{"CR-3", -40}; });
    checkNoThrow("car zero rate allowed", [&]{ Car{"CR-4", 0}; });

    //ELECTRIC CAR
    ElectricCar e("ELECTRIC EV-1", 40);
    check("ELECTRIC daily price (car + 5)", e.dailyPrice(), 45);
    check("ELECTRIC price 3 day", e.rentalCost(3), 135);
    check("ELECTRIC price 7 day", e.rentalCost(7), 283.5);
    checkStr("ELECTRIC type", e.getType(), "ElectricCar");
    checkStr("ELECTRIC plate", e.getPlate(), "ELECTRIC EV-1");
    checkThrows("empty plate",[&]{ElectricCar{"",40};});
    checkThrows("ELECTRIC negative rate", [&]{ ElectricCar{"EV-2", -40}; });

    //TRUCK
    Truck t("TRUCK TR-1", 60, 2);
    check("TRUCK daily price (rate + 15 x tons)", t.dailyPrice(), 90);
    check("TRUCK price 3 day", t.rentalCost(3), 270);
    check("TRUCK price 7 day", t.rentalCost(7), 567);
    check("TRUCK 0 tons = only the rate", Truck("TR-0", 60, 0).dailyPrice(), 60);
    checkStr("TRUCK type", t.getType(), "Truck");
    checkStr("TRUCK plate", t.getPlate(), "TRUCK TR-1");
    checkThrows("Truck rate ",[&]{Truck{"tr-2",-40,2};});
    checkThrows("Truck ton ",[&]{Truck{"tr-2",40,-2};});

    //RENTAL DAYS (same rule for every vehicle, tested on the car)
    check("6 days: no discount yet", c.rentalCost(6), 240);
    check("7 days: discount starts", c.rentalCost(7), 252);
    // inside checkNoThrow: if rentalCost(1) wrongly throws, we get a FAIL line instead of a crash
    checkNoThrow("1 day allowed", [&]{ check("1 day: smallest rental", c.rentalCost(1), 40); });
    checkThrows("0 days refused",  [&]{ c.rentalCost(0); });
    checkThrows("-1 days refused", [&]{ c.rentalCost(-1); });

    //POLYMORPHISM: through const Vehicle&
    const Vehicle& v1 = b;
    const Vehicle& v2 = c;
    const Vehicle& v3 = e;
    const Vehicle& v4 = t;
    check("Vehicle& bike daily price", v1.dailyPrice(), 10);
    check("Vehicle& car daily price", v2.dailyPrice(), 40);
    check("Vehicle& electric daily price", v3.dailyPrice(), 45);
    check("Vehicle& truck daily price", v4.dailyPrice(), 90);
    checkStr("Vehicle& electric type", v3.getType(), "ElectricCar");
    check("Vehicle& electric rentalCost 7 day", v3.rentalCost(7), 283.5);

    return report();
}
