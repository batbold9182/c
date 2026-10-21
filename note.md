# C++ Notes

My personal C++ reference: what I learned, why it works, and the bugs I fixed.

## Contents
1. [Basics](#1-basics)
2. [Types and their limits](#2-types-and-their-limits)
3. [Input and output](#3-input-and-output)
4. [Control flow](#4-control-flow)
5. [Functions, references and pointers](#5-functions-references-and-pointers)
6. [Containers and memory](#6-containers-and-memory)
7. [Classes](#7-classes)
8. [Inheritance and polymorphism](#8-inheritance-and-polymorphism)
9. [Designing classes](#9-designing-classes)
10.[DSA] (#10-map, set, algorithms, lambdas, file I/O, smart pointers)
11. [Exceptions](#11-exceptions)
12. [Math](#12-math)
13. [Project structure](#13-project-structure)
14. [Unit testing](#14-unit-testing)
15. [Building](#15-building)
16. [Git and databases](#16-git-and-databases)
17. [Bug log](#17-bug-log)

---

## 1. Basics
- `int main()` returns to the OS: `0` = success, anything else = error. `return 0;` is optional.
- C++ is case sensitive (`main` ≠ `Main`).
- Standard library names live in `std::` (`std::cout`, `std::string`).

**`using namespace std;`** dumps hundreds of names (`count`, `max`, `remainder`…) into my code → name collisions:
```cpp
#include <algorithm>
using namespace std;
int count = 0;
int main() { count++; }   // ❌ 'count' is ambiguous (mine or std::count?)
```
Never in a header (it spreads to every includer). OK in small practice files or inside one function. Good compromise: `using std::cout;`.

---

## 2. Types and their limits
- `char` = one character in **single** quotes: `'b'` (not `"b"`).
- `int` is 32 bits: **-2,147,483,648 … 2,147,483,647**. Overflow rolls over silently (`100000 * 100000` → `1410065408`) and is **undefined behavior**.
- `double` (8 bytes, ~15 digits) over `float` (4 bytes, ~7 digits).
- **Never compare doubles with `==`**: `0.1 + 0.2 == 0.3` is false. Use `std::fabs(a - b) < 1e-9`.
- `enum class Day { Monday, Tuesday };` → `Day::Monday` (scoped, preferred over plain `enum`).
- `str[i]` = no bounds check; `str.at(i)` = throws `std::out_of_range` on a bad index.

---

## 3. Input and output
| Object | Purpose |
|---|---|
| `cin` / `cout` | keyboard input / normal output |
| `cerr` / `clog` | errors (unbuffered) / logs |

`"\n"` over `std::endl` (`endl` also flushes = slower).

### `cin` pitfalls
- `cin >> x` reads one word/value; `std::getline(std::cin, line)` reads the whole line.
- After a failed read, `cin` stays in the **fail** state and every later `>>` does nothing → `cin.clear()` + `cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n')`.
- **End of input** (Ctrl+Z / Ctrl+D) can't be fixed with `clear()` → check `cin.eof()` and stop, or the program loops forever.
- `cin >>` leaves **leftovers**: typing `1.5` at a menu reads `1` and leaves `.5` for the next read.

### ✅ Safe input: read the whole line, then parse
```cpp
bool readNumber(double& out) {
    std::string line;
    if (!std::getline(std::cin, line)) return false;   // input ended
    std::istringstream ss(line);                        // <sstream>
    char extra;
    return (ss >> out) && !(ss >> extra);               // exactly one number, nothing after
}
```
`5` ✅ · `5abc` ❌ · `5 9` ❌ · `abc` ❌. Don't mix `cin >>` and `getline`.

### `<iomanip>`
`setprecision(n)` (stays in effect; default shows only 6 digits → `1.23457e+08`), `setw(n)` (next item only), `setfill(c)`, `quoted(s)`.

---

## 4. Control flow
| | in a loop | in a `switch` |
|---|---|---|
| `break` | leaves the loop (only the **innermost** one) | leaves the `switch` |
| `continue` | jumps to the next pass | applies to the surrounding loop |

- Code right after `break` / `continue` / `return` / `throw` never runs (warning **C4702**).
- Without `{ }`, `for`/`if` cover only the **next statement**, whatever the indentation says.
- `switch`: integers/chars/enums only; each `case` needs `break` or it falls through.
- **Loops:** put setup at the **start** (e.g. `clear()`), not cleanup at the end (`continue` skips it). Every loop needs a way out.

---

## 5. Functions, references and pointers

### Passing parameters
| Style | Copy? | Changes caller's variable? | Use for |
|---|---|---|---|
| `f(int x)` | yes | no | small types |
| `f(int& x)` | no | **yes** | results / modifying |
| `f(const std::string& s)` | no | no | big types you only read |

`&` = no copy, `const` = can't change. A literal like `"Anna"` only binds to `const std::string&`.
Return two things: `bool readNumber(double& out)` (bool = success, `out` = value).

- Overloading: same name, different parameter lists.
- A parameter does nothing unless the body uses it (warning **C4100**: received but never used).
- One function, one job. `=` copies **right → left** (`hour = h`).
- `void` functions are actions, not values: `a->monthEnd();` ✅, `cout << a->monthEnd()` ❌. Do first, then ask.

### Range-based `for`
```cpp
for (double s : v)        s += 5;   // ❌ changes copies
for (double& s : v)       s += 5;   // ✅ changes the elements
for (const auto& x : v)   ...       // ✅ read-only, no copies
```

### Pointers
```cpp
double* p = &score;   // & = address of
*p = 95;              // * = the value it points to
p->area();            // = (*p).area()
```
`nullptr` = points to nothing; check before `*p`. **Prefer references**; use a pointer when "nothing" must be possible.

### Dangling pointers
A pointer to memory that's gone → undefined behavior (garbage, crash, or *seems to work*):
1. `push_back` / `insert` / `erase` can **move** a vector's elements (iterator invalidation) → store an index instead.
2. Returning `&local` from a function.
3. Using a pointer after `delete` (habit: set it to `nullptr`).

---

## 6. Containers and memory
- `sizeof(arr)` = **bytes**; element count = `sizeof(arr) / sizeof(arr[0])` or `std::size(arr)`.
- `std::vector` = default choice (contiguous, fast `v[i]`); `std::list` only for frequent middle inserts.
- Erase by index: `v.erase(v.begin() + i);`

### `new` / `delete`
- Every `new` needs exactly **one** `delete` (`new[]` → `delete[]`), after the last use.
- Leak by reassigning: `s = new Circle(1);` loses the old object → `delete` first.
- `vector<T*>::clear()` only empties the list of pointers → `for (auto* x : v) delete x;` **then** `clear()`.
- Don't `delete` inside the loop that still uses the vector.
- Prefer no `new` at all: plain objects (`Square sq(2);`), `std::vector`, `std::unique_ptr` free themselves.

---

## 7. Classes
A class = **data** (what it remembers) + **functions** (what it does).
```cpp
class Account {
  protected:
    std::string owner;                     // data
    double balance;
  public:
    Account(const std::string& owner, double start)
        : owner(owner), balance(start) {   // initializer list stores the values
        if (owner.empty() || start < 0) throw std::invalid_argument("Invalid account");
    }
    double getBalance() const { return balance; }   // const = doesn't change the object
};
```
| | Who can use it |
|---|---|
| `public` | everyone |
| `protected` | the class + its subclasses |
| `private` | only the class |

Each label applies **until the next one**. Data subclasses need → `protected`; functions others call → `public`.

### Rules
- **"What does an object remember?"** Nothing → it's a folder of functions; use a namespace.
- **No `cin`/`cout` inside logic classes** → reusable and testable.
- **Every member gets a value** in the constructor; anything not listed holds garbage.
- **Constructors validate and throw**: if an object exists, it's valid. Printing an error still creates the broken object.
- **Store what defines it, calculate the rest** (radius stored, area calculated).
- **Check first, change last**: a refused operation must leave the object unchanged.
- **Every door needs a lock**: constructor checks at creation, setters check later changes (a constructor can call the setter, so the rule lives in one place).
- `explicit` on one-argument constructors prevents accidental conversions.

### Gotchas
- Inside a class a name finds the **member first**: `pow(a, b)` in `calculator::pow` calls itself → stack overflow. Use `std::pow`.
- Parameter with the same name as a member **hides** it (warning **C4458**).
- `const` position: `double f() const` = doesn't change the object ✅; `const double f()` = const return value ❌.
- **Declaration** ends with `;` (the menu), **definition** has `{ }` (the kitchen). Exactly one definition:
  two → `C2084`; none but called → `LNK2019`. In a `.cpp`: `double Circle::area() const { ... }`.
- **Most vexing parse**: `Circle(r);` declares a *variable* `r`. Use `Circle bad(r);` or `Circle{r};`.

---

## 8. Inheritance and polymorphism

### Inheritance = "is a" (one direction only)
```cpp
class Square : public Rectangle {                    // always write public (default is private → C2243)
  public:
    Square(double side) : Rectangle(side, side) {}   // pass values to the parent's constructor
    std::string name() const override { return "Square"; }
};
```
- Square **inherits** `area()`; don't rewrite it, and **don't redeclare the parent's data** (a second, never-set variable → garbage).
- The parent holds what **all** children share; the child adds what makes it special (`bonus` in `Manager`, not in `SalariedEmployee`).

### Three kinds of functions
| Kind | Base has code? | Subclasses | Example |
|---|---|---|---|
| normal | ✅ | use it as is | `deposit`, `getPlate`, `rentalCost` |
| `virtual` | ✅ sensible default | **may** replace it | `withdraw` |
| pure virtual `= 0` | ❌ no sensible default | **must** write it | `type()`, `area()`, `dailyPrice()` |

- `= 0` does **not** mean "value 0": it means "no code here".
- A class with a `= 0` function is **abstract**: `Shape s;` → `C2259`.
- `override` makes the compiler check the name and `const` match exactly.
- **A class with virtual functions needs a virtual destructor** (otherwise `delete` through a base pointer doesn't destroy the whole object). Destructors run child → parent.

### What `virtual` does
**When a function is called through a base pointer or reference, C++ jumps directly to the real object's version.**
```cpp
Dog d;  Animal& a = d;
a.sound();   // without virtual: Animal::sound() (uses the label)   with virtual: Dog::sound() (the real object)
```
- It's about **which function runs**; it never stores or replaces values.
- It does **not** walk Vehicle → Car → ElectricCar. It goes **straight** to the real object's function. If that class doesn't have one, the **nearest parent's** version runs.
- A parent's version runs only when the code asks explicitly: `Car::dailyPrice()` (without `Car::` → calls itself → crash).
- Reuse vs. replace: Savings **calls** `Account::withdraw` (the parent's rule still applies); Checking **replaces** it (overdraft).

### A non-virtual function calling a virtual one
```cpp
double rentalCost(int days) const {        // same rule for everyone → written once, not virtual
    if (days < 1) throw std::invalid_argument("Days must be at least 1");
    double cost = dailyPrice() * days;     // virtual → each vehicle's own price
    if (days >= 7) cost *= 0.9;            // 10 % off = pay 90 %
    return cost;
}
```

### Polymorphism in `main`
```cpp
std::vector<Vehicle*> v = { new Bike("BK-1"), new Car("CR-1", 40) };
for (const Vehicle* x : v) std::cout << x->getType() << " " << x->rentalCost(3) << "\n";   // no type checks
for (Vehicle* x : v) delete x;
v.clear();
```
Also works with references (`const Shape& s = sq;`), no `new` needed. Through `const` pointers/references only `const` functions can be called.

- **Objects enforce, `main` reacts**: classes throw with a reason; `main` catches and prints `e.what()`, without re-checking the rules.
- `try/catch` **inside** the loop, so one refusal doesn't stop the rest. One loop per step.
- Type-specific info: best a virtual `details()`; `if (type() == "...")` is fragile; `dynamic_cast<T*>(p)` (→ `nullptr` if not a T) only rarely.

---

## 9. Designing classes
**Method:** one row of the rules table → one class. Fill in a design table first (class, parent, members, constructor parameters, what each function returns).

| Question | Answer |
|---|---|
| Same for every object of the class? (type name, Bike's 10) | write it in the class's code |
| Different **per object**? (plate, rate, name) | member variable, set by the constructor |
| Different **per call**? (`days`, `amount`) | function parameter |
| Same for all subclasses? (`getPlate`) | normal function in the base, written once |
| Happens once at creation? | constructor |
| Happens every time something is done? | that function (`withdraw`, `setHours`) |

**Totals:** declare before the loop, `total += x;` inside (own line, `<<` binds before `+=`), print after.

---
## 10. DSA
```cpp
-'MAP'
-'SET'
-'ALOGRITHMS'
-'LAMBDAS'
-'FILE I/O'
-'SMART POINTERS'
(#10-map, set, algorithms, lambdas, file I/O, smart pointers)
---
## 11. Exceptions
```cpp
if (b == 0) throw std::invalid_argument("Division by zero");   // <stdexcept>
try { divide(5, 0); }
catch (const std::invalid_argument& e) { std::cout << e.what(); }
```
- `throw` searches outwards for a `catch`; none → the program is killed.
- `try` is a safety net: it only catches what happens **inside** it.
- Handle it here → `if` + `continue`. A function can't do its job → `throw`.
- Lines after a throwing call inside `try` are skipped (count successes there).
- If a constructor throws inside `new`, the memory is freed and nothing is added.

---

## 12. Math
`std::pow`, `std::fmod` (remainder for doubles; `%` is ints only), `std::fabs`, `std::sqrt` (`<cmath>`).
- `x / 0`, `x % 0`: undefined → check first.
- `INT_MIN / -1`, `INT_MIN % -1`: hardware exception, `try` can't catch it. `double` + `fmod` avoid it.
- Seed random numbers once: `std::srand(std::time(0));` (better: `<random>`).
- Precedence: `a + b + c / 2.0` = `a + b + (c / 2)`. Use a variable.

---

## 13. Project structure
```
c++/
├── inc/          libraries (headers): logic only
├── stage1/ …     apps: one program (one main) per folder
├── test/         one test program per library + test.helper.h
└── build/        compiled output (not in git)
```
- Header-only (functions have `{ }` in the `.h`) → include it. Split (`;` only) → also compile the `.cpp`, or `LNK2019`.
- `#pragma once` in every header (never in a `.cpp`). A header includes everything it uses.
- `#include "..."` is relative to the **file containing it**; after moving files, fix the paths. Use `/`.
- C++ headers have no `.h`: `<string>`, `<cmath>`. `<string.h>` is the C header (no `std::string`).
- Two `main()`s in one build → `LNK2005`. Namespaces avoid clashes when combining programs.
- `inline` lets variables/functions be defined in a header; inline **variables** need C++17.

---

## 14. Unit testing
My helpers in `test/test.helper.h`: `check`, `checkStr`, `checkThrows`, `checkNoThrow`, `report()`.
```cpp
check("car 3 days", c.rentalCost(3), 120);
checkThrows("negative rate", [&]{ Car{"CR-3", -40}; });      // constructor errors: create inside
checkThrows("0 days", [&]{ c.rentalCost(0); });              // function errors: call on an object
checkNoThrow("1 day", [&]{ check("1 day costs 40", c.rentalCost(1), 40); });
return report();                                             // summary + exit code (0 = all passed)
```
**Lambdas** `[&]{ ... }` package code so `checkThrows` can run it inside its own `try`. `[&]` = real outside variables, `[=]` = read-only copies, `[]` = none.

### What to test
Normal cases · negatives and zero · decimals · big numbers · every error must throw · things that must **not** throw · **boundaries** (6 vs. 7 days, 160 vs. 161 hours, 89.9 vs. 90) · **each condition separately** (only `a` wrong, only `b` wrong) · polymorphism through `const Base&` · **every bug I've fixed**.

### `void` functions
Check what they changed: reference outputs (start them at a "wrong" value like -1), modified containers, returned pointers (`p == &v[1]`). Printing functions take `std::ostream& out = std::cout`; the test passes an `std::ostringstream`.

### Habits
- Fresh data and a unique name per test. Return the right exit code.
- After `checkThrows`, also **check the value is unchanged** (catches "changed, then threw").
- Anything that might throw goes inside `checkThrows` / `checkNoThrow`; a plain throwing call crashes the test program.
- When a test fails, decide which is wrong: the code or the test. Document intentional choices with a comment + test.
- A test that can never fail tests nothing. The comma operator `(a, b)` keeps only the last value.

### Mutation testing: are my tests good?
Break the code on purpose (`||` → `&&`, `>=` → `>`, delete a check), run the tests, **undo**. Tests fail → good. Tests pass → add a test.
With correct code, weak and strong tests look the same; only deliberate bugs show the difference.
**Equivalent mutant**: a change with no effect (overtime `<= 160` → `< 160`): no test can catch it.

| Tests | First version | Final |
|---|---|---|
| shapes | missed 2 of 2 | caught both |
| accounts | 0 of 4 caught | 33 tests, 8/8 |
| employees | 0 of 4 caught | 19 tests, 6/7 |
| vehicles | — | 40 tests, 7/7 |

Whole program: `build\main.exe < input.txt`. Test bad input, two operations in a row, and end of input.
Real-world tools: **GoogleTest** (industry standard), Catch2 / doctest, **CTest** (runs test `.exe`s, checks exit codes).

---

## 14. Building
Compile (`.cpp` → `.obj`) then link (`.obj` → `.exe`). MSVC also makes `.pdb` (debug info, `/Zi`) and `.ilk`; all are safe to delete. g++ leaves only the `.exe`.

| Error starts with | Who | Usual cause |
|---|---|---|
| `C2xxx` / `C3xxx` | compiler | typo, duplicate definition, missing include |
| `LNKxxxx` | linker | a `.cpp` not compiled in, function never written, two `main()`s |

### My VS Code tasks (Ctrl+Shift+B, read the name before Enter)
| Task | Use for |
|---|---|
| build active file | single-file programs, header-only tests |
| build all .cpp in active file's folder | multi-file apps (`stage3/a`) → `build\a.exe`; **never** in `test/` |
| build shape tests | `shape.test.cpp` + `stage3/a/shape.cpp` |

Ignore the *detected tasks* (VS Code defaults without my flags).
- **Build ≠ run**: run with `.\build\name.exe` (PowerShell needs `.\`). "Build finished" + strange exit = the program crashed (usually an uncaught exception).
- Flags: `/W4` (more warnings; they found real bugs), `/std:c++20`. **MSVC defaults to C++14.**
- Version: `cl` → 19.40 (VS 2022). Standard: `_MSVC_LANG` (`201402` C++14, `201703` C++17, `202002` C++20).

---

## 15. Git and databases
**Git:** don't commit build output. `.gitignore`: `build` (matches at any level), `*.exe`, `*.obj`, `*.pdb`, `*.ilk`, `.env`. Comments on their own line. Already committed → `git rm -r --cached build`.

**Databases:** SQL is the language; SQLite (embedded, one file, one app) vs. server databases (PostgreSQL, MS SQL Server, MySQL: many users over the network). C++ → PostgreSQL via `libpqxx`. Never put passwords in code.

---


## 16. Bug log
| # | Bug → fix | Lesson |
|---|---|---|
| 1 | Same random number every run → `srand(time(0))` | Random needs a seed |
| 2 | `pow` called itself → `std::pow` | Members are found first |
| 3 | `100000 * 100000` = 1410065408 → `double` | Know type limits |
| 4 | `INT_MIN % -1` crashed → `double` + `fmod` | Edges hide bugs |
| 5 | Infinite loop at end of input → `if (cin.eof()) break;` | What if input ends? |
| 6 | Bad input crashed → throw inside `try`, or `if` | `try` only catches inside |
| 7 | `1.23457e+08` shown → `setprecision(15)` | Display ≠ value |
| 8 | Count always 0 (`count++` after `continue`) → move it | Read warnings |
| 9 | Second calculation skipped (extra `cin`) → remove | Test two in a row |
| 10 | `1.5` at menu ran Add with 0.5 → `getline` + `istringstream` | Read whole lines |
| 11 | Unused params in `validateInput` → remove / split | One function, one job |
| 12 | Scores 0 and 100 rejected (`<=`/`>=`) → `<`/`>` | Test exact boundaries |
| 13 | Grades never ended → `eof` checks + 0 to quit | Every loop needs an exit |
| 14 | Old students in next round → `clear()` at loop start | Setup at the start |
| 15 | Extra prompt after input ended → `eof` check after inner loop | `break` = innermost only |
| 16 | Curved score negative → clamp in `curveScores` | Functions protect their rules |
| 17 | Test exit code 0 with failures → `failed == 0 ? 0 : 1` | Tools trust exit codes |
| 18 | 5 × C4458 in `Student` → remove unused members | Don't half-build a class |
| 19 | `Square(5)` area 0 (own `double a;`) → inherit | Don't redeclare parent data |
| 20 | `Circle(-3)` created (printed error) → throw | Constructors throw |
| 21 | `Triangle(1,2,10)` created (check never called) → check in constructor | Validate where born |
| 22 | C2084 (`name()` in .h and .cpp) → one definition | One function, one body |
| 23 | C2509 (`Square::Rectangle::area`) → delete | Inherited = no definition |
| 24 | LNK2019 (constructors missing / `shape.cpp` not built) | LNK = missing code |
| 25 | Heron wrong (`a+b+c/2.0`) → `double s = (a+b+c)/2.0;` | Operator precedence |
| 26 | `'string' not in std` (`<string.h>`) → `<string>` | C++ headers have no .h |
| 27 | Moved files broke includes → `"../inc/x.h"` | Includes are file-relative |
| 28 | Shape tests passed broken code → separate conditions | Mutation testing |
| 29 | `owner`/`balance` as virtual functions → data members | Data stores, functions do |
| 30 | Refused `withdraw(-50)` changed balance → check first | Check first, change last |
| 31 | 4th savings withdrawal allowed (counted in constructor) | Constructor runs once |
| 32 | Random results (unset `overdraftLimit`, `amount` member) | Every member gets a value |
| 33 | C2665: `double initialBalance()` → no `()` | `()` makes a function |
| 34 | C2248: nothing public → data protected, functions public | Labels last until the next |
| 35 | C2243: `: Account` → `: public Account` | Always write public |
| 36 | Savings withdraw did nothing (code after `throw` in `if`) | Outside the `if` |
| 37 | Student redeclared `overdraftLimit` → pass 0 to parent | Don't redeclare parent data |
| 38 | C2562: `void` returned a value → empty body | void = nothing back |
| 39 | 3b `main` threw itself, `while(true)` → catch, no infinite loop | Objects enforce, main reacts |
| 40 | `cout << accounts[i]` printed addresses → `a->getX()` | A pointer is an address |
| 41 | `cout << a->monthEnd()` → call, then print | Do first, then ask |
| 42 | Table printed 3 times (print inside loop) → print after | Which lines repeat? |
| 43 | "4 withdrawals" did 1 → loop over attempts, `try` inside | Loop over what repeats |
| 44 | `clear()` leaked → `delete` first | clear() ≠ delete |
| 45 | C7525 inline variables → `/std:c++20` | Know your standard |
| 46 | "Build failing" = test crashed (throw outside `checkThrows`) | Build ≠ run |
| 47 | Test expected fee on positive balance → fix the test | Decide the rule first |
| 48 | `bonus`/`hour` in `SalariedEmployee` → one class per row | Parent = shared only |
| 49 | C2665 with `"Anna"` (`std::string&`) → `const std::string&` | const& for inputs |
| 50 | `manager::pay()` called itself → `SalariedEmployee::pay()` | Parent needs `Parent::` |
| 51 | Hourly pay garbage (C4100) → store in initializer list | Read C4100 |
| 52 | `setHours` did nothing (`h = hour`) → `hour = h` | `=` copies right → left |
| 53 | Overtime for short weeks (swapped branches) | Read conditions out loud |
| 54 | Ran shape tests by accident → pick the right task | Read the task name |
| 55 | Plate always empty → `: Plate(Plate)` | Receiving ≠ storing |
| 56 | `getPlate`/`rentalCost` copied 5× → once in the base | Same for all → write once |
| 57 | `const double f()` → `double f() const` | const after `()` |
| 58 | 3-day rental cost 12 (`× 0.1`) → `× 0.9` when `>= 7` | 10 % off = pay 90 % |
| 59 | 7 days no discount (`> 7`) → `>= 7` | Test 6 vs. 7 |
| 60 | ElectricCar price 5 (unset rate passed up) → rate parameter | Values come from the caller |
| 61 | Truck ignored tons → `rate + 15 * tons` | Compare with the rules |
| 62 | Total broken (no `{ }`, reset inside, `+=` in `cout`) | Before / inside / after |
| 63 | Test crashed on a mutant (plain `check`) → inside `checkNoThrow` | Wrap risky calls |









