#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <stdexcept>

inline int passed = 0;
inline int failed = 0;

void check(const std::string& name, double actual, double expected) {
    if (std::fabs(actual - expected) < 1e-9) {
        passed++;
    } else {
        failed++;
        std::cout << "FAIL: " << name << "  expected " << expected
                  << " but got " << actual << "\n";
    }
}

// Check that something throws invalid_argument
template <typename Func>
void checkThrows(const std::string& name, Func f) {
    try {
        f();
        failed++;
        std::cout << "FAIL: " << name << "  expected an exception, none thrown\n";
    } catch (const std::invalid_argument&) {
        passed++;
    }
}

// compare strings: name() and type() tests
inline void checkStr(const std::string& name, const std::string& actual, const std::string& expected) {
    if (actual == expected) passed++;
    else { failed++; std::cout << "FAIL: " << name << "  expected \"" << expected
                               << "\" but got \"" << actual << "\"\n"; }
}

// something must NOT throw: valid inputs near a boundary
template <typename Func>
void checkNoThrow(const std::string& name, Func f) {
    try { f(); passed++; }
    catch (const std::exception& e) {
        failed++; std::cout << "FAIL: " << name << "  unexpected exception: " << e.what() << "\n";
    }
}

// the summary + exit code every test ends with
inline int report() {
    std::cout << "Passed: " << passed << ", Failed: " << failed << "\n";
    return failed == 0 ? 0 : 1;
}