
 #include <bits/stdc++.h>
 using namespace std;


 class ParentClass{
  public:
   virtual void type()     // virtual keyword added.
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
    ParentClass * pptr1;
    ParentClass * pptr2;
    ChildClass child;
    ParentClass parent;
    
    pptr1 = &child;   // parent type pointer pointing to child type object
    pptr2 = &parent;  // child type pointer pointing to parent type object

    pptr1->type();     // effect of virtual function is seen from this call.
    pptr1->extraFun();

    pptr2->type();
    pptr2->extraFun();

    return 0;
 }