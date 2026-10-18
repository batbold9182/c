# C++ Notes

My personal C++ reference: what I learned, why things work, and the bugs I fixed along the way.

## Contents
1. [Program basics](#1-program-basics)
2. [Data types and their limits](#2-data-types-and-their-limits)
3. [Input and output](#3-input-and-output)
4. [Control flow](#4-control-flow)
5. [Functions](#5-functions)
6. [Arrays, vectors and lists](#6-arrays-vectors-and-lists)
7. [Memory: new and delete](#7-memory-new-and-delete)
8. [Classes](#8-classes)
   - [8b. Inheritance and polymorphism](#8b-inheritance-and-polymorphism)
   - [8c. Inheritance in practice (bank accounts)](#8c-inheritance-in-practice-stage-3b-bank-accounts)
   - [8d. Designing classes from scratch](#8d-designing-classes-from-scratch-stages-3c--3d)
9. [Exceptions](#9-exceptions)
10. [Math functions](#10-math-functions)
11. [Project structure: library, app, tests](#11-project-structure-library-app-tests)
12. [Unit testing](#12-unit-testing)
13. [Compiling and build files](#13-compiling-and-build-files)
14. [Git](#14-git)
15. [Databases (overview)](#15-databases-overview)
16. [Pointers and references](#16-pointers-and-references)
17. [Bug log: bugs I found and fixed](#17-bug-log-bugs-i-found-and-fixed)

---

## 1. Program basics

### `int main()`
Every program starts at `main`. It returns an `int` to the **operating system**:
- `0` = success
- anything else = an error happened

`return 0;` at the end of `main` is optional. The compiler adds it automatically, but writing it is a clear convention.

C++ is **case sensitive**: `main` and `Main` are different names.

### Namespaces and `std::`
Everything from the Standard Library (`cout`, `cin`, `string`, `vector`…) lives inside the namespace `std`. That's why we write `std::cout`.

`using namespace std;` lets you write `cout` instead of `std::cout`, **but**:
- It dumps hundreds of names (`count`, `max`, `min`, `size`, `remainder`…) into your code, so your own names can **collide** with them:
  ```cpp
  #include <algorithm>
  using namespace std;
  int count = 0;
  int main() { count++; }   // ❌ error: 'count' is ambiguous (mine or std::count?)
  ```
- New C++ versions add new names to `std`, so code can break after an update.
- **Never put it in a header file (`.h`)**: it spreads into every file that includes that header.

| Where | `using namespace std;` |
|---|---|
| Header files | ❌ never |
| Real projects | ❌ avoid, write `std::` |
| Small practice files | ⚠️ OK |
| Inside one function | ✅ OK |
| `using std::cout;` (one name) | ✅ good compromise |

---

## 2. Data types and their limits

### `char`
Holds **one** character, in **single** quotes:
```cpp
char c = 'b';          // ✅
char c = "b";          // ❌ double quotes = string, not char
```

### `int` and its limits
`int` is 32 bits, so it has a fixed range:
```
INT_MIN = -2,147,483,648
INT_MAX =  2,147,483,647
```
The negative side goes one further, because of how negatives are stored (two's complement).

**Overflow:** if a result doesn't fit, the CPU keeps only the lowest 32 bits, like a car odometer rolling over:
```cpp
int x = 100000 * 100000;   // should be 10,000,000,000
                           // actually prints 1,410,065,408 (no warning!)
```
Signed overflow is **undefined behavior**: the language makes no promises about what happens. A silently wrong answer is worse than a crash, because nobody notices.

### `float` vs `double`
| | `float` | `double` |
|---|---|---|
| Size | 4 bytes | 8 bytes |
| Precision | ~6–7 significant digits | ~15–16 significant digits |
| Max value | ~3.4 × 10³⁸ | ~1.8 × 10³⁰⁸ |

Use `double` by default.

### ⚠️ Never compare doubles with `==`
Most decimals can't be stored exactly in binary:
```cpp
0.1 + 0.2 == 0.3                       // false! (it's 0.30000000000000004)
std::fabs((0.1 + 0.2) - 0.3) < 1e-9    // true: "close enough"
```

### `enum`
A set of named constants, used for fixed groups of values:
```cpp
enum Day { Monday, Tuesday, Wednesday };   // Monday = 0, Tuesday = 1, ...
Day today = Tuesday;
```
Prefer `enum class Day { ... };` in modern C++, which keeps the names scoped as `Day::Monday`.

### Strings: `.at()` vs `[]`
- `str[i]`: fast, **no** bounds check. A bad index means undefined behavior.
- `str.at(i)`: checks the index and throws `std::out_of_range` if it's invalid.

Use `[]` when you're sure the index is valid, `.at()` when you're not.

---

## 3. Input and output

### `<iostream>` objects
| Object | Purpose |
|---|---|
| `cin` | Input from the keyboard |
| `cout` | Normal output |
| `cerr` | Error messages (not buffered, appears immediately) |
| `clog` | Log messages |
| `wcin`, `wcout`, `wcerr`, `wclog` | Same, but for wide characters (`wchar_t`) |

`"\n"` vs `std::endl`: both end the line, but `endl` also **flushes** the output (forces it to the screen), which is slower. Prefer `"\n"`.

### `cin >>` vs `getline`
- `cin >> x` reads **one word/value** and stops at a space.
- `std::getline(std::cin, line)` reads the **whole line**, including spaces.
```cpp
// user types: bat bold
std::cin >> name;              // name = "bat"
std::getline(std::cin, name);  // name = "bat bold"
```

### `cin` status flags
`cin` has warning lights. When any is on, **every later `cin >>` does nothing** until `cin.clear()`.

| Flag | Meaning | Check with |
|---|---|---|
| fail | Couldn't read what was asked (e.g. letters for an `int`) | `cin.fail()` |
| eof | **End of input**: there will never be more (Ctrl+Z on Windows, Ctrl+D on Linux/Mac) | `cin.eof()` |

Recovering from bad input:
```cpp
std::cin.clear();                                                    // turn the flag off
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // throw away the bad line
```
**`clear()` cannot fix end of input.** There's nothing left to read. Always check `cin.eof()` and stop, or the program loops forever.

### `cin >>` leaves leftovers in the buffer
`cin >>` reads only what it needs. The rest **stays waiting** for the next read:
```
user types "1.5 2" at the menu
cin >> choice   → reads "1", leaves ".5 2" in the buffer
cin >> num      → reads ".5" without the user typing anything!
```

### ✅ The safe way: read whole lines, then parse them
```cpp
#include <sstream>

bool readNumber(double& out) {
    std::string line;
    if (!std::getline(std::cin, line)) return false;   // input ended
    std::istringstream ss(line);                        // treat the line like a mini cin
    char extra;
    return (ss >> out) && !(ss >> extra);               // exactly one number, nothing after
}
```
| Line | Result |
|---|---|
| `5` | ✅ accepted |
| `5abc` | ❌ leftover `a` |
| `5 9` | ❌ leftover `9` |
| `abc` | ❌ not a number |

`getline` always consumes the whole line, so nothing is left behind. **Rule: don't mix `cin >>` and `getline`; use only `getline` for user input.**

### `<iomanip>`: formatting output
| Manipulator | Does |
|---|---|
| `setprecision(n)` | Number of significant digits for floating-point numbers |
| `setw(n)` | Width of the **next** output item |
| `setfill(c)` | Fill character used with `setw` |
| `setbase(b)` | Base 10, 8 or 16 |
| `quoted(s)` | Read/write strings with spaces inside quotes |

By default `cout` shows only **6 significant digits** for doubles:
```cpp
std::cout << 123456790.0;                           // 1.23457e+08  ← looks wrong!
std::cout << std::setprecision(15) << 123456790.0;  // 123456790
```
`setprecision` **stays in effect** once set, so call it once at the start, not before every output.

---

## 4. Control flow

### `break` vs `continue`
| | In a loop | In a `switch` |
|---|---|---|
| `break` | Leave the loop | Leave the `switch` only |
| `continue` | Skip the rest of this pass, go to the next | Applies to the surrounding **loop** |

### Unreachable code
Anything directly after `break`, `continue` or `return` in the same block **never runs**:
```cpp
continue;
count++;     // ❌ never executes (compiler warning C4702: unreachable code)
```
**Read compiler warnings.** They point straight at bugs like this.

### `switch`
- Only works on integers/chars/enums, not strings.
- Each `case` needs `break`, or it **falls through** into the next case.
- `default` runs when no case matches.

---

## 5. Functions

### Function overloading
Several functions can share a name if their **parameter lists differ** (number or types):
```cpp
int    add(int a, int b);
double add(double a, double b);
int    add(int a, int b, int c);
```

### Passing parameters
| Style | Copies? | Can change the original? | Use for |
|---|---|---|---|
| `void f(int x)` | ✅ yes | ❌ no | Small types (`int`, `double`, `char`) |
| `void f(int& x)` | ❌ no | ✅ yes | When the function must change the caller's variable |
| `void f(const std::string& s)` | ❌ no | ❌ no | Big types you only read (`string`, `vector`) |

### Returning two things: `bool` + reference
```cpp
bool readNumber(double& out);    // returns "did it work?", and the number through `out`

if (!readNumber(num1)) { /* show error */ }
```

### Passing a variable doesn't check it
A parameter does nothing unless the code **inside** the function uses it. If the compiler warns `unreferenced formal parameter`, that parameter is useless; remove it.

### One function, one job
If a function needs dummy arguments like `validate(0, 0, choice)`, it's doing too much. Split it.

---

## 6. Arrays, vectors and lists

### `sizeof` gives **bytes**, not element count
```cpp
int nums[5] = {10, 20, 30, 40, 50};
sizeof(nums);                   // 20  (5 ints × 4 bytes)
sizeof(nums) / sizeof(nums[0]); // 5   (20 bytes ÷ 4 bytes per int)
```
Use `[0]` because the first element always exists. (Modern C++: `std::size(nums)`.)

### `vector` vs `list`
| | `std::vector` | `std::list` |
|---|---|---|
| Layout | One block of memory, like an array | Linked list: each element points to the next |
| Access by index `v[i]` | ✅ fast | ❌ not available |
| Insert/erase in the middle | Slow (elements shift) | Fast |
| Default choice | ✅ **yes** | Only when you really need a linked list |

Erasing by position:
```cpp
tasks.erase(tasks.begin() + index);   // index is 0-based
```

---

## 7. Memory: `new` and `delete`

Normal variables are cleaned up automatically. Memory from `new` is **not**; you must free it yourself:
```cpp
std::string* guests = new std::string[numGuests];   // size decided at runtime
// ... use guests ...
delete[] guests;   // new[] → delete[],  new → delete
```
Forgetting `delete` = **memory leak** (RAM stays used until the program ends).

**Modern C++ avoids raw `new`/`delete`:**
```cpp
std::vector<std::string> guests(numGuests);      // frees itself automatically
auto p = std::make_unique<Circle>(2);            // smart pointer, frees itself
```

### When to `delete`
- Any time **after the last use**, but every `new` needs **exactly one** `delete`.
- ❌ **Leak by reassigning:**
  ```cpp
  Shape* s = new Square(2);
  s = new Circle(1);     // the Square is lost: nothing points to it anymore
  delete s;              // only deletes the Circle
  ```
  ✅ `delete s;` **before** `s = new ...`, or use separate pointers / a vector.
- ❌ Using `s` after `delete s` → dangling pointer (see section 16).
- `delete` inside a loop over a vector leaves the vector full of dangling pointers. Delete in a separate loop at the end, then `clear()`.
- Simplest: don't use `new` when a normal object works (`Square sq(2);` cleans itself up).

---

## 8. Classes

A class bundles **data** (member variables) with the **functions that work on that data**.

```cpp
class BankAccount {
  private:
    double balance = 0;                   // data: hidden from outside

  public:
    BankAccount(double start) {           // constructor: runs when the object is created
        if (start > 0) balance = start;
    }
    void deposit(double amount) {         // the class enforces its own rules
        if (amount > 0) balance += amount;
    }
    double getBalance() const {           // const = promises not to change the object
        return balance;
    }
};

BankAccount a(100);   // each object keeps its OWN data
BankAccount b(50);
```

| Keyword | Meaning |
|---|---|
| `public` | Anyone can use it |
| `private` | Only the class's own functions can use it (**encapsulation**) |
| Constructor | Same name as the class, no return type, runs on creation |
| `const` method | Read-only; the compiler stops it from changing data |

### Test: "What does an object of this class remember?"
If the answer is **nothing**, it's not really a class, just a folder of functions. Use free functions or a `namespace` instead:
```cpp
namespace mathutils { double add(double a, double b); }
```

### Separation rule
A class should do **logic only**: no `cin` or `cout` inside. `main` (or a UI layer) talks to the user.
That makes the class reusable (console, GUI, web) and **testable**.

### Name lookup gotcha
Inside a class, an unqualified name finds the **class member first**:
```cpp
class calculator {
    double pow(int a, int b) {
        return pow(a, b);        // ❌ calls ITSELF forever → stack overflow crash
    }
};
// ✅ fix: return std::pow(a, b);   and/or rename the method to power()
```

### Member hiding (compiler warning C4458)
If a function's parameter or local variable has the **same name as a member**, the local one wins and **hides** the member:
```cpp
class Student {
    double score;                          // member
    char letterGrade(double score) { ... } // ⚠️ this 'score' hides the member
};
```
It may work by accident, but it's confusing. Use different names, e.g. `radius` (parameter) and `r` (member).

### Declaration vs. definition
| | Looks like | Meaning |
|---|---|---|
| **Declaration** | `double area() const;` (ends with `;`) | "This function exists" (the menu) |
| **Definition** | `double area() const { return ...; }` (has `{ }`) | "Here is its code" (the kitchen) |

- Every function needs **exactly one** definition.
- Two definitions → **compiler error** `C2084: function already has a body`
- Declared but never defined (and called) → **linker error** `LNK2019: unresolved external symbol`

In a `.cpp` file, `ClassName::` says which class the function belongs to:
```cpp
double Circle::area() const { return PI * r * r; }
```

### Constructors: initializer list + validation
```cpp
Circle::Circle(double radius) : r(radius) {        // ": r(radius)" sets the member before the body runs
    if (r <= 0) throw std::invalid_argument("Circle: radius must be positive");
}
```
A constructor **can't return** an error, so it **throws**. Then: **if an object exists, it's valid**. Printing an error instead of throwing still creates the broken object.

`explicit` on a one-argument constructor stops accidental conversions (a plain `5` silently becoming a `Circle`).

### Design rule: store what defines it, calculate the rest
A circle stores its **radius**. Area and perimeter are **calculated** in functions, not stored as variables; stored copies could go out of sync with the dimensions.

---

## 8b. Inheritance and polymorphism

### Inheritance: "is a"
```cpp
class Square : public Rectangle {                 // a Square IS A Rectangle
  public:
    Square(double side) : Rectangle(side, side) {}  // pass values to the parent's constructor
    std::string name() const override { return "Square"; }
};
```
Square **inherits** `area()` and `perimeter()` from Rectangle; there's no need to rewrite them.

⚠️ Don't redeclare the parent's variables in the child (`double a;` in Square). That creates a **second, never-set** variable and gives garbage results.
Private members of the parent are **not accessible** in the child.

### Virtual functions: the base class as a contract
```cpp
class Shape {
  public:
    virtual double area() const = 0;       // pure virtual: no code here, every subclass MUST write it
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;
    virtual ~Shape() {}                    // virtual destructor
};
```
| Term | Meaning |
|---|---|
| `virtual` | Call the **real object's** version, even through a `Shape*` or `Shape&` |
| `= 0` (pure virtual) | **Not** "the value is 0"! It means "no code here; subclasses must provide it" |
| Abstract class | Has at least one `= 0` function. `Shape s;` is a compile error; only subclasses can be created |
| `override` | In the subclass: "I'm filling in the parent's virtual function". Compiler error if the name or `const` doesn't match exactly, so it catches typos |
| Virtual destructor | So `delete` through a `Shape*` destroys the **whole** object. **A class with virtual functions needs a virtual destructor.** |

`Shape` holds **no data**, only the list of functions every shape promises. `override` replaces **functions**, not values.

### Polymorphism: one loop, every shape
```cpp
std::vector<Shape*> shapes = { new Circle(2), new Rectangle(3, 4), new Square(5), new Triangle(3, 4, 5) };

for (Shape* s : shapes)
    std::cout << s->name() << ": area " << s->area() << "\n";   // right formula for each

for (Shape* s : shapes) delete s;   // clean up at the END, after the last use
shapes.clear();                     // don't leave dangling pointers in the vector
```
The loop never checks the type; no `if (circle) ... else if (square) ...`. A new shape class works **without changing the loop**.

`p->area()` is a shortcut for `(*p).area()`: follow the pointer, then call the function.

Polymorphism also works through a **reference**, with no `new`/`delete` needed:
```cpp
Square sq(2);
const Shape& s = sq;
s.area();      // calls Rectangle::area() → 4
```

### Destructor order
Deleting a `Square` runs: `~Square` → `~Rectangle` → `~Shape` (child first, then parents). Without a virtual destructor, only `~Shape` would run.

### Most vexing parse ⚠️
```cpp
Circle(-3);       // ✅ creates a temporary Circle (a number can't be a variable name)
double r = -3;
Circle(r);        // ❌ declares a VARIABLE named r of type Circle!
Circle bad(r);    // ✅ give it a name
Circle{r};        // ✅ or use braces
```

---

## 8c. Inheritance in practice (Stage 3b: bank accounts)

### What `virtual` really does (in one sentence)
**When a function is called through a base pointer or reference, `virtual` makes C++ look through it to the real object and run that object's version.**
```cpp
Dog d;
Animal& a = d;      // label: Animal, real object: Dog
a.sound();          // without virtual → Animal::sound() ("...")   — uses the LABEL
                    // with virtual    → Dog::sound()    ("Woof")  — looks at the REAL OBJECT
d.sound();          // always "Woof" — called directly, the type is already known
```
`virtual` is about **which function runs**. It never stores or replaces a value; both versions of the function exist side by side.

### Three kinds of member functions
| Kind | Example | Base class has code? | Subclasses… |
|---|---|---|---|
| Normal (non-virtual) | `void deposit(double)` | ✅ | use it as is; same for everyone |
| `virtual` with a default | `virtual void withdraw(double)` | ✅ a sensible general rule | **may** reuse it or replace it |
| Pure virtual `= 0` | `virtual std::string type() const = 0` | ❌ no sensible default | **must** write their own |

### Which version runs? The nearest one
When a subclass doesn't write a function, C++ uses the **nearest parent** that does:
```
StudentAccount → has withdraw? no
CheckingAccount → has withdraw? YES → this runs (with Student's overdraftLimit = 0)
Account        → not reached
```
Same as `Square` using `Rectangle::area()`.

### Calling the parent's version: `Base::function()`
```cpp
void SavingsAccount::withdraw(double amount) {
    if (counter >= 3) throw std::invalid_argument("max 3 per month");  // my own rule first
    Account::withdraw(amount);   // then the parent's checks + subtraction
    counter++;                   // only reached if nothing threw
}
```
⚠️ Without `Account::`, `withdraw(amount)` calls **itself** → infinite recursion → stack overflow (like bug #2, `pow`).

Reuse vs. replace:
| | Savings | Checking |
|---|---|---|
| Parent's rule ("not more than you have") | still applies → **call** `Account::withdraw` | doesn't apply (overdraft allowed) → **don't call it**, check and subtract yourself |

### `protected`
| | Who can use it |
|---|---|
| `public` | everyone (`main` too) |
| `protected` | the class **and its subclasses** |
| `private` | only the class itself |

Data the subclasses must change (`balance`) → `protected`. Functions the outside world uses (`deposit`, `withdraw`, getters) → `public`. No `setBalance()`: money only moves through functions that check the rules.

⚠️ `class B : A` without a keyword is **private inheritance** → `A* p = &b;` fails (`C2243: conversion exists, but is inaccessible`). Always write `class B : public A`.

### Data vs. functions
- **Data** = what the object **remembers** (`owner`, `balance`, `overdraftLimit`). Never `virtual`.
- **Functions** = what the object **does** (`withdraw`, `monthEnd`). `virtual` only if subclasses need their own version.
- If a value is **different every call** (`amount`), it's a **function parameter**, not a member variable.

### Constructor vs. function: where does the code go?
| Ask | Put it in |
|---|---|
| Happens **once**, when the object is created? (store + validate data) | the **constructor** |
| Happens **every time** something is done? (a withdrawal) | that **function** (`withdraw`) |
| Happens **every month**? | `monthEnd()` |

A constructor that tries to withdraw has no amount yet (bugs #31, #32).

### Every member must get a value
The initializer list sets **only what you list**. Anything missing holds **garbage** (undefined behavior, random results):
```cpp
CheckingAccount(const std::string& owner, double balance, double ODLimit)
    : Account(owner, balance), overdraftLimit(ODLimit) { ... }   // forget overdraftLimit(ODLimit) → garbage
```
Warning `C4100: unreferenced formal parameter` = "you received a value and never stored it".

### Check first, change last
A function that may refuse must refuse **before** touching any data:
```cpp
void withdraw(double amount) override {
    if (amount <= 0) throw ...;                          // check
    if (balance - amount < -overdraftLimit) throw ...;   // check
    balance -= amount;                                   // change — only if all checks passed
}
```
Otherwise a "refused" operation still changes the data (`withdraw(-50)` added 50 — bug #30).

### `void` functions can't be used as values
```cpp
a->monthEnd();                        // ✅ an action on its own line
std::cout << a->monthEnd();           // ❌ nothing to print
```
**Do** the action first, then **ask** for the result: `a->monthEnd(); std::cout << a->getBalance();`

### `main` reacts, objects enforce
- The **objects** check their own rules and `throw` with a reason.
- `main` **catches** and prints `e.what()`. It does **not** re-check the rules or throw itself.
- `try/catch` **inside** the loop, so one refusal doesn't stop the other accounts.
- One loop per **step** (withdraw all → month end all → print once), so order is right and output is readable.

### Getting type-specific information
| Need | Use |
|---|---|
| Every account shows its own extra info | a virtual function, e.g. `virtual std::string details() const` (best) |
| Only some accounts, using common functions | `if (a->type() == "Savings")` (fragile: spelling!) |
| A function only one subclass has | `dynamic_cast<SavingsAccount*>(a)` → `nullptr` if it's not one (use rarely) |
| One specific account you created | keep its own pointer (delete it only once!) |

---

## 8d. Designing classes from scratch (Stages 3c + 3d)

### The method: one row of the rules → one class
| Column of the rules table | Becomes |
|---|---|
| Data it remembers | `private` member variables **and** constructor parameters |
| Price / pay rule | the body of the virtual function (`dailyPrice()`, `pay()`) |
| Type name | `type()` |

Fill in a design table **before** coding: class, parent, members, constructor parameters, what each function returns.

### What goes where?
| Question | Answer |
|---|---|
| Same for **every object of the class**? (`type()`, Bike's 10, Intern's 500) | write it in the class's code |
| Different **per object**? (plate, rate, tons, name) | **data**: member variable, set by the constructor |
| Different **per call**? (`days`, `amount`, new hours) | **function parameter** |
| Shared by **all** subclasses the same way? (`getPlate()`, `getName()`) | a **normal** function in the base class, written once |
| The parent holds what **all** children share; the child adds what makes it special | `bonus` belongs in `Manager`, not in `SalariedEmployee` |

"Is a" works in **one direction**: every Manager is a SalariedEmployee, not every SalariedEmployee is a Manager.

### A non-virtual function that calls a virtual one
```cpp
double rentalCost(int days) const {          // NOT virtual: the same rule for every vehicle, written once
    if (days < 1) throw std::invalid_argument("Days must be at least 1");
    double cost = dailyPrice() * days;       // virtual: each vehicle's own price
    if (days >= 7) cost *= 0.9;              // 10 % OFF = pay 90 % → × 0.9 (× 0.1 would mean "pay 10 %")
    return cost;
}
```
The base class writes the **shared rule**, the subclasses fill in only the **part that differs**.

### How a virtual call really runs (no walking through the classes)
`e.rentalCost(3)` on an `ElectricCar`:
```
rentalCost (in Vehicle) calls dailyPrice()
  → virtual: the object knows its real type → jumps DIRECTLY to ElectricCar::dailyPrice()
      → its code says Car::dailyPrice()  → only NOW Car's version runs → 40
      → + 5 = 45
  → 45 × 3 = 135
```
- It does **not** go Vehicle → Car → ElectricCar step by step. It goes **straight** to the real object's version.
- A parent's version runs **only if the code explicitly asks** (`Car::dailyPrice()`).
- `Vehicle → Car → ElectricCar` is the **family tree** (inheritance), not the order of a call.

### `const` position
```cpp
const double rentalCost(int days)      // ❌ returns a "const double" (useless)
double rentalCost(int days) const      // ✅ the function doesn't change the object
```
Through `const Vehicle*` / `const Vehicle&` only `const` functions can be called.

### Every door into the object needs its own lock
The constructor checks values at creation; a setter (`setHours`) checks every later change. Missing either = a way to break the object.
A setter can reuse the rule: the constructor calls `setHours(h)`, so the rule lives in **one** place.

### Assignment direction
`=` copies **right → left**: `hour = h` means "hour **becomes** h". `h = hour` throws the new value away.

### Totals in a loop
| Where | What |
|---|---|
| **before** the loop | `double total = 0;` (inside the loop it would reset every pass) |
| **inside** the loop (with `{ }`!) | `total += ...;` on its own line, not inside a `cout` chain (`<<` runs before `+=`) |
| **after** the loop | print once |

Without `{ }`, a `for` repeats only the **next single statement**, whatever the indentation says.

### Creating objects in `main`
```cpp
Car c("CR-1", 40);                      // plain object: c.rentalCost(3), cleaned up automatically
std::vector<Vehicle*> v;                // for polymorphic loops
v.push_back(new Car("CR-1", 40));       // v[i]->rentalCost(3), needs delete at the end
Vehicle x("AB-1");                      // ❌ C2259: abstract class
```

---

## 9. Exceptions

### How `throw` works
`throw` stops the current function and searches **outwards** through the callers for a matching `catch`. If none is found, `std::terminate()` **kills the program**.

```cpp
double divide(double a, double b) {
    if (b == 0) throw std::invalid_argument("Division by zero");   // needs <stdexcept>
    return a / b;
}

try {
    divide(5, 0);
} catch (const std::invalid_argument& e) {   // catch by const reference
    std::cout << e.what() << "\n";
}
```

### The safety net
`try { }` is a safety net: it **only catches what falls inside it**.
```
validate(choice);        ← throws here → no net → 💥 crash
try {
    validate(choice);    ← throws here → caught ✅
}
```

### When to `throw` vs when to use `if`
| Situation | Use |
|---|---|
| You can handle the problem **right here** (bad menu input in `main`) | `if` + `continue` / `return` |
| A function **can't** do its job and must tell its caller (`divide(5, 0)`) | `throw`, with `try/catch` at the caller |

### Counting only successes
Code after a throwing call inside `try` is **skipped** when an exception happens:
```cpp
try {
    std::cout << calc.divide(a, b);
    count++;              // only reached if divide didn't throw ✅
} catch (...) { }
```

---

## 10. Math functions (`<cmath>`)

| Function | Does | Note |
|---|---|---|
| `std::pow(a, b)` | aᵇ | Handles negative exponents: `pow(2, -1) = 0.5` |
| `std::fmod(a, b)` | Remainder for **doubles** | `%` only works on integers |
| `std::fabs(x)` | Absolute value of a double | Used for comparing doubles |
| `std::sqrt(x)` | Square root | |

### Integer edge cases
- `x / 0` and `x % 0`: crash / undefined. Always check for 0.
- `INT_MIN % -1` and `INT_MIN / -1`: the result (+2,147,483,648) doesn't fit in an `int`. On x86 the CPU raises a **hardware exception** and the program dies. `try/catch` **cannot** catch this.
- Switching to `double` + `fmod` avoids both overflow and this crash.

### `<cstdlib>`
General utilities: `rand()`, `srand()`, `exit()`, `abs()`, memory functions.

Random numbers need a seed, or you get the **same number every run**:
```cpp
#include <ctime>
std::srand(std::time(0));        // once, at the start of main
int r = std::rand() % 100 + 1;   // 1–100
```
(Modern C++: `<random>` with `std::mt19937` is better.)

---

## 11. Project structure: library, app, tests

```
c++/
├── inc/                      ← LIBRARIES (headers): logic only, no cin/cout
│   ├── calculator.h          ← header-only
│   ├── student.h             ← header-only
│   └── shape.h               ← declarations only; code is in stage3/shape.cpp
├── stage1/main.cpp           ← APPS: one program per folder
├── stage2/stage2.cpp
├── stage3/main.cpp, shape.cpp
├── test/*.test.cpp           ← TESTS: one test program per library
└── build/                    ← compiled output (not in git)
```
- The app and the test `#include` the same library. Each has its own `main()` and builds into its own `.exe`.
- **One program = one `main()`.** Two `.cpp` files with `main()` built together → `LNK2005: main already defined`. Keep one program per folder.
- **Header-only library**: all code is in the `.h`, so including it is enough.
- **Split library** (`shape.h` + `shape.cpp`): every program that uses it must **also compile `shape.cpp`**, or the linker fails.
- `#pragma once` at the top of every **header** (never in a `.cpp`).
- A header must `#include` everything it uses. Don't rely on the `.cpp` including `<iostream>` first.
- `#include "..."` paths are **relative to the file that contains them**. After moving a file, fix its includes (`"inc/x.h"` → `"../inc/x.h"`).
- Use **`/`** in include paths. It works on every OS; `\` only works on Windows.
- C++ headers have **no `.h`**: `<string>`, `<cmath>`, `<cstdlib>`. `<string.h>` is the old **C** header (`strlen`…) and does **not** contain `std::string`.
- **Namespaces** prevent name clashes when several stages are combined into one program: `stage1::ReadInt` and `stage2::ReadInt` are different functions.

---

## 12. Unit testing

**Unit test** = a small program that calls functions with known inputs and checks the outputs **automatically**.

My hand-made test helpers:
```cpp
int passed = 0, failed = 0;

void check(const std::string& name, double actual, double expected) {
    if (std::fabs(actual - expected) < 1e-9) passed++;
    else { failed++; std::cout << "FAIL: " << name << " expected " << expected
                               << " but got " << actual << "\n"; }
}

template <typename Func>
void checkThrows(const std::string& name, Func f) {
    try { f(); failed++; std::cout << "FAIL: " << name << " no exception\n"; }
    catch (const std::invalid_argument&) { passed++; }
}

check("divide", calc.divide(5, 2), 2.5);
checkThrows("divide by zero", [&] { calc.divide(5, 0); });   // [&]{...} = lambda

return failed == 0 ? 0 : 1;    // exit code: 0 = all passed
```

### What to test
1. Normal case: `add(2, 3)`
2. Negatives and zero: `add(-5, 3)`, `multiply(5, 0)`
3. Decimals: `add(0.1, 0.2)`
4. Big numbers: `multiply(100000, 100000)`
5. Error cases: `divide(5, 0)` **must** throw
6. **Every bug I've fixed**, so it can never come back unnoticed

7. **Boundaries**: `90` is A but `89.9` is B; `Triangle(1, 2, 3)` (flat) must throw
8. **Each condition separately**: for `if (a <= 0 || b <= 0)`, test **only a** wrong and **only b** wrong. Testing both wrong at once can't tell `||` from `&&`.
9. Things that must **not** throw (valid inputs near the boundary)

### Testing `void` functions
A `void` function returns nothing, so check **what it changed**:
| A function… | Test it by… |
|---|---|
| returns a value | checking the return value |
| writes to **reference parameters** (`findMinMax`) | checking those variables after the call. Start them at a "wrong" value like `-1`, so a function that forgot to write them fails |
| **modifies** a container (`curveScores`) | checking the container after the call |
| returns a **pointer** (`findScore`) | checking `nullptr`, and `p == &v[1]` (the address of the real element) |
| **throws** | `checkThrows` |
| **prints** | give it a `std::ostream& out = std::cout` parameter; the test passes a `std::ostringstream` and checks `out.str()` |

### Good test habits
- **Fresh data for each test.** Tests that share a vector break each other.
- **Unique test names**, so a failure says exactly what broke.
- **Return the right exit code**: `return failed == 0 ? 0 : 1;`. `return 0;` always says "success".
- `check` only prints **failures**, so always print a summary (`Passed: X, Failed: Y`).
- **When a test fails, decide which is wrong: the code or the test.** (`letterGrade(-5)`: throw, or return 'F'? Choosing the correct behaviour is part of writing tests.)
- One `check` = one value. `check("x", (a, 8, b, 4))` does **not** check two things; the **comma operator** keeps only the last value. Write a helper instead:
  ```cpp
  void checkShape(const std::string& label, const Shape& shape, double area, double perimeter) {
      check(label + " area", shape.area(), area);
      check(label + " perimeter", shape.perimeter(), perimeter);
  }
  ```
- In tests, prefer plain objects + references over `new`. Nothing to `delete`, no leaks.
- **A refusal must leave the object unchanged**: after `checkThrows`, also `check` the value. `checkThrows` alone can't see "changed, then threw".
  ```cpp
  checkThrows("negative amount", [&]{ c.withdraw(-50); });
  check("refused withdrawal leaves balance", c.getBalance(), 1000);
  ```
- **Any call that might throw goes inside `checkThrows` / `checkNoThrow`**, also setup steps. A plain call that throws crashes the whole test program (exit code `-1073740791`, no `FAIL` line, later tests never run).
- A test that **can never fail** tests nothing (student `monthEnd` when a student can never be negative).
- A `check` on a call that might wrongly throw → put the check **inside** `checkNoThrow`, so a bug gives a FAIL line instead of a crash:
  ```cpp
  checkNoThrow("1 day allowed", [&]{ check("1 day costs 40", c.rentalCost(1), 40); });
  ```
- **Which function throws?** Constructor errors → create the object inside the lambda (`[&]{ Car{"CR-3", -40}; }`). Function errors → call it on an existing object (`[&]{ c.rentalCost(0); }`). `Car.dailyPrice{...}` is wrong: `Car` is the class, `.` needs an object.
- Some decisions are intentional (e.g. not validating a value): write a **comment** and a `checkNoThrow` test, so it's documented and nobody "fixes" it by accident.
- No test suite checks "everything possible". Aim for: every rule, every boundary, every error, every past bug — and measure with mutation testing.

### Lambdas: `[&]{ ... }`
A small unnamed function written in place. `checkThrows` needs it so it can run the code **inside its own `try`** (a plain call would run — and throw — before `checkThrows` even starts).
| Capture | Meaning |
|---|---|
| `[&]` | use outside variables **by reference** (the real `bat`) |
| `[=]` | use **copies** (read-only inside the lambda) |
| `[]` | use no outside variables |

### Shared test helpers: `test/test.helper.h`
`check`, `checkStr`, `checkThrows`, `checkNoThrow`, `report()`. Every test ends with `return report();` (prints the summary **and** returns the exit code).
- Variables/functions defined in a header need `inline` (`inline int passed = 0;`) — **inline variables need C++17**.
- Test helpers live in `test/`, not `inc/` (`inc/` is for the libraries my apps use).

### Mutation testing: are my tests any good?
Two different questions:
| Question | How to check |
|---|---|
| Is my **code** correct? | Run the tests on the real code. All pass → ✅ |
| Are my **tests** good? | **Break the code on purpose**, one small change at a time (`\|\|` → `&&`, `<=` → `<`, delete a check). Tests fail → ✅ good. Tests still pass → 🔴 write a test that catches it. Then **undo the change**. |

With correct code, good and weak tests look the same (all pass). Only a deliberate bug shows the difference.
My shape tests: before adding separate-condition tests, `Rectangle` with `&&` and a deleted triangle check both passed **10/10**. Afterwards, they were caught (3 and 2 failures).
My account tests: first version caught **0 of 4** mutants (17/17 passed). Final version: **33 tests, 8 of 8 mutants caught** (subtract-before-check, fee at exactly 0, student overdraft, deposit doing nothing, deposit(0) accepted, counter not reset, no interest).
My employee tests: 1 test caught 0 of 4 → 19 tests caught 6 of 7. My vehicle tests: **40 tests, 7 of 7 caught**.

**Equivalent mutant:** a change that doesn't alter behaviour, so **no** test can catch it. Example: overtime `hour <= 160` → `hour < 160`. At exactly 160 both give `rate × 160` (+ 0 overtime). Not a gap in the tests.

### Testing the whole program
```bash
build\main.exe < test_input.txt
```
`<` feeds the file in as if typed. Always test: bad input, out-of-range choices, **two operations in a row**, and end of input (Ctrl+Z).

### Test tools in the real world
| Name | What it is |
|---|---|
| **GoogleTest** | Most common framework in industry (`EXPECT_EQ`, `EXPECT_THROW`) |
| **Catch2 / doctest** | Simpler frameworks; doctest is one header file |
| **CTest** | CMake's test **runner**. It runs test `.exe` files and checks the exit code |

---

## 13. Compiling and build files

C++ builds in two steps: **compile** (`.cpp` → `.obj`) and **link** (`.obj` + libraries → `.exe`).

MSVC (`cl.exe`) output files:
| File | What it is | Keep? |
|---|---|---|
| `.obj` | Compiled machine code, not yet a program | No |
| `.exe` | The program | Yes |
| `.pdb` | Debug info (lets the debugger show my source lines and variables). Created by `/Zi` | Only for debugging |
| `.ilk` | Incremental link data (faster rebuilds) | No |
| `vc140.pdb` | Compiler debug info | No |

All of these can be deleted anytime; the next build recreates them.

My VS Code `tasks.json` sends everything to one root folder:
```
/Fo${workspaceFolder}\build\    → .obj
/Fd${workspaceFolder}\build\    → vc140.pdb
/Fe${workspaceFolder}\build\... → .exe (+ .pdb, .ilk)
```
- `cl.exe` won't create the `build` folder; it must already exist.
- Two `.cpp` files with the same name overwrite each other's `.exe`.

**g++** (MinGW/MSYS2) leaves only the `.exe`: temporary object files are deleted, and `-g` debug info goes inside the `.exe`.
```bash
g++ main.cpp -o main.exe
```

Useful compiler flags: `/W4` (MSVC) or `-Wall -Wextra` (g++) turn on more warnings. Warnings found real bugs in my code.

### Compiler errors vs. linker errors
| Starts with | Who complains | Meaning | Usual cause |
|---|---|---|---|
| `C2xxx` / `C3xxx` | **Compiler** | My code has a mistake | Typo, duplicate definition, missing `#include` |
| `LNKxxxx` | **Linker** | Something was declared but its code wasn't found, or was found twice | A `.cpp` file wasn't compiled in; a function never written; two `main()`s |

Seeing `LNK2019: unresolved external symbol`? Ask: **which `.cpp` file did I forget to compile in?**

### Building several files
```bash
cl /EHsc /W4 stage3\main.cpp stage3\shape.cpp /Fe:build\stage3.exe
```
My VS Code tasks (Ctrl+Shift+B shows the list):
| Task | Use for |
|---|---|
| **build active file** | Single-file programs and header-only tests |
| **build all .cpp in active file's folder** | Multi-file programs like `stage3/` → `build\stage3.exe` |
| **build shape tests** | `test\shape.test.cpp` + `stage3\shape.cpp` |

### Build ≠ run
Building only **creates** the `.exe`; nothing is printed. Run it yourself in PowerShell:
```bash
.\build\stage3.exe
```
PowerShell needs `.\` ("this folder") to run a local program.

| You see | Meaning |
|---|---|
| `error C2xxx` / `LNKxxxx`, no "Build finished successfully" | the **build** failed |
| "Build finished successfully", then the program prints nothing / exits strangely | the **program** crashed (usually an uncaught exception) |

In the Ctrl+Shift+B list, pick **my** tasks (marked "Task generated by Debugger" / my descriptions). The ones under *detected tasks* are VS Code's defaults without my flags.

### Which C++ version am I using?
- Compiler: run `cl` → `Version 19.40` = Visual Studio 2022 (17.10).
- Standard: print `_MSVC_LANG` (`201402` = C++14, `201703` = C++17, `202002` = C++20). On MSVC `__cplusplus` says `199711` unless `/Zc:__cplusplus`. On g++ use `__cplusplus` / `g++ --version`.
- **MSVC defaults to C++14.** My tasks use `/std:c++20` (g++: `-std=c++20`). Without it: `C7525: inline variables require at least '/std:c++17'`.

### Header-only or not?
Functions in the header end with `{ ... }` → header-only, including it is enough (calculator, student, account).
They end with only `;` → the code is in a `.cpp` that must be compiled in (shape). Missing it → `LNK2019`.

---

## 14. Git

Don't commit build output: it's large, changes every build, can be recreated, and only works on my machine.

`.gitignore`:
```gitignore
# no slash = matches a "build" folder at ANY level
build
*.exe
*.obj
*.pdb
*.ilk
# passwords / secrets
.env
```
Comments in `.gitignore` must be on their **own line**. `build  # comment` would be treated as part of the pattern.
`.gitignore` doesn't remove files that are **already** committed:
```bash
git rm -r --cached build
```

---

## 15. Databases (overview)

**SQL** is the **language**. PostgreSQL, MySQL, MS SQL Server and SQLite are **databases** that speak it.

| | Embedded (**SQLite**) | Server (**PostgreSQL**, **MS SQL**, MySQL, Oracle) |
|---|---|---|
| Analogy | My wallet / personal safe | A bank |
| Runs as | A library inside my app, one `.db` file | A separate always-on program |
| Users | One app, one machine | Many users at once, over the network |
| Setup | None | Install/rent a server, users, passwords |
| Deploy | Ship the app | App **+** database server (or managed: Supabase, AWS RDS, Azure) |

- **PostgreSQL**: free, open source, fastest growing, popular with startups and tech companies.
- **MS SQL Server**: Microsoft, paid (free Express edition), common in companies using Windows, C#/.NET and Azure. Uses the **T-SQL** dialect (`TOP 10` instead of `LIMIT 10`).
- **Oracle / Db2**: banks, governments, large enterprises.
- **NoSQL**: MongoDB (documents), Redis (fast in-memory cache).

C++ → PostgreSQL uses the **libpqxx** library. Never put connection passwords in code; use environment variables.

My plan: learn C++ fundamentals first, then SQL, then connect them.

---

## 16. Pointers and references

### Pass by value vs. pass by reference
**Pass by value** (`void f(int x)`)
- The function gets a **copy**. The original value is not modified.
- Suitable when you don't want to alter the original data, and for small types.

**Pass by reference** (`void f(int& x)`)
- The function works on the **original variable itself** (another name for it, no copy). Changes affect the caller's value.
- Suitable when you want to modify the original data, or to avoid copying large data.
- Add `const` (`const std::vector<double>& v`) to avoid the copy **and** forbid changes:
  - `&` = **no copy**
  - `const` = **can't change it**

```cpp
void byValue(int x)     { x = 99; }   // changes the copy only
void byRef(int& x)      { x = 99; }   // changes the caller's variable

int a = 1;
byValue(a);   // a is still 1
byRef(a);     // a is now 99
```

### Range-based for: copy vs. reference
```cpp
for (double s : scores)  s += 5;   // ❌ changes copies, vector unchanged
for (double& s : scores) s += 5;   // ✅ changes the real elements
for (const auto& name : names) std::cout << name;   // ✅ read-only, no copies
```

### Pointers
A pointer is a variable that holds a **memory address**.
```cpp
double score = 90;
double* p = &score;    // &score = "address of score"
std::cout << p;        // prints an address, e.g. 0x7ffd...
std::cout << *p;       // *p = "the value at that address" → 90
*p = 95;               // changes score through the pointer → score is now 95
```
| Symbol | In a declaration | In an expression |
|---|---|---|
| `*` | `double* p`: p is a pointer to double | `*p`: the value p points to (dereference) |
| `&` | `double& r`: r is a reference | `&x`: the address of x |

### `nullptr`: "points to nothing"
Used to say "not found" or "no object yet". **Always check before using `*p`.** Dereferencing `nullptr` crashes the program.
```cpp
double* findScore(std::vector<double>& scores, double target) {
    for (double& s : scores)
        if (s == target) return &s;   // address of the REAL element
    return nullptr;                   // not found
}

double* found = findScore(scores, 90);
if (found) {          // same as: if (found != nullptr)
    *found = 95;      // changes the score inside the vector
}
```

### Reference vs. pointer
| | Reference `int&` | Pointer `int*` |
|---|---|---|
| Can be "nothing" | ❌ must always refer to something | ✅ can be `nullptr` |
| Can change what it refers to | ❌ fixed forever | ✅ can point somewhere else |
| Syntax to use the value | Just `r` | `*p` |
| Use for | Function parameters | "Maybe not found", optional values, dynamic memory |

**Rule:** prefer references. Use a pointer when "nothing" must be a possible answer.

### ⚠️ Dangling pointers
A **dangling pointer** still holds an address, but the memory there is **no longer valid**: it was freed or moved. Using it is **undefined behavior**: it may print garbage, crash, or *seem to work*. "Seems to work" is the dangerous part; the bug can stay hidden for a long time.

**Cause 1: the vector moved its elements (`push_back`)**

A `vector` keeps its elements in one block of memory with room for a limited number of elements (its **capacity**). When it's full and you `push_back`:
1. it allocates a **new, bigger** block somewhere else,
2. moves all the elements there,
3. **frees the old block**.

```cpp
std::vector<double> scores = {90, 80};
double* p = &scores[0];     // points into the old block
scores.push_back(70);       // may reallocate → old block freed
*p = 100;                   // ❌ undefined behavior: p is dangling
```
If there's still spare capacity, nothing moves and `p` stays valid. That's why the bug is **unpredictable**, not "always broken".
The same applies to iterators and references into the vector ("**iterator invalidation**"); `insert` and `erase` can do it too.

**Fix:** don't keep pointers into a vector across `push_back`/`insert`/`erase`. Store an **index** instead, or look the element up again afterwards.

**Cause 2: pointing to a local variable that has ended**
```cpp
int* bad() {
    int x = 5;
    return &x;      // ❌ x is destroyed when the function returns
}
```

**Cause 3: using memory after `delete`**
```cpp
int* p = new int(5);
delete p;
*p = 10;            // ❌ use after free
p = nullptr;        // ✅ habit: set to nullptr after delete
```

Smart pointers (`std::unique_ptr`) and containers like `vector` prevent most of these problems. That's why modern C++ avoids raw `new`/`delete`.

---

## 17. Bug log: bugs I found and fixed

| # | Bug | Cause | Fix | Lesson |
|---|---|---|---|---|
| 1 | Same "random" number every game | `rand()` never seeded | `srand(time(0))` | Random needs a seed |
| 2 | `pow` crashed the program | Method called itself (name lookup finds the member first) | `std::pow` + rename to `power` | Be explicit with `std::` |
| 3 | `100000 * 100000 = 1410065408` | `int` overflow (rolls over silently) | Use `double` | Know your type's limits |
| 4 | `INT_MIN % -1` crashed | Result doesn't fit in `int`; the CPU raises a hardware exception | `double` + `fmod` | Edges (`0`, `-1`, `INT_MIN`) hide bugs |
| 5 | Infinite loop when input ended | `cin.clear()` can't create new input | `if (cin.eof()) break;` | Ask: "what if there's no more input?" |
| 6 | Bad input crashed the program | `throw` outside of `try` | Move into `try`, or use `if` instead | `try` only catches what's inside it |
| 7 | `123456790` shown as `1.23457e+08` | `cout` shows 6 digits by default | `setprecision(15)` | Display ≠ value |
| 8 | Count always 0 | `count++` after `continue` (unreachable) | Move it after the `switch`, inside `try` | Read compiler warnings |
| 9 | Second calculation skipped | Extra `cin >> choice` ate the user's input | Remove it | Test two operations in a row |
| 10 | `1.5` at the menu ran Add with 0.5 | `cin >>` leaves leftovers in the buffer | `getline` + `istringstream` | Read whole lines, then parse |
| 11 | `validateInput` had useless params | Passing a variable ≠ checking it | Remove parameters / split function | One function, one job |
| 12 | Grades: scores 0 and 100 rejected | `score <= 0 \|\| score >= 100` (off by one) | `score < 0 \|\| score > 100` | Test the exact boundaries |
| 13 | Grades: program never ended | `while(true)` with no `break` / no `eof` check | `eof` check after every read + `0` to quit | Every loop needs a way out |
| 14 | Grades: old students appeared in the next round | `clear()` at the end of the loop, skipped by `continue` | `clear()` at the **start** of the loop | Setup at the start, not cleanup at the end |
| 15 | Grades: extra prompt after input ended | `break` inside `for` only leaves the `for`, not the `while` | Check `eof` again after the inner loop | `break` exits only the innermost loop |
| 16 | Grades: curved score could go negative | Negative bonus not handled | Clamp to 0 in `curveScores` | Functions should protect their own rules |
| 17 | Grade tests: exit code 0 even with failures | `return 0;` at the end of the test | `return failed == 0 ? 0 : 1;` | Tools trust the exit code |
| 18 | `Student`: 5 × warning C4458 | Members `name`/`score` hidden by parameters with the same name | Remove the unused members | Don't half-build a class |
| 19 | Shapes: `Square(5)` area `0`, perimeter `2.78e-309` | Square declared its **own** `double a;`, never set | Delete it; inherit `area()`/`perimeter()` from Rectangle | Inherit, don't redeclare |
| 20 | Shapes: `Circle(-3)` was still created | Constructor printed "wrong size" instead of throwing | `throw std::invalid_argument` | Constructors throw |
| 21 | Shapes: `Triangle(1, 2, 10)` was created | `validTriangle()` existed but nothing called it | Check in the constructor, use `<=` | Validate where the object is born |
| 22 | Shapes: `C2084 already has a body` | `name()` defined in the `.h` **and** the `.cpp` | Keep one definition | One function, one body |
| 23 | Shapes: `C2509 not declared in 'Square'` | Defined `Square::Rectangle::area` for an inherited function | Delete it | Inherited functions need no definition |
| 24 | Shapes: `LNK2019 unresolved external` | Constructors declared but never written; later `shape.cpp` not compiled in | Write them; build all `.cpp` files together | `LNK` = missing code, not a typo |
| 25 | Shapes: Heron's formula wrong | `a+b+c/2.0` = `a + b + (c/2)` (precedence) | `double s = (a + b + c) / 2.0;` | `*` and `/` before `+`; use a variable |
| 26 | Shapes: `'string' is not a member of 'std'` | `#include <string.h>` (C header) | `#include <string>` | C++ headers have no `.h` |
| 27 | Moved files no longer compiled | `#include "inc/x.h"` is relative to the file | `"../inc/x.h"` | Fix includes after moving files |
| 28 | Shape tests passed even with broken code | Throw tests used both sides invalid; no triangle throw tests | Test each condition separately | Mutation testing proves tests work |
| 29 | Account: `owner`/`balance` written as virtual functions | Mixed up data and functions | `protected: std::string owner; double balance;` | Data stores, functions do; data is never virtual |
| 30 | Checking: `withdraw(-50)` was refused but the balance changed | `balance -= amount;` **before** the checks | Check first, change last | A refusal must leave everything unchanged |
| 31 | Savings: 4th withdrawal allowed | Withdrawal counting written in the **constructor** | Override `withdraw()`, count there | Constructor runs once; functions run every call |
| 32 | Checking/Student: random results, student couldn't be created | `overdraftLimit` not in the initializer list + `amount` as an unset member, withdrawn in the constructor | `overdraftLimit(ODLimit)`; `amount` as `withdraw`'s parameter | Every member gets a value; per-call values are parameters |
| 33 | Account: `C2665 cannot convert 'double' to 'double (*)(void)'` | Parameter written `double initialBalance()` | `double initialBalance` | `()` makes it a function |
| 34 | Nothing could call `deposit()` (`C2248`) | Everything under `protected:`, no `public:` | Data `protected`, functions `public` | Each label applies until the next one |
| 35 | `C2243 conversion ... inaccessible` | `class SavingsAccount : Account` (private by default) | `: public Account` | Always write `public` |
| 36 | Savings `withdraw` did nothing, 4 × "OK" | Lines after `throw` inside the `if`'s braces (`C4702`) | Close the `if` right after `throw` | Lines that always run go outside the `if` |
| 37 | `StudentAccount` declared its own `overdraftLimit` | Redeclared a parent's member (like bug #19) | Pass `0` to the parent's constructor instead | Children don't redeclare parents' data |
| 38 | `void monthEnd() { return balance; }` (`C2562`) | Returning a value from `void` | Empty body `{}` | `void` = nothing to give back |
| 39 | Stage 3b `main` crashed immediately | `main` threw on a non-savings account; `while(true)` with no exit | Objects throw, `main` catches; no infinite loop | `main` reacts, objects enforce |
| 40 | `cout << accounts[i]` printed addresses | Printing a pointer | `a->type()`, `a->getOwner()`, `a->getBalance()` | A pointer's value is an address |
| 41 | `cout << a->monthEnd()` didn't compile | Using a `void` call as a value | Call it, then print `getBalance()` | Do first, then ask |
| 42 | Month-end table printed 3 times | `printAccounts` inside the loop over accounts | Loop for the action, print once after | Know which lines repeat |
| 43 | "4 withdrawals" withdrew only once | Looped over accounts, not over 4 attempts | `for (int i = 1; i <= 4; i++)` with `try` inside | Loop over what should repeat |
| 44 | `accounts.clear()` leaked memory | `clear()` without `delete` | `for (Account* a : accounts) delete a;` first | `clear()` only empties the list of pointers |
| 45 | Tests: `C7525 inline variables require /std:c++17` | MSVC defaults to C++14 | `/std:c++20` in all tasks | Know your C++ standard |
| 46 | Tests "build failing" (actually crashing) | `fee.withdraw(1150)` past the limit, outside `checkThrows` | Correct amount; wrap risky calls in `checkNoThrow` | Build ≠ run; uncaught exception = crash |
| 47 | Test expected a fee on a positive balance | Test disagreed with the rule (fee only when negative) | Fix the test, add the 0 boundary | Decide the rule first, then the test |
| 48 | Payroll: `bonus` and `hour` in `SalariedEmployee` | Mixed several types' data into one class | One class per row of the rules; bonus in `Manager` | The parent holds only what all children share |
| 49 | Payroll: `C2665` creating an employee with `"Anna"` | `std::string&` (non-const) can't bind to a literal | `const std::string&` | Always `const&` for input strings |
| 50 | `manager::pay()` crashed | `return pay() + bonus;` called itself | `SalariedEmployee::pay() + bonus` | Calling the parent needs `Parent::` (bug #2 again) |
| 51 | Hourly pay was garbage (`-2.09e-310`) | Constructor didn't store `hour` / `hourlyRate` (`C4100`) | Store them in the initializer list | Read `unreferenced formal parameter` warnings |
| 52 | `setHours` never changed the pay | `h = hour;` (wrong direction), first after `throw` | `hour = h;` after the check | `=` copies right → left |
| 53 | Overtime paid for **short** weeks | `if (hour >= 160)` branches swapped | `if (hour <= 160)` normal pay | Read conditions out loud |
| 54 | Employee tests "ran shape.test.exe" | Picked "build all .cpp in folder" in `test/` (several `main`s) and "build shape tests" | "build active file" for each test | Read the task name before Enter |
| 55 | Vehicle plate always empty | Base constructor didn't store `Plate` | `: Plate(Plate)` | Receiving ≠ storing |
| 56 | `getPlate`/`rentalCost` written 4–5 times | Made them virtual / copied into each subclass | One normal function in `Vehicle` | Same for all → write once in the base |
| 57 | `const double rentalCost(...)` couldn't be called on `const Vehicle*` | `const` in front = const return value | `double rentalCost(int days) const` | `const` goes **after** the `( )` |
| 58 | 3-day car rental cost 12 | `× 0.1` = "pay 10 %", branches swapped | `× 0.9` only when `days >= 7` | 10 % off = pay 90 % |
| 59 | Exactly 7 days got no discount | `days > 7` | `days >= 7` | "7 or more" = `>=`; test 6 vs. 7 |
| 60 | ElectricCar price 5 instead of 45 | Passed `Car`'s own unset `dailyRate` to `Car`'s constructor | Take a `rate` parameter, pass it up | Values come from the caller, not from unset members |
| 61 | Truck price ignored tons | `return dailyRate;` | `dailyRate + 15 * tons` | Compare every function with the rules table |
| 62 | `total` didn't add up / didn't compile | No `{ }` on the `for`, `total = 0` inside the loop, `+=` inside `cout` | Declare before, add inside (own line), print after | Braces + "before / inside / after" |
| 63 | A test with a plain `check(..., c.rentalCost(1), ...)` crashed on a mutant | Call that might throw outside `checkNoThrow` | Put the check inside `checkNoThrow` | Wrap anything that might throw |
