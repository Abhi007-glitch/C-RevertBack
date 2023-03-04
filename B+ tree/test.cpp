/**
 * 
 * 
 * creating object using construtor directly inside loop
 * {
 *   . create object at same address(this was due to compiler of VS Code ) 
 *   . and these objects get distroyed on completion of each iteration. 
 * 
 * }
 * 
 * solution : use - new to allocate memory in heap and this will make new object at new address in heap for each object creation call 
 *                  this auto destruction of object after every iteration happens because these objects where created in call stack which get's clean after every call 
 * Note : Remeber to create a destructor to avoid memory leak ( as we have allocated in space)
 * 
 * 
 * */
  


#include <iostream>
#include <math.h>
#include <limits.h>
#include <stack>
#include <vector>
using namespace std;


class Student{
   public :
    string Name;
    int roll_no;

    Student(string name , int roll_no)
    {
        this->Name= name ;
        this->roll_no=roll_no;
    }
    
};



int main()
{
    string arr[] = {"abhi","jatin","vaibhav"};
    int roll_no[]= {1,17,41};
    
    vector<Student*>s;
    for ( int i=0 ;i<3;i++)
    {
        Student* s1 = new Student (arr[i],roll_no[i]);
        cout<<s1->Name<<endl;
        cout<<&*s1<<endl;
        s.push_back(s1);
    }
    return 0;
     
}