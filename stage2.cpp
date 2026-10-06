#include <iostream>
#include <sstream>
#include "inc/student.h"
#include <stdexcept>
/**Requirements
- [x] Ask how many students, then read each name and score (0–100)
- [x] Store them in two `std::vector`s (names, scores)
- [x] Write these functions — choose correctly between pass-by-value, `&` and `const &`:
  - `double average(const std::vector<double>& scores)`
  - `void findMinMax(const std::vector<double>& scores, double& min, double& max)` — returns results **through references**
  - `void curveScores(std::vector<double>& scores, double bonus)` — adds bonus, caps at 100
  - `char letterGrade(double score)` — A/B/C/D/F
- [x] Print a table: name, score, letter grade
- [x] **Pointer part:** write `int* findScore(std::vector<int>& scores, int target)` that returns a pointer to the first matching score or `nullptr`. In `main`, if found, change the score **through the pointer**.

**Questions to answer in a comment at the top of the file**
1. Why does `average` take `const &` and not just `std::vector<int>`?
it returns single double value so does not have to whole vector if we use vector it will copy new vector and then uses loop to calculate average
using const avoids everything.
2. What happens if you use the pointer from `findScore` after `push_back` to the vector? (Search: "iterator invalidation")
If push_back needs more space, the vector moves all its elements to new memory and frees the old memory. The pointer still points to the old, freed memory (a dangling pointer), so using it is undefined behavior.
**Learn:** references, const, pointers, nullptr, range-based for**/
    bool ReadGrade(double& out) {
        std::string line;
        if (!std::getline(std::cin, line)) return false;  

        std::istringstream ss(line);   
        char extra;
        if (ss >> out && !(ss >> extra)) return true;   
        return false;
    }
    bool ReadInt(int& out) {

    std::string line;
        if (!std::getline(std::cin, line)) return false;
        std::istringstream ss(line);
        char extra;
        return (ss >> out) && !(ss >> extra);
    }
void printTable(const std::vector<std::string>& names, const std::vector<double>& scores) {
        std::cout << "Name\tScore\tLetter Grade\n";
        for (size_t i = 0; i < names.size(); ++i) {
            std::cout << names[i] << "\t" << scores[i] << "\t" << Student().letterGrade(scores[i]) << "\n";
        }
    }
int main(){
    std::vector<std::string> names;
    std::vector<double> scores;
    std::string name;
    double score,bonus,min,max;
    int totalStudents;
    Student student;
    
    while(true){
        names.clear(); 
        scores.clear();
    try {
        std::cout << "How many students? ";
        if (!ReadInt(totalStudents)) {
            if(std::cin.eof()) break;
            std::cout << "Invalid number.\n";
            continue; 
        }
        if(totalStudents == 0) break;
        if (totalStudents < 0) { std::cout << "Must be at least 1.\n"; continue; }
        for (int i = 0; i < totalStudents; ++i) {
            std::cout << "Enter score for student " << (i + 1) << ": ";
            if (!ReadGrade(score) || score < 0 || score > 100) {
                if(std::cin.eof()) break;
                std::cout << "Invalid score.\n";
                --i;
                continue; 
            }
            scores.push_back(score);

            std::cout << "Enter name for student " << (i + 1) << ": ";
            std::getline(std::cin, name);
            names.push_back(name);
        }

        if (totalStudents < 0) { std::cout << "Must be at least 1.\n"; continue; }
        std::cout << "Enter bonus points to curve scores: ";
        if (!ReadGrade(bonus)) {
            if(std::cin.eof()) break;
            std::cout << "Invalid bonus.\n";
            continue; 
        }
        student.curveScores(scores, bonus);
        std::cout<<"Average score: "<<student.average(scores)<<"\n";;
        student.findMinMax(scores, min, max);
        std::cout << "Minimum score: " << min << "\n";
        std::cout << "Maximum score: " << max << "\n";
        std::cout << "bonus added: " << bonus << "\n";
        for (size_t i = 0; i < scores.size(); ++i) {
            scores[i] = std::min(scores[i], 100.0);
            std::cout<< "student name: " << names[i] << ", curved score: " << scores[i] << ", letter grade: " << student.letterGrade(scores[i]) << "\n";
        }
        std::cout << "Enter a score to find: ";
        if (!ReadGrade(score)) {
            if(std::cin.eof()) break;
            std::cout << "Invalid score.\n";
            continue; 
        }
        double* foundScore = student.findScore(scores, score);
        if (foundScore) {
            std::cout << "Score " << score << " found. Enter new score: ";
            double newScore;
            if (!ReadGrade(newScore) || newScore < 0 || newScore > 100) {
                if(std::cin.eof()) break;
                std::cout << "Invalid score.\n";
                continue; 
            }
            *foundScore = newScore;  
            std::cout << "before update: " << score << ", after update: " << *foundScore << "\n";

        } else {
            std::cout << "Score " << score << " not found.\n";
        }
        std::cout << "Updated table:\n";
        printTable(names, scores);
    }
        catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            continue;
        }
    }
    return 0;
}