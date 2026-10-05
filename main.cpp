#include <iostream>
#include <limits>
#include <stdexcept>
#include <iomanip>
#include "inc/calculator.h"
#include <sstream>
bool readChoice(int& out) {
    std::string line;
    if (!std::getline(std::cin, line)) return false;
    std::istringstream ss(line);
    char extra;
    return (ss >> out) && !(ss >> extra);
}
bool readNumber(double& out) {
    std::string line;
    if (!std::getline(std::cin, line)) return false;   // input ended

    std::istringstream ss(line);   // treat the line like a mini cin
    char extra;
    if (ss >> out && !(ss >> extra)) return true;    // got a number, and nothing after it
    return false;
}
int main() {
    int count = 0;
    while(true) {
        calculator calc;
        int choice;
        std::cout << "Select an operation:\n";
        std::cout << "1. Add\n";
        std::cout << "2. Subtract\n";
        std::cout << "3. Multiply\n";
        std::cout << "4. Divide\n";
        std::cout << "5. Power\n";
        std::cout << "6. Check remainder\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
        if (!readChoice(choice)) {
            if (std::cin.eof()) break;          // input ended → exit
            std::cout << "Please enter a number from 1 to 7.\n";
            continue;
        }
        if (choice < 1 || choice > 7) {
            std::cout << "Please enter a number from 1 to 7.\n";
            continue;
        }
        double num1, num2;         
        try {
            if (choice == 7) {
                break;
            }
            
            std::cout << "First number: ";
            if (!readNumber(num1)) {
                std::cout << "Invalid number.\n";
                continue; 
            }

            std::cout << "Second number: ";
            if (!readNumber(num2)) {
                std::cout << "Invalid number.\n";
                continue;
            }
          } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            continue;
        }
        
        try {
            switch (choice) {
                case 1:
                    std::cout << std::setprecision(15) << "########Result: " << calc.add(num1, num2) << std::endl;
                    break;
                case 2:
                    std::cout << std::setprecision(15) << "########Result: " << calc.subtract(num1, num2) << std::endl;
                    break;
                case 3:
                    std::cout << std::setprecision(15) << "########Result: " << calc.multiply(num1, num2) << std::endl;
                    break;  
                case 4:
                    std::cout << std::setprecision(15) << "########Result: " << calc.divide(num1, num2) << std::endl;
                    break;
                case 5:
                    std::cout << std::setprecision(15) << "########Result: " << calc.power(num1, num2) << std::endl;
                    break;
                case 6:
                    std::cout << std::setprecision(15) << "########Remainder: " << calc.remainder(num1, num2) << std::endl;
                    break;
                default:
                    std::cout  << "########Invalid choice. Please try again.########" << std::endl;
                    continue;
                
            }
            count++;
        } catch (const std::invalid_argument& e) {
          std::cout << e.what() << std::endl;

        }
        
       
        continue;
        
    }
    std::cout << "You have performed " << count << " operations." << std::endl;
    return 0;
}