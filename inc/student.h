#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <iostream>
class Student {
    public:
        double average(const std::vector<double>& scores) {
            if (scores.empty()) return 0;
            double sum = 0;
            for (double score : scores) {
                sum += score;
            }
            return sum / scores.size();
        }
        void findMinMax(const std::vector<double>& scores, double& min, double& max) {
            if (scores.empty()) {
                throw std::invalid_argument("Scores vector is empty.");
            }
            min = scores[0];
            max = scores[0];
            for (double score : scores) {
                if (score < min) min = score;
                if (score > max) max = score;
            }
        }
        void curveScores(std::vector<double>& scores, double bonus) {
            for (double& score : scores) {
                score += bonus;
                if (score > 100) score = 100;
                if (score < 0) score = 0;
            }
        }
        char letterGrade(double score) {
            if (score < 0 || score > 100) throw std::invalid_argument("Score must be 0-100");
            if (score >= 90) return 'A';
            if (score >= 80) return 'B';
            if (score >= 70) return 'C';
            if (score >= 60) return 'D';
            return 'F';
        }
        double* findScore(std::vector<double>& scores, double target) {
        for (double& score : scores) {
            if (score == target) {
                return &score;  
            }
        }
        return nullptr;  
    }
};