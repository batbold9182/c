#include <iostream>
#include <fstream>
#include <string>

int main() {

    while (true) {
        std::cout << "\n1. Write file\n";
        std::cout << "2. Read file\n";
        std::cout << "3. Update file\n";
        std::cout << "4. Clear file\n";
        std::cout << "5. Exit\n";

        int choice;
        std::cin >> choice;
        std::cin.ignore(10000, '\n');

        switch (choice) {

            case 1: {
                std::ofstream MyFile("test.txt", std::ios::app);

                std::string text;
                std::cout << "Enter text: ";
                std::getline(std::cin, text);

                MyFile << text << '\n';
                MyFile.close();
                break;
            }

            case 2: {
                std::ifstream MyFile("test.txt");

                if (!MyFile.is_open()) {
                    std::cout << "File could not be opened\n";
                    break;
                }

                std::string text;
                while (std::getline(MyFile, text)) {
                    std::cout << text << '\n';
                }

                MyFile.close();
                break;
            }

            case 3: {
                std::fstream MyFile("test.txt", std::ios::out | std::ios::trunc);

                if (!MyFile.is_open()) {
                    std::cout << "File could not be opened\n";
                    break;
                }

                std::string text;

                std::cout << "Enter new content: ";
                std::getline(std::cin, text);

                MyFile << text << '\n';

                MyFile.close();

                std::cout << "File updated!\n";
                break;
            }

            case 4: {
                std::ofstream MyFile("test.txt", std::ios::trunc);
                MyFile.close();

                std::cout << "File cleared!\n";
                break;
            }

            case 5:
                return 0;

            default:
                std::cout << "Wrong input\n";
        }
    }

    return 0;
}