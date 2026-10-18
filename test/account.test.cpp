#include "../inc/account.h"
#include "test.helper.h"
#include <stdexcept>

int main(){
    //SAVING
    SavingsAccount d("test", 100, 0.05);
    d.deposit(50);
    check("depo adds",d.getBalance(),150);
    checkThrows("deposit zero",     [&]{ d.deposit(0); });
    checkThrows("deposit negative", [&]{ d.deposit(-5); });
    check("refused deposit leaves balance", d.getBalance(), 150);
    checkThrows("savings negative balance" ,[&] {SavingsAccount{"test",-1000,0.05};});
    checkThrows("savings empty owner",    [&]{ SavingsAccount{"", 1000, 0.05}; });
    checkThrows("savings negative rate",  [&]{ SavingsAccount{"test", 1000, -0.05}; });
    checkNoThrow("savings zero start",    [&]{ SavingsAccount{"test", 0, 0.05}; });
    SavingsAccount s("test", 1000, 0.05);
    checkNoThrow("savings 3 withdrawals", [&]{ s.withdraw(100); s.withdraw(100); s.withdraw(100); });
    checkThrows("savings 4th withdrawal", [&]{ s.withdraw(100); });
    check("savings balance after 3",      s.getBalance(), 700);
    s.monthEnd();
    check("savings interest",             s.getBalance(), 735);
    checkNoThrow("savings after monthEnd",[&]{ s.withdraw(100); });
    const Account& a1 = s;
    checkStr("savings type",  a1.type(), "Savings");
    //######################################################################

    //Checking
    CheckingAccount fee("test",1000,100);
    CheckingAccount nofee("test",1100,100);
    CheckingAccount c("test", 1000, 100);
    CheckingAccount zero("test", 100, 100);
    checkNoThrow("checking withdraw to 0",    [&]{ zero.withdraw(100); });   
    zero.monthEnd();
    check("checking no fee at exactly 0", zero.getBalance(), 0);
    checkThrows("checking negative amount", [&]{ c.withdraw(-50); });
    check("refused withdrawal leaves balance", c.getBalance(), 1000);
    checkThrows("Checking negative balance" , [&] {CheckingAccount{"checktest",-1000,100};});
    checkThrows("Checking empty owner" , [&]{CheckingAccount{"",1000,100};});
    checkNoThrow("checking overdraft to -50", [&]{ fee.withdraw(1050); });
    checkNoThrow("checking withdraw to 0",    [&]{ zero.withdraw(100); });
    check("fee checking" , fee.getBalance(), -50);
    fee.monthEnd();
    check("Month end checking",fee.getBalance(),-60);
    checkThrows("Overdraftlimit checking", [&]{fee.withdraw(1110);});
    const Account& a2 = c;
    checkStr("checking type", a2.type(), "Checking");
    
    //student
    StudentAccount st("test",100000);
    checkNoThrow("student withdraw exact balance", [&]{ st.withdraw(100000); });   // → 0 allowed
    checkThrows("student can't go below 0",        [&]{ st.withdraw(0.01); });  // refused
    check("student balance stays 0", st.getBalance(), 0);
    checkThrows("Student negative balance" , [&] {StudentAccount{"test",-1000};});
    checkThrows("Student empty owner" , [&]{StudentAccount{"",1000};});
    const Account& a3 = st;
    checkStr("student type",  a3.type(), "Student");

    return report();
}