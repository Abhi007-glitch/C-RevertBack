

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

class MyLinkedList
{
public:
    Node *head;
    Node *tail;

    MyLinkedList(Node *head)
    {
        this->head = head;
        this->tail = head;
    }

    MyLinkedList()
    {
        this->head = NULL;
        this->tail = NULL;
    }

    //creating destructor
    ~ MyLinkedList()
    {
        cout<<"MyLinkedList Destructor function called."<<endl;
    }

    void addNodeAtStart(Node *node);

    void addNodeAtEnd(Node *node);

    int deleteNode(int val);

    bool isPresent(int val);

    Node *getHead();

    Node *getTail();

    Node *getMiddel();
};

Node * MyLinkedList :: getMiddel()
{
     Node * node = this->head;
    if (node->data==0)
    {
        return  NULL;
    }
   
    Node * fast = node;
    Node * slow = node;
   
    while(fast==NULL || fast->next!=NULL && fast->next->next!=NULL)
    {
        fast = fast->next->next;
        slow=slow->next;
    }

    return slow;
}

Node *MyLinkedList ::getHead()
{
    return this->head;
}

Node *MyLinkedList ::getTail()
{
    return this->tail;
}

void MyLinkedList ::addNodeAtStart(Node *node)
{

    if (this->head==NULL)
    {
        this->head = node;
        this->tail = node;
        return;
    }

    node->next = this->head;
    this->head = node;
    return;
}

void MyLinkedList ::addNodeAtEnd(Node *node)
{
    if (this->head==NULL)
    {
      this->head = node;
      this->tail = node;
      return;
    }
  
    (this->tail)->next = node;
    this->tail = node;
}

bool MyLinkedList ::isPresent(int val)
{

    Node *cur = this->head;
    while (cur != NULL && cur->data != val)
    {
        cur = cur->next;
    }

    if (cur != NULL)
    {
        return true;
    }
    return false;
}


int MyLinkedList :: deleteNode(int val)
{
   Node * node =  this->getHead();
   Node * temp;
    int ans =0;
   if (node->data ==0)
   {
    return ans;
   }
   
   //deleting head
   if (node->data==val)
   {
    ans =1;
    temp = node;
    node = node->next;
    this->head = node;
    delete node;  // deleting Node to avoid memory lickage
    return ans;
   }


   //deleting intermediate node
   Node* prev =node;
   node = node->next;
    
   while(node!=NULL && node->next!=NULL)
   {
    
     if (node->data==val)
     {  temp =node;
        prev->next = node->next;
        ans =1;
        delete temp; // deleted Node 
        return ans;
     }
     prev= node;
     node = node->next;
     
   }


   //deleting tail
   if (node->next==NULL && node->data==val)
   {
    temp = node;
    cout<<"deleting a tail value"<<endl;
     prev->next =NULL;
     delete temp;
     ans=1;
     return ans;
   }

   return ans;

}
// Driver program
int main()
{
    cout << "Working" << endl; // accesing value from nested pointer

    Node *n1 = new Node(1);
    Node *n2 = new Node(2);
    Node *n3 = new Node(3);
    Node *n4 = new Node(4);

    MyLinkedList *l1 = new MyLinkedList();

    l1->addNodeAtEnd(n3);
    l1->addNodeAtStart(n2);
    l1->addNodeAtEnd(n4);
    l1->addNodeAtEnd(n1);
    



    // l1->tail->next = n2;
    // l1->tail = l1->tail->next;

    // l1->tail->next = n3;
    // l1->tail = l1->tail->next;

    // l1->tail->next = n4;
    // l1->tail = l1->tail->next;

    cout << l1->head->data << endl;
    cout << l1->tail->data << endl;
    cout << "Iterating over MyLinkedList" << endl;
    

    Node *cur = l1->head;
    
    while (cur != NULL)
    {
        cout << cur->data <<endl;
        cur = cur->next;
    }

    cout<<l1->isPresent(3)<<endl;
    cout<<l1->isPresent(5)<<endl;
    
    Node * middle = l1->getMiddel();
    
    cout<<"The value of middle node is : "<<middle->data<<endl;

    int ans =l1->deleteNode(3);

    cout<<"Deleted node : "<< ans <<endl;


       cur = l1->head;
    
    cout<<"After Deletion"<<endl;
    while (cur != NULL)
    {
        cout << cur->data <<endl;
        cur = cur->next;
    }
    return 0;
}
