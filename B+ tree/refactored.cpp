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
  int curSize = 0;

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

  ChildNode *findPosition(int key);

  BPTNodeLinkedList* splitNodeAtLeaf(int key);

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
  BPTNodeLinkedList *rootNode = NULL;

  BPTree()
  {
    this->rootNode = NULL;
  }

  BPTree(BPTNodeLinkedList *node)
  {
    this->rootNode = node;
  }

  BPTNodeLinkedList* Search(int key)
  {
    BPTNodeLinkedList *root = this->rootNode;
    BPTNodeLinkedList *cur = NULL, *temp = NULL,*split_ListNode=NULL;
    ChildNode *node = NULL, *prev = NULL;
    stack<BPTNodeLinkedList *> st;
    st.push(root);
    int count = 0;
    int split_value = -1;
    while (st.size() != 0)
    {
      prev = NULL;
      cur = st.top();
      st.pop();
      if (cur->head == NULL)
      {
        ChildNode *n2 = new ChildNode(key);
        cur->head = n2;
        cur->curSize++;
        cout << "Added First Node to First ListNode in the B+ Tree" << endl;
        return NULL;
      }

      if (cur->head->child == NULL) // is a leaf ListNode
      {
        cout << cur->head << endl;
        if (cur->Size() < cur->size) // there is space in ListNode if new node is required to be inserted
        {
          // find right position and insert
          ChildNode *n1 = new ChildNode(key);
          int val = cur->head->key;
          cout << "Before entering value into findPosition " << val << endl;
          prev = cur->findPosition(key);

          if (prev == NULL) // new node is the smallest value in ListNode
          {
            node = cur->head;
            cur->head = n1;
            cur->head->next = node;
            cur->curSize++;
            return NULL;
          }

          if (prev->key == key)
          {
            cout << "Key already Present - case - not full ListNode " << endl;
            return NULL; // key already present;
          }
          node = prev->next;

          prev->next = n1;
          n1->next = node;
          cur->curSize++;
          cout << "added a new key to ListNode" << endl;
        }
        else
        {
          // if key value alreday present then do nothing
          prev = cur->findPosition(key);
          if (prev->key == key)
          {
            cout << "Key already present" << endl;
            return NULL;
          }
          else
          { // if key value not present then perform code spliting
            split_ListNode = cur->splitNodeAtLeaf(key);
            cout<<"Value passed to parent is "<<split_ListNode->head->key<<endl;
            
           if (st.size()==0) // means there is no parent ListNode 
           {
              
            // creating a new BPTNodeLinkedList for new root and making other nodes as it's child 
            BPTNodeLinkedList* newListNode = new BPTNodeLinkedList(cur->size);
            ChildNode * dummyFirstNode = new ChildNode(INT_MIN); // creating a dummy node for pointing to the values less than split value
            ChildNode * newNode = new ChildNode(split_ListNode->head->key);
            

            //linking nodes within ListNode
            dummyFirstNode->next =newNode; 
            newListNode->head = dummyFirstNode;
            

            // Linking older ListNode with new ListNode as a child 
            dummyFirstNode->child = cur;
            newNode->child =  split_ListNode;
           newListNode->curSize=2;
           this->rootNode =newListNode; // updating the root Node 
            return newListNode;
           }
           else 
           { 
            /** 
             * ----------------------------------------logic for passing value to an exisiting parent node --------------------------------------------------------------------------------
             * 
             * passing value to existing parent - this will be done only once here 
             *                                    as there can be only one parent node 
             *                                    after passing the value to parent all the nodes become internal node 
             *                                    so a seprate logic runs for them 
             *                                    as while passing value from leaf to parent whole node is copie inside the parent
             *                                    but in case of passing value to internal nodes the node is passed from internal node to it's parent instead of making copy
             *
             *
             * */
            BPTNodeLinkedList* topListNode = st.top();
            st.pop();
            if (topListNode->curSize<topListNode->size)  // bhut jaga he -- space available 
            {
             // inserting value into topListNode and 
            }
            else    // nhi jaga he -- no free space available require spliting of parent node and passsing updated node to parent's parent.
            {

            }
             
             
           }
           
          }
        }
      }
      else // is not a leaf ListNode
      {
        // traversing the B+ tree
        // find the right place to insert and insert that ListNode to stack
        prev = cur->findPosition(key);
        temp = prev->child;
        st.push(temp);
      }
    }
    return NULL;
  }
};

ChildNode *BPTNodeLinkedList ::findPosition(int key)
{
  ChildNode *cur = NULL;
  ChildNode *prev = NULL;
  cur = this->head;
  prev = NULL;
  int count = 0;
  int curSize = this->Size();

  if (curSize == 0) // if ListNode was empty
  {
    return prev;
  }
  // new value is smaller than the smallest value in ListNode
  cout<<cur->key<<endl;
  if (cur->key > key)
  {
    return prev;
  }
  cout << "inside position" << cur->key << endl;
  while (count < curSize && cur->key < key)
  {
    prev = cur;
    cur = cur->next;
    count++;
  }

  if (cur != NULL && cur->key == key)
  {
    cout << "Equal " << key << endl;
    return cur;
  }

  return prev;
}

BPTNodeLinkedList* BPTNodeLinkedList ::splitNodeAtLeaf(int key)
{
  ChildNode *cur, *prev, *node;
  BPTNodeLinkedList* newList = new BPTNodeLinkedList(this->size);
  int size = this->size;
  node = new ChildNode(key);
  

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
  this->curSize = (size + 1) / 2;

  if (node->key < cur->key)
  {
    // if new value is the less than rest half values
    newList->head = node;
    prev->next = node;
    node->next = cur; // further node linked here into new linkedlist
    placed = true;
    newList->curSize=((size + 1) / 2)+1;
    return newList; // early return
  }
  else
  {
    newList->head = cur;
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
        newList->curSize=((size + 1) / 2)+1;
        return newList; // early return
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
  cout << "Added new Node and splitted previous one" << endl;
  newList->curSize = ((size + 1) / 2) + 1;
  return newList;
}

int main()
{
  BPTNodeLinkedList l1 = BPTNodeLinkedList(4);
  BPTNodeLinkedList *ptr = &l1;

  BPTree tree = BPTree(ptr);

  tree.Search(3);
  ChildNode *node = tree.rootNode->head;
  int key = tree.rootNode->head->key;
  // tree.rootNode->head->key
  cout << "Root value before : " << key << endl;

  cout << "Size of ListNode is : " << tree.rootNode->curSize << endl;
  int key2 = tree.rootNode->head->key;
  tree.Search(7);
  tree.Search(2);
  tree.Search(9);
  
  key = tree.rootNode->head->key;
  cout << "Root value after : " << key << endl;

  cout << "Size of ListNode is : " << tree.rootNode->curSize << endl;

  ChildNode *temp = tree.rootNode->head;
  cout << "Printing ListNode : " << endl;
  while (temp != NULL)
  {
    cout << temp->key << endl;
    temp = temp->next;
  }


  cout<<"Adding 5th value "<<endl;
  BPTNodeLinkedList* newRoot =  tree.Search(4);
  // if (newRoot!=NULL)
  // {
  //   tree.rootNode = newRoot;
  // }
   temp = tree.rootNode->head;
 
   cout<<"New ROot value is :"<<endl;
   cout<<temp->next->key<<endl;
   
   if (temp->next->next==NULL)
   {
    cout<<"Working Fine"<<endl;
   }
  
   tree.Search(7);

  return 0;
}
