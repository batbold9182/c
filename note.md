## iostream - includes input and outbound data to the console or file 
Object	Description
    *cerr*	An output stream for error messages
    *clog*	An output stream to log program information
    *cin*	An input stream that reads keyboard input from the console by default
    *cout*	An output stream which writes output to the console by default
    *wcerr*	The same as cerr but outputs wide char (wchar_t) data rather than char data
    *wclog*	The same as clog but outputs wide char (wchar_t) data rather than char data
    *wcin*	The same as cin but interprets each input character as a wide char (wchar_t)
    *wcout*	The same as cout but outputs wide char (wchar_t) data rather than char data
## using namespace std; - makes easy to use std
    STD stands for standart In C++, things like cout, cin, and endl belong to the Standard Library.
    for example normal c uses std::cin>>x with namespace can write it
    std::cin>>x --> cin>>x dont have to write std:: everytime
## int main - c++ return integer to OS (Operating System)
    When programm finishes executing c++ directly returns int value to the OS
    + c++ is case sensitive so int main() !== int Main()

## why return 0 - as mentioned at top c++ returns integer 
    All other values expect 0 means error occured so when 0 returns to OS it means success but it is not mandatory but its a nice convention to tell OS the programm exited normally. Without it compiler puts it for you automatically.

## char is only used for single character that is inside in ' ' this quotes
    char a = "double quot"; is error not a single char plus wrong quote used for strings
    correct one is 
        char correct = 'b';

## float vs double differs how many digits the value can have after the decimal point.
    float only takes up to 6 or 7 double takes upto about 15
    float size is 4 byte double size is 8 byte

## str.at() vs str[x] use [] when you know the index is valid; use .at() when you want bounds checking.


## cin vs getline() cin stops reading at first space(" ")
    example cin<<name; <-- assign "bat bold"
            output will be "bat"
        VS
            getline(cin,name) <-- assign "bat bold"
            output will be "bat bold"

## ARRAYS
sizeof() --> tells in bytes not index so array is like {int , int , int } it means {4bytes,4bytes,4bytes}
int myNumbers[5] = {10, 20, 30, 40, 50};
cout << sizeof(myNumbers); // returns 20 because int type is 4 bytes above 5*4 =20 which explains

to find a normal size i can just divide it by any element in the arr but prefer [0] since its guaranteed to have an element

int myNumbers[5] = {1,2,3,4,5};
cout<<sizeof(myNumbers)/sizeof(myNumbers[0]); 
top example means cout<< (4byte+4byte +...n +4byte)/4byte = 5

## enums is constants of structures preferebbly used in week days months days etc constants

## new and delete memory management from taking inputs i need to manage my memory manually for normal variables it manages itself but when it depends on input i cant estimate so manually clean memory for example:
int main() {
  int numGuests;
  cout << "How many guests? ";
  cin >> numGuests;

  // Check for invalid input
  if (numGuests <= 0) {
    cout << "Number of guests must be at least 1.\n";
    return 0;
  }

  // Create memory space for x guests (an array of strings)
  string* guests = new string[numGuests];

  // Ignore the leftover newline character after reading numGuests
  cin.ignore();

  // Enter guest names
  for (int i = 0; i < numGuests; i++) {
    cout << "Enter name for guest " << (i + 1) << ": ";
    getline(cin, guests[i]); // Read the full name (including spaces)
  }

  // Show all guests
  cout << "\nGuests checked in:\n";
  for (int i = 0; i < numGuests; i++) {
    cout << guests[i] << "\n";
  }

  delete[] guests; // Clean up memory
  return 0;
}
if i dont clean it it will cause memory leak in ram.

## Multiple functions can have same name as long as its number or datatypes are differend -- called function overload

## <cstdlib> library that server random number process control memory management etc.

## lists vs vector
    vector is like an array list is array but every element is connected by linked list if specific linked lists required list recommended other than that for me for now vector okay.