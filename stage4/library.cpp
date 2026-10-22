#include <iostream>
#include <map>
#include <exception>
#include <string>
#include <sstream>
#include <algorithm>
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
        void removeBook(int id){
            if(!books.count(id)) throw std::invalid_argument("Book did not found");
            books[id] = Book{id};
        }
        void borrowBook(int id) {
            auto it = books.find(id);                     // look up the book by id
            if (it == books.end()) throw std::invalid_argument("Book not found");

            Book& book = it->second;                      // & = the REAL book inside the map
            if (book.borrowed) throw std::invalid_argument("Book is already borrowed");

            book.borrowed = true;                         // change just this field
        }
        void returnBook(int id){
            auto it = books.find(id);
            if(it == books.end()) throw std::invalid_argument("Book not found");

            Book& book = it->second;
            if(!book.borrowed) throw std::invalid_argument("Book did not borrowed");

            book.borrowed = false;
        }
        void allBooks(){
            for (auto i = books.begin(); i != books.end(); i++) {
                std::cout << i->first << " "
                        << i->second.title << " "
                        << i->second.author << " "
                        << i->second.year << " "
                        << (i->second.borrowed ? "Borrowed" : "Available")
                        << '\n';
            }
        }
        void searchBook(const std::string& text) {
            bool found = false;
            for (const auto& [id, book] : books) {                       // id = key, book = value
                if (book.title.find(text) != std::string::npos) {
                    std::cout << id << " " << book.title << " " << book.author << "\n";
                    found = true;
                }
            }
            if (!found) std::cout << "Book not found.\n";
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
                std::cout<<"List of all books\n";
                lib.allBooks();
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
