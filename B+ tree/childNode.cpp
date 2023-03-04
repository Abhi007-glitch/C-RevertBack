#include <iostream>
#include <math.h>
#include <limits.h>
#include <stack>
using namespace std;

class ChildNode;

class BPTNodeLinkedList
{
public:
  ChildNode *head = NULL;
  int size = 0;
  int curSize = 0;
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

  int Size();
};

class ChildNode
{
public:
  // data members
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
  public :
  BPTNodeLinkedList *rootNode;

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
    BPTNodeLinkedList *cur;
    ChildNode *node, *prev = NULL;
    stack<BPTNodeLinkedList *> st;
    int count =0;

    if (root->head==NULL || root->Size()<root->size)
    {
      node = root->head;
      prev = NULL;
      if (node==NULL)  // if root has no nodes till now - first node value to be inserted into ListNode
      {
          ChildNode FNode(key);
          cur->head = &FNode;
          cur->curSize++; 
          return;
      }
      else 
      {
        count=0;
        while(count<root->Size())
        {
          if (node->key>key)
          {
            
          }
        }
      }
    }
    st.push(root);
    while (st.size() != 0)
    {
      cur = st.top();
      st.pop();
      node = cur->head;
      prev = NULL;
      count =0;

      

      while (count<cur->Size()  && node->key < key )
      {
        prev = node;
        node = node->next;
        count++;
      }

      if (prev->child != NULL)  // i.e not a leaf node
      {
        if (node == NULL)
        {
          // search in child node
          st.push(prev->child);
        }
        else
        {
          // search in child node
          st.push(prev->child);
        }
      }
      else // if a leaf node
      {


         ChildNode temp;
         temp.key = key;
         ChildNode * tempPtr = &temp;
         int split = cur->insertNode( tempPtr);
         cout<<"split value "<<split<<endl;
         cout<<"split performed and need to pass new value to parent"<<endl;
      }
    }
  }
};


int BPTNodeLinkedList ::insertNode(ChildNode *node)
{
  int curSize = this->Size();
  int size = this->size;
  ChildNode *cur;
  ChildNode *prev;

  if (curSize < size)
  {
    cur = this->head;
    prev = NULL;

    while (cur != NULL && cur->next != NULL)
    {
      if (cur->key > node->key)
      {
        break;
      }
      else if (cur->key < node->key)
      {
        prev = cur;
        cur = cur->next;
      }
      else
      {
        return -1;
      }
    }

    if (cur == NULL)
    {
      this->head = &*node;
      this->curSize++;
      return -1;
    }
    else
    {
      if (cur->key > node->key)
      {
        prev->next = node;
        node->next = cur;
        this->curSize++;
      }
      else if (cur->key < node->key)
      {
        cur->next = node;
        this->curSize++;
      }
      else
      {
        return -1;
      }
    }

    return -1;
  }
  else // split required
  {
    cout << "Split Required" << endl;
    BPTNodeLinkedList newList(this->size);

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
      newList.curSize = ((size+1)/2)+1;
     return newList.head->key;

    // creating a parent node

    // BPTNodeLinkedList parent(this->size);

    // // creating copy of node;
    // ChildNode copy = node->keyCopyNode(node);
    // parent.head = &copy;

    // cout << " new root created with value " << parent.head->key << endl;
    
  }
}

int BPTNodeLinkedList ::Size() // returns curSize which is filled.
{
  return this->curSize;
}

int main()
{
  BPTNodeLinkedList l1 = BPTNodeLinkedList(4);
  BPTNodeLinkedList * ptr = &l1;

  
  BPTree tree = BPTree(ptr);

  // ChildNode node1(1);
  // l1.insertNode(&node1);
  tree.Search(1);
  
  // ChildNode node2(7);
  // l1.insertNode(&node2);
  // tree.Search(2);

  // ChildNode node3(3);
  // l1.insertNode(&node3);
  // tree.Search(3);

  // ChildNode node4(9);
  // l1.insertNode(&node4);
  // tree.Search(4);

  ChildNode *temp = l1.head;
  while (temp != NULL)
  {
    cout << temp->key << endl;
    temp = temp->next;
  }

  ChildNode node5(5);
  l1.insertNode(&node5);

  temp = l1.head;
 
  while (temp!=NULL)
  {
    cout << temp->key << endl;
    temp = temp->next;
   
  }


  cout<<"checking BPTree class"<<endl;
  tree.Search(3);

  temp = l1.head;
  while (temp != NULL)
  {
    cout << temp->key << endl;
    temp = temp->next;
  }
  
  return 0;
}