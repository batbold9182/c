#include <iostream>
#include <vector>
#include "../inc/library.h"

int main(){
    Library lib;
    while(true){
        int choice;
        std::cout<<"Welcome to the library\n";
        std::cout<<"1.Add book\n";
        std::cout<<"2.Remove book\n";
        std::cout<<"3.Borrow\n";
        std::cout<<"4.Return\n";
        std::cout<<"5.List of all books\n";
        std::cout<<"6.Search\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter your choice: ";
        if (!ReadInt(choice)) {
            if (std::cin.eof()) break;         
            std::cout << "Please enter a number from 1 to 7.\n";
            continue;
        }
        if (choice < 1 || choice > 7) {
            std::cout << "Please enter a number from 1 to 7.\n";
            continue;
        }

        try {
            if(choice == 7){
                break;
            }
        }   catch (const std::invalid_argument& e){
                std::cout<<":REFUSED " << e.what()<< "\n";
                continue;
        }

        try {
            int id = 0, year =0;
            std::string title, author;
            std::string query;
            switch (choice)
            {
            case 1:
                std::cout<<"Enter book id";
                if (!ReadInt(id))      { std::cout << "Invalid ID.\n";   break; }
                std::cout << "Enter book title: ";
                if (!readLine(title))  break;

                std::cout << "Enter author: ";
                if (!readLine(author)) break;

                std::cout << "Enter publication year: ";
                if (!ReadInt(year))    { std::cout << "Invalid year.\n"; break; }

                lib.addBook(id, title, author, year);
                std::cout << "Book added.\n";
                break;
            case 2:
                std::cout<<"Enter book id";
                if(!ReadInt(id))    {std::cout<<"Invalid id.\n"; break;}
                lib.removeBook(id);
                std::cout<<"Book removed\n";
                break;
            case 3:
                std::cout<<"Enter book id";
                if(!ReadInt(id))    {std::cout<<"Invalid id.\n"; break;}
                lib.borrowBook(id);
                std::cout<<"Successfully borrowed\n";
                break;
            case 4:
                std::cout<<"Enter book id";
                if(!ReadInt(id))    {std::cout<<"invalid id\n"; break;}
                lib.returnBook(id);
                std::cout<<"Successfully returned\n";
                break;
            case 5:
                std::cout<<"List books sorted by: 1. ID\n2. Title\n3. Author\n4. Year\n";
                int five; 
                if (!ReadInt(five)) {
                if (std::cin.eof()) break;         
                std::cout << "Please enter a number from 1 to 4.\n";
                continue;
                }
                lib.allBooks(five);
                break;
            case 6:
                std::cout<<"Search a book\n";
                getline(std::cin,query);
                lib.searchBook(query);
                break;
            default:
            std::invalid_argument("Invalid argument");
                continue;
            }
        }   catch (const std::invalid_argument& e) {
            std::cout<<":REFUSED " << e.what()<< "\n";
        }
        
}
    

    return 0;
}
