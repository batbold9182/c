#include <iostream>
#include <vector>
#include "../../inc/account.h"
#include <stdexcept>
/* 
    0.1 - prints "woof meow"  Dog d;  Cat c; calls dog as d cat as c
                            Animal& a1 = d; class animal points at d which is dog
                            Animal& a2 = c; class animal points at c which is cat
    0.2 - virtual gives const value with override i can update that value on my chosen class for example base class gets val : "BASE" 
    i can override it in my subclass like val:"overridden" and then if i try to get val like base.subclass.val -> it will return "overridden"
    0.3 - return 42 exactly as i said on 0.2 its overriding base class with its subclass 
    0.4 - no need to return side *side since square already gets side from rectangle class
    0.5 - for variable it's assigned value 0. for function its pure virtual function which makes other subclasses possible to create their own function
          without depending on base class.

    ===== PART 8 =====
    1. Why is withdraw virtual but deposit not?
       Withdraw rules are different for each account type (savings: max 3 per month,
       checking: may go down to -overdraftLimit, student: never below 0), so each subclass
       needs its own version, and a call through Account* must run the REAL object's version.
       Deposit works the same for every account, so no subclass needs its own version
       and there is nothing to look up.

    2. Why is type() pure virtual (= 0) but withdraw has a default version?
       There is no sensible default for type(): a plain "Account" doesn't exist, every real
       account must say what it is. So Account has no code for it and every subclass MUST write it.
       withdraw DOES have a sensible general rule ("you can't take out more than you have"),
       so Account provides it. Subclasses MAY reuse it (Savings calls Account::withdraw)
       or replace it (Checking writes its own rule with the overdraft).

    3. protected vs private, why not public?
       public:    anyone could write a->balance = 1000000 and skip every rule.
       private:   only Account could change balance, so CheckingAccount::withdraw and
                  SavingsAccount::monthEnd couldn't do their jobs.
       protected: Account and its subclasses can change it, the outside world must go through
                  deposit() and withdraw(), which check the rules (encapsulation).

    4. What if SavingsAccount::withdraw called withdraw(amount) without Account:: ?
       Inside a class a name finds the class's OWN member first, so withdraw(amount) would call
       SavingsAccount::withdraw = itself. It would call itself again and again, counter++ is never
       reached, until the program crashes with a stack overflow (same as bug #2, pow calling pow).
       Account:: says explicitly: run the PARENT's version.

    5. a->withdraw(150), a is Account* pointing to a StudentAccount. Which code runs?
       withdraw is virtual, so C++ looks through the Account* to the real object: a StudentAccount.
       StudentAccount does not write its own withdraw, so C++ uses the nearest parent that does:
       CheckingAccount::withdraw runs, with overdraftLimit = 0 (passed by Student's constructor).
       It checks amount <= 0, then balance - amount < -0, and only then subtracts.
       Account::withdraw does NOT run, and no StudentAccount code runs at all.
*/
void printAccounts(const std::vector<Account*>& accounts) {
    for (const Account* a : accounts)
        std::cout << a->type() << "  " << a->getOwner() << "  " << a->getBalance() << "\n";
}
int main(){
    std::vector<Account*> accounts;
    accounts.push_back(new CheckingAccount("annna", 1000 ,100));
    accounts.push_back(new SavingsAccount("banna", 1000 ,0.05));
    accounts.push_back(new StudentAccount("canna", 100));
    
    std::cout<<"#####ACCOUNTS######\n";
    printAccounts(accounts);

    std::cout << "\n=== Withdraw 150 from every account ===\n";
    for (Account* a : accounts) {
        try {
            a->withdraw(150);
            std::cout << a->type() << " " << a->getOwner() << ": withdrew 150 -> " << a->getBalance() << "\n";
        } catch (const std::invalid_argument& e) {
            std::cout << a->type() << " " << a->getOwner() << ": REFUSED: " << e.what() << "\n";
        }
    }

    std::cout << "\n=== Month end ===\n";
    for (Account*a : accounts){
        a->monthEnd();
    }
    printAccounts(accounts);
    std::cout << "\n=== Savings: 4 withdrawals ===\n";
    Account* savings = accounts[1];        
    for (int i = 1; i <= 4; i++) {
        try {
            savings->withdraw(100);
            std::cout << "withdrawal " << i << " OK -> " << savings->getBalance() << "\n";
        } catch (const std::invalid_argument& e) {
            std::cout << "withdrawal " << i << " REFUSED: " << e.what() << "\n";
        }
    }

    std::cout << "\n=== Invalid accounts ===\n";
    try{
        accounts.push_back(new CheckingAccount("", 1000 ,100));
        
        
    } catch(const std::invalid_argument& e){
        std::cout<<":REFUSED " << e.what()<< "\n";
    }
    try{
        accounts.push_back(new SavingsAccount("banna", 1000 ,-0.05));
    } catch(const std::invalid_argument& e){
        std::cout<<":REFUSED " << e.what()<< "\n";
    }
    try{
        accounts.push_back(new StudentAccount("canna", -100));
    } catch(const std::invalid_argument& e){
        std::cout<<":REFUSED " << e.what()<< "\n";
        
    for (Account* a : accounts) delete a;
    accounts.clear();
    return 0;
    }
}