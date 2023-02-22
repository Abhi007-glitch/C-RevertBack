

// C++ program to illustrate Pointer Arithmetic
#include <bits/stdc++.h>
using namespace std;

class Node
{
public:
    int data;
    Node *next;

    Node()
    {
        this->data = 0;
        this->next = NULL;
    }

    Node(int data)
    {
        this->data = data;
        this->next = NULL;
    }

    Node(int data, Node *next)
    {
        this->data = data;
        this->next = next;
    }

    // creating destructor -
    ~Node()
    {
      cout<<"Node destructor"<<endl;
    }



    void printData();
};

void Node::printData()
{
    cout << this->data << endl;
}



class Stack {
 public :
    int size;
    Node* head;

    Stack()
    {
        this->size=0;
        this->head =NULL;
    }

    ~Stack()
    {
        cout<<" Stack Destructor called."<<endl;
    }


    void push(int val);

    int pop();

    int top();

    bool isEmpty();
    
};


   void Stack :: push( int val )
   {
   
    Node* cur = new Node(val);
    cur->next = this->head;
    this->head = cur;
    this->size += 1;
    return;
   }

   int Stack :: pop()
   {
      if (this->isEmpty())
      {
        return -1;
      }
      
      Node* cur = this->head;
      Node* temp =cur;
      int ans = temp->data;
      cur = cur->next;
      this->head = cur;
      this->size = this->size-1;
      delete temp;       
                                                                                 
      return ans;

   }



   int Stack :: top()
   {
    if (this->isEmpty())
    {
        return -1;
    }
    return this->head->data;
   }

   bool Stack :: isEmpty()
   {
      if (this->size==0)
      {
        return true;
      }

      return false;
   }



   int main ()
   {
    Stack st;
    st.push(10);
    st.push(20);
    st.push(30);
    cout<<st.top()<<endl;
    cout<<"removed"<<endl;
    int removed ;
    removed =st.pop();
    cout<<st.top()<<endl;
    cout<<removed<<endl;
    cout<<st.size<<endl;
    return 0;
   }