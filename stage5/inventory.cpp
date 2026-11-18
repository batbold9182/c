#include <iostream>
#include "../inc/inventory.h"
#include <vector>

int main(){
    inventory<std::string> names;
    inventory<Product> product;

    names.add("Apple");
    names.add("Avacado");
    names.add("Banana");

    std::vector<std::string> aWords = names.filter(
        [](const std::string& s) { return s[0] == 'A'; }); 
    
    for (const std::string& w : aWords) {
        std::cout << w << "\n";
    }
    
    Product apple{"Apple",0.50};
    Product bread{"Bread",2.20};
    Product cheese{"Cheese",6.80};

    product.add(apple);
    product.add(bread);
    product.add(cheese);

    for (size_t i = 0; i<product.size();i++){
        const Product& P = product.get(i);
        std::cout<<P.name <<" "<<P.price<< "\n";
    }
    auto cheap = product.filter([](const Product& p) { return p.price < 5; });   // Apple, Bread
    for (const Product& p : cheap) {
        std::cout << p.name << " " << p.price << "\n";
    }
    std::cout<<names.size() << "\n";

    return 0;
}