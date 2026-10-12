#pragma once
#include <string>
#include <stdexcept>
class Account {
    protected:
        std::string owner;
        double balance;
    public:
        Account(const std::string& owner , double initialBalance)
        : owner(owner), balance(initialBalance){
            if(owner.empty() || initialBalance < 0){
                throw std::invalid_argument("Invalid account");
            }
        };
        void deposit(double amount){
            if(amount<=0){
                throw std::invalid_argument("Deposit amount must be positve");
            }
            balance += amount;
        }
        virtual void withdraw(double amount){
            if(amount <=0 || amount>balance){
                throw std::invalid_argument("Invalid withdraw amount");
            }
            balance -= amount;
        }
        virtual std::string type() const = 0;
        virtual void monthEnd() = 0;
        double getBalance() const {return balance;};
        std::string getOwner() const {return owner;};
        virtual ~Account(){}
};

class SavingsAccount :public Account {
    private:
        int counter = 0;
        double interestRate = 0.02;
    public:
        SavingsAccount(const std::string& owner, double balance, double rate)
            : Account(owner, balance), interestRate(rate) {
            if (interestRate < 0) throw std::invalid_argument("Interest rate can't be negative");
        }
        void withdraw(double amount ) override {
                if (counter >= 3){
                    throw std::invalid_argument("Savings:max3 withdrawl");
                }
                Account::withdraw(amount);
                counter++;
            }
        void monthEnd() override {
            balance += balance * interestRate;
            counter = 0;
        }
        std::string type() const override {return "Savings";}

};

class CheckingAccount :public Account {
    private:
        double overdraftLimit;
    public:
        CheckingAccount(const std::string& owner, double balance,double ODLimit)
        : Account(owner,balance),overdraftLimit(ODLimit){
            if(overdraftLimit<0){
                throw std::invalid_argument("Limit must be positive");
             }
            }
            void withdraw(double amount) override {
                
                if(amount<=0){
                    throw std::invalid_argument("Withdraw amount must be positive");
                }

                if(balance - amount  < -overdraftLimit){
                    throw std::invalid_argument("Overdraft limit");
                }
                balance -= amount;
            };

        
        void monthEnd() override {
            if(balance<0){
                balance -= 10;
            }
        }
        std::string type() const override {return "Checking";}
};

class StudentAccount :public CheckingAccount {
    public:
        StudentAccount(const std::string& owner, double balance)
        :CheckingAccount(owner,balance,0){

        }
        void monthEnd() override {
        }
        std::string type() const override {return "student";}
};
