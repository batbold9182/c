#include <iostream>
#include "../inc/inventory.h"

int main(){
    inventory<std::string> names;
    inventory<int> quantity;
    
    names.add("a");
    names.add("b");
    quantity.add(34);
    std::cout<<names.size() << "\n";
    std::cout<<quantity.size() << "\n";

    quantity.remove(0);
    names.remove(0);
    std::cout << names.get(0) << "\n"; //PRINTS A✅ SINCE B REMOVED
    std::cout<<"###################" << "\n";

    std::cout<<names.size() << "\n";
    std::cout<<quantity.size() << "\n";
    return 0;
}