#include <iostream>
#include <vector>
#include <list>

using namespace std;

int main(){
    int del = 1;
    vector<string> tasks = {"test","a","b"};
    tasks.erase(tasks.begin() + del);
    for (int i = 1; tasks.size()+1>i;i++){
            cout<<i <<"." << tasks[i-1] + "\n";
          }

    
    return 0;
}