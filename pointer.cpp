// C++ program to illustrate Pointer Arithmetic
#include <bits/stdc++.h>
using namespace std;


// Driver program
int main() { 
    string str = "Pant";
    string * ptr; // declearing pointer
    ptr= &str;  // intializing pointer

    // nested pointer (pointer pointing to another pointer)
    string* * nested = &ptr;


    cout<<str<<endl;
    cout<<ptr<<endl;
    cout<<*ptr<<endl;
    cout<<*nested<<endl;  
    cout<<**nested<<endl; // accesing value from nested pointer

}
