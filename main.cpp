#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
using namespace std;


class games{
  public:
    void numberguessgame(){
      int random = rand() % 100 + 1;

    int lives = 10;
    int guess;
    bool won = false;
    while(lives>0){
      cin >> guess;
       if (guess<random){
        cout<<"higher\n";
      } else if(guess>random){
        cout<<"Lower\n";
      } else {
        cout<<"you won number is : \n" << random;
        won = true;
        break;
      }
      lives--;
    }
    if(!won){
        cout<<"You lost\n";
      }
    }
};
class apps{

  public:
    void calculator(){
        double a ,b, c;
        char op;
        cout << "Enter an operator (+, -, *, /): ";
        cin >> op;
        cout << "Enter two numbers: ";
        cin >> a >> b;
        switch(op){
          case '/':
            c = a/b;
            break;
          case '*':
            c = a*b;
            break;
          case '+':
            c = a+b;
            break;
          case '-':
            c = a-b;
            break;
          default:
            cout<<"invalid number or operator";
            return;
        }
        cout<<"result : "<< c;
      }
    void temperatureConverter(){
      int ch ;
      double celcius;
      cout<<"Choose conversion\n" << "1.To Fahrenheit F\n" << "2.To Kelvin (K)\n";
      cin >> ch;
      cout<<"Enter celcius C :";
      cin >> celcius;
      switch(ch){
        case 1:
          cout<< celcius << "C = " << (celcius*9/5) + 32 << "F"; 
          break;
        case 2:
          cout<< celcius << "C =" << celcius + 273.15 <<"(K)" ;  
          break;
        default:
          cout<< "choose only 1 or 2";
      }

    }
      void todo(){
        vector<string> tasks;
        int ch;

        while(true){
        cout << "\nChoose what to do\n" << "1.Add task\n" << "2.Show task\n" <<"3.Delete task\n"<<"4.Return\n";
        cin >> ch;
        switch(ch){
          case 1:{
            string task;
            cout<<"enter task:";
            cin.ignore();
            getline(cin, task);

            tasks.push_back(task);
            break;
          }
          case 2:
            if(tasks.empty()){
              cout<<"task empty";
            } else {
                for (int i = 0; tasks.size()>i;i++){
                    cout<<i+1 << "."<< tasks[i]  << "\n";
                    }
                }  
            break;
          
          case 3:
            int del;
            cout<<"choose task to delete";
            cin>>del;

            if(del>=1 && del<=tasks.size()){
              tasks.erase(tasks.begin() + del -1);
            } else {
              cout <<"invalide number";
            }
            break;
            
          case 4:
            return;
          default: 
            cout<<"wrong number";
            break;
        }
    }
  }
};



int main(){
    games game;
    apps app; 
    int choice;
    cout << "c++ small tasks\n";
    cout << "1. Calculator\n";
    cout << "2. Number Guessing Game\n";
    cout << "3. Temperature convertion\n";
    cout << "4. To do list\n";
    cout << "5. Exit\n";
    cout << "Choose: ";
    cin >> choice;
    switch(choice){
      case 1:
        app.calculator();
      break;
      case 2:
        game.numberguessgame();
      break;
      case 3:
        app.temperatureConverter();
        break;
      case 4:
        app.todo();
        break;
      case 5:
        cout<<"Bye";
      break;
      default:
        cout<<"invalid choice";
    }
    return 0; 

}