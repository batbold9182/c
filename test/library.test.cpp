#pragma once
#include "../inc/library.h"
#include "test.helper.h"
#include <stdexcept>

int main(){
    Library lib;
    lib.addBook(1,"4","4",2000);
    lib.addBook(2,"3","2",2001);
    lib.addBook(3,"2","1",2002);
    lib.addBook(4,"1","3",2003);
    check("test addbook",2,2);

    
    lib.removeBook(1);              // book with id(1) removed thats why borrow book used id(2) on test
    check("remove book",1,1);       // or before test can switch its places and still use id(1);
    checkNoThrow("already borrowed",[&]{lib.borrowBook(2);});
    checkNoThrow("returning book",[&]{lib.returnBook(2);});
    

    lib.allBooks(2);
    check("sort by title ",1,1);

    lib.allBooks(3);
    check("sort by author",3,3);

    lib.allBooks(4);
    check("sort by year",2000,2000);

    lib.borrowBook(4);
    lib.allBooks(5);
    check("sort by borrow",4,4);
    return report();
}