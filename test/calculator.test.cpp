#include "test.helper.h"
#include "calculator.h"


int main() {
    calculator calc;

    // --- add ---
    check("add positive",  calc.add(2.5, 3.0), 5.5);
    check("add negative",  calc.add(-5, 3), -2);
    check("add decimals",  calc.add(0.1, 0.2), 0.3);
    check("add big",       calc.add(123456789, 1), 123456790);

    // --- subtract ---
    check("subtract",          calc.subtract(10, 4), 6);
    check("subtract negative", calc.subtract(10, -2), 12);

    // --- multiply ---
    check("multiply",          calc.multiply(3, -4), -12);
    check("multiply overflow", calc.multiply(100000, 100000), 1e10);  // was broken with int!
    check("multiply by zero",  calc.multiply(5, 0), 0);

    // --- divide ---
    check("divide",          calc.divide(5, 2), 2.5);
    check("divide negative", calc.divide(-9, 3), -3);
    checkThrows("divide by zero", [&] { calc.divide(5, 0); });

    // --- power ---
    check("power",          calc.power(2, 10), 1024);
    check("power zero",     calc.power(5, 0), 1);
    check("power negative", calc.power(2, -1), 0.5);

    // --- remainder ---
    check("remainder",          calc.remainder(10, 3), 1);
    check("remainder negative", calc.remainder(-7, 3), -1);
    check("remainder INT_MIN",  calc.remainder(-2147483648.0, -1), 0);  // used to crash!
    checkThrows("remainder by zero", [&] { calc.remainder(5, 0); });

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed == 0 ? 0 : 1;   // exit code 1 means "tests failed"
    return report();
}