/**
 * POLYMORPHISM
 * 1) Polymorphism during Complie time - functional overloading and operator overloading
 * 2) Polymorphism during Run time -  functional overriding (Virtual function)
 * 
 * overloading - same function name with different number or type of parameters
 * 
 * overriding - based on use of Virtual keyword function of base class or derived class is linked during run time
 * 
 * 
 * 
 * 
 * 
 * 
 * */
 

 #include <bits/stdc++.h>
 using namespace std;


 class ParentClass{
  public:
    void type()
    {
        cout<<"Parent type"<<endl;
    }

    void extraFun()
    {
        cout<<"Parent's Extra function"<<endl;
    }

    
 };


 class ChildClass : public ParentClass{
  public:
     void type()
     {
        cout<<"Child type"<<endl;
     }

     void extraFun()
     {
        cout<<"Child's ExtraFunction"<<endl;
     }
     
 };



 int main()
 {
    ParentClass * pptr;
    ChildClass child;

    pptr = &child;

    pptr->type();
    pptr->extraFun();

    return 0;
 }