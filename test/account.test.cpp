#include "../inc/account.h"
#include "test.helper.h"
#include <stdexcept>

int main(){
    //SAVING
    checkThrows("savings negative balance" ,[&] {SavingsAccount{"test",-1000,0.05};});
    checkThrows("savings empty owner",    [&]{ SavingsAccount{"", 1000, 0.05}; });
    checkThrows("savings negative rate",  [&]{ SavingsAccount{"test", 1000, -0.05}; });
    checkNoThrow("savings zero start",    [&]{ SavingsAccount{"test", 0, 0.05}; });
    SavingsAccount s("test", 1000, 0.05);
    s.withdraw(100); s.withdraw(100); s.withdraw(100);
    checkThrows("savings 4th withdrawal", [&]{ s.withdraw(100); });
    check("savings balance after 3",      s.getBalance(), 700);
    s.monthEnd();
    check("savings interest",             s.getBalance(), 735);
    checkNoThrow("savings after monthEnd",[&]{ s.withdraw(100); });

    //######################################################################

    //Checking
    CheckingAccount fee("test",1000,100);
    CheckingAccount nofee("test",1100,100);

    checkThrows("Checking negative balance" , [&] {CheckingAccount{"checktest",-1000,100};});
    checkThrows("Checking empty owner" , [&]{CheckingAccount{"",1000,100};});
    check("No fee ", nofee.getBalance() ,1100);
    fee.withdraw(1050);
    check("fee checking" , fee.getBalance(), -50);
    fee.monthEnd();
    check("Month end checking",fee.getBalance(),-60);
    checkThrows("Overdraftlimit checking", [&]{fee.withdraw(1110);});
    std::cout<<fee.getBalance() << " " << nofee.getBalance() << " " << "\n";


    //studenr
    StudentAccount st("test",100000);
    checkThrows("Student negative balance" , [&] {StudentAccount{"test",-1000};});
    checkThrows("Student empty owner" , [&]{StudentAccount{"",1000};});
    st.monthEnd();
    check("Month end fee check", st.getBalance(),100000);
    return report();
}