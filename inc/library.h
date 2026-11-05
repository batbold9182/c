#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <set>
#include <fstream>

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
            books.erase(id);
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
        void allBooks(int sortBy) const {
            std::vector<Book> copy;
            for (auto it = books.begin();it!=books.end(); it++){
                copy.push_back(it->second);
            }
            if (sortBy == 2){
                    std::sort(copy.begin(), copy.end(),
                    [](const Book& a, const Book& b) {
                        return a.title < b.title;
                    }
                );
            }  else if (sortBy == 3){
                    std::sort(copy.begin(),copy.end(),
                    [](const Book& a, const Book& b) {
                        return a.author < b.author;
                        }
                    );
                } else if (sortBy == 4){
                    std::sort(copy.begin(),copy.end(),
                    [](const Book& a, const Book& b) {
                        return a.year < b.year;
                        }
                    );
                } else if (sortBy == 5){  
                    auto count = std::count_if(copy.begin(),copy.end(),
                    [](const Book& book) {
                        return book.borrowed == true;
                     }
                    );
                    if (count != 0){
                        std::cout << "Total borrowed books: " << count << '\n';
                    } else if (count == 0){
                        std::cout << "Every book is available. \n";
                    }
                    
                }
             
            for (const Book& book : copy) {
                std::cout << book.id << " "
                        << book.title << " "
                        << book.author << " "
                        << book.year << " "
                        << (book.borrowed ? "Borrowed" : "Available") << " "
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
        void allAuthors() const {
            std::set<std::string> authors;
            for (auto it = books.begin();it!=books.end(); it++){
               authors.insert(it->second.author);
            }
            std::cout << "Total unique authors: " << authors.size() << '\n';
            for (const auto& author : authors) {
                std::cout << author << '\n';
            }


    }
        void saveToFile(const std::string& filename) const {
            std::ofstream file(filename);
            if (!file.is_open()) {
                std::cout << "File could not be opened\n";
                return;
            }
            for (const auto& [id, book] : books) {
                file << id << '|' << book.title << '|' << book.author << '|'
                     << book.year << '|' << (book.borrowed ? 1 : 0) << '\n';
            }
        }
        void loadFromFile(const std::string& filename) {
            std::ifstream file(filename);
            if (!file.is_open()) {
                std::cout << "File could not be opened\n";
                return;
            }
            books.clear();
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream ss(line);
                std::string idText, title, author, yearText, borrowedText;
                std::getline(ss, idText, '|');
                std::getline(ss, title, '|');
                std::getline(ss, author, '|');
                std::getline(ss, yearText, '|');
                std::getline(ss, borrowedText);              

                int id = std::stoi(idText);
                addBook(id, title, author, std::stoi(yearText));
                if (borrowedText == "1") books[id].borrowed = true;  
            }
        }
};      

    
inline bool ReadInt(int& out) {

    std::string line;
        if (!std::getline(std::cin, line)) return false;
        std::istringstream ss(line);
        char extra;
        return (ss >> out) && !(ss >> extra);
    }

inline bool readLine(std::string& out) {
    return static_cast<bool>(std::getline(std::cin, out));
}