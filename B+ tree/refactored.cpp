#include <iostream>
#include <math.h>
#include <limits.h>
#include <stack>
using namespace std;



class ChildNode;

class BPTNodeLinkedList
{
  
  public:
  int size = 0;
  int curSize =0;

  ChildNode *head = NULL;
  bool isLeaf = false; // to know the endpoint of the search
  
  
  BPTNodeLinkedList(int size, ChildNode *first_node)
  {

    this->head = first_node;
    curSize = 1;
    this->size = size;
  }

  BPTNodeLinkedList(int size)
  {

    this->head = NULL;
    curSize = 0;
    this->size = size;
  }

  int insertNode(ChildNode *node);

  ChildNode * findPosition( int key);
 
  int splitNodeAtLeaf(int key);

  int Size() // returns curSize which is filled.
{
  return this->curSize;
}

};

class ChildNode
{

  // data members
 public:
  int key = INT_MIN;
  ChildNode *next = NULL;
  BPTNodeLinkedList *child = NULL;


  ChildNode()
  {
    this->key = INT_MIN;
    this->next = NULL;
    this->child = NULL;
  }

  ChildNode(int key, ChildNode *next, BPTNodeLinkedList *child)
  {
    this->key = key;
    this->next = next;
    this->child = child;
  }

  ChildNode(int key, ChildNode *next)
  {
    this->key = key;
    this->next = next;
    this->child = NULL;
  }

  ChildNode(int key)
  {
    this->key = key;
    this->next = NULL;
    this->child = NULL;
  }

  ChildNode keyCopyNode(ChildNode *node)
  {
    ChildNode newNode;
    newNode.key = node->key;
    newNode.next = NULL;
    newNode.child = NULL;
    return newNode;
  }
};

class BPTree
{
  public:
  BPTNodeLinkedList *rootNode=NULL;

  BPTree()
  {
    this->rootNode = NULL;
  }

  BPTree(BPTNodeLinkedList* node)
  {
    this->rootNode = node;
  }

   void Search(int key)
  {
    BPTNodeLinkedList *root = this->rootNode;
    BPTNodeLinkedList *cur=NULL, *temp=NULL;
    ChildNode *node=NULL, *prev = NULL;
    stack<BPTNodeLinkedList *> st;
    st.push(root);
    int count =0;
    int split_value = -1;
    while(st.size()!=0)
    {   
      prev=NULL;
        cur = st.top();
        st.pop();
        if (cur->head == NULL)
        {
           ChildNode* n2 = new ChildNode(key);
           cur->head=n2;
           cur->curSize++;
           cout<<"Added First Node to First ListNode in the B+ Tree"<<endl;
           return;
        }


        if (cur->head->child==NULL)  // is a leaf ListNode
        {
           cout<<cur->head<<endl;
            if (cur->Size()<cur->size)  // there is space in ListNode if new node is required to be inserted
            {
             //find right position and insert
             ChildNode* n1 = new ChildNode(key);
              int val = cur->head->key;
              cout<<"Before entering value into findPosition "<<val<<endl;
              prev = cur->findPosition(key);

              if (prev==NULL) // new node is the smallest value in ListNode
              {
                node = cur->head;
                cur->head = n1;
                cur->head->next=node;
                cur->curSize++;
                return ;
              }

              if (prev->key == key)
              {
                cout<<"Key already Present - case - not full ListNode "<<endl;
                return ; // key already present;
              }
              node = prev->next;
              
              prev->next = n1;
              n1->next = node;
              cur->curSize++;
              cout<<"added a new key to ListNode"<<endl;
            }
            else 
            {
            // if key value alreday present then do nothing
            prev = cur->findPosition(key);
            if (prev->key==key)
            {
              cout<<"Key already present"<<endl;
              return;
            }
            else 
            {
              split_value = cur->splitNodeAtLeaf(key);
              // if key value not present then perform code spliting 
            }
            

            }
        }
        else    // is not a leaf ListNode
        {
           // traversing the B+ tree
           // find the right place to insert and insert that ListNode to stack
           prev = cur->findPosition(key);  
           temp = prev->child;
           st.push(temp);
           
        }

    }

  }


};


ChildNode * BPTNodeLinkedList :: findPosition(int key)
{
  ChildNode *cur=NULL;
  ChildNode *prev=NULL;
  cur = this->head;
  prev=NULL;
  int count =0;
  int curSize = this->Size();
  
  if (curSize==0) // if ListNode was empty
  {
     return prev;
  }
  // new value is smaller than the smallest value in ListNode 
  if (cur->key>key)
  {
    return prev;
  }
    cout<<"inside position"<<cur->key<<endl;
  while(count<curSize && cur->key<key)
  {
    prev =cur;
    cur=cur->next;
    count++;
  }

 if (cur!=NULL && cur->key==key)
 {
  cout<<"Equal "<<key<<endl;
  return cur;
 }
 
 return prev;

}


int BPTNodeLinkedList :: splitNodeAtLeaf(int key)
{
  ChildNode * cur,* prev,*node;
  BPTNodeLinkedList newList(this->size);
  int size = this->size;
  ChildNode Node(key);
  node = &Node;
  
  
    // inserting new Listnode into Tree and splitting if number of element reaches (n/2)
    int count = 0;
    cur = this->head;
    prev = NULL;
    bool placed = false;

    if (cur->key > node->key)
    {
      node->next = cur;
      placed = true;
      this->head = node; /// updated head here
      count++;             
                         // code for forwarding this update is to be added here
    }
     
    while (count < ((size + 1) / 2))
    {

      if (!placed)
      {
        // checking if new node can be placed at this position or not
        if (cur->key > node->key)
        {
          prev->next = node;
          node->next = cur;
        }
        else
        {
          prev = cur;
          cur = cur->next;
        }
        count++;
      }
      else
      {
        prev = cur;
        cur = cur->next;
        count++;
      }
    }
    this->curSize =(size+1)/2;
    
    
    if (node->key < cur->key)
    {  
      // if new value is the less than rest half values
      newList.head = node;
      prev->next = node;
      node->next = cur; // further node linked here into new linkedlist
      placed = true;
      return newList.head->key; // early return
    }
    else
    {
      newList.head = cur;
    }

    while (cur->next != NULL && !placed)
    {
      // defining head of newList
      if (!placed)
      {
        // checking if new node can be placed at this position or not
        if (cur->key > node->key)
        {
          prev->next = node;
          node->next = cur;
          placed = true;
          return newList.head->key; // early return
        }
        else
        {
          prev = cur;
          cur = cur->next;
        }
      }
    }

    if (!placed)
    {
      if (cur->key > node->key)
      {
        prev->next = node;
        node->next = cur;
      }
      else
      {
        cur->next = node;
      }
      
    }
    cout<<"Added new Node and splitted previous one"<<endl;
      newList.curSize = ((size+1)/2)+1;
     return newList.head->key;

  
}


int main ()
{
   BPTNodeLinkedList l1 = BPTNodeLinkedList(4);
  BPTNodeLinkedList * ptr = &l1;

  BPTree tree = BPTree(ptr);



  tree.Search(1);
  ChildNode* node = tree.rootNode->head;
  int key = tree.rootNode->head->key;
  // tree.rootNode->head->key
  cout<<"Root value before : "<<key<<endl;

  cout<<"Size of ListNode is : "<<tree.rootNode->curSize<<endl;
  int key2 = tree.rootNode->head->key;
  tree.Search(7);
  key = tree.rootNode->head->key;
  cout<<"Root value after : "<<key<<endl;
  
  cout<<"Size of ListNode is : "<<tree.rootNode->curSize<<endl;

  ChildNode *temp = tree.rootNode->head;
  cout<<"Printing ListNode : "<<endl;
  while(temp!=NULL)
  {
     cout<<temp->key<<endl;
     temp = temp->next;
  }
  return 0;
}
