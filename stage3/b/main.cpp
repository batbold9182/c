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