#include <iostream>
#include <map>
#include <exception>
#include <string>
#include <sstream>
struct Book {
    int id;
    std::string title;
    std::string author;
    int year;
    bool borrowed;
};

class Library {
    //key = id
    private:
        std::map<int, Book> books;
    public:
        void addBook(int id, const std::string& title, const std::string& author, int year) {
        if (books.count(id)) throw std::invalid_argument("A book with this id already exists");
        books[id] = Book{id, title, author, year, false};
    } 

};

bool ReadInt(int& out) {

    std::string line;
        if (!std::getline(std::cin, line)) return false;
        std::istringstream ss(line);
        char extra;
        return (ss >> out) && !(ss >> extra);
    }

bool readLine(std::string& out) {
    return static_cast<bool>(std::getline(std::cin, out));
}


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
