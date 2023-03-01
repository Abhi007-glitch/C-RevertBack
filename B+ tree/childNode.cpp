#include <iostream>
#include <math.h>
#include <limits.h>
using namespace std;

class ChildNode;

class BPTNodeLinkedList{
   public:
   
   ChildNode * head =NULL;
   int size=0;
   int curSize=0;
   bool isLeaf=false; // to know the endpoint of the search


   BPTNodeLinkedList(int size,ChildNode* first_node)
   {
      
       this->head = first_node;
       curSize=1;
       this->size = size;
   }
   
   BPTNodeLinkedList(int size)
   {
    
    this->head =NULL;
    curSize =0;
    this->size = size;
   }
   
   void insertNode(ChildNode * node);

   int Size();



};






class ChildNode{
    public:
    // data members 
    int key=INT_MIN;
    ChildNode* next=NULL;
    BPTNodeLinkedList* child=NULL;

    ChildNode()
    {
        this->key=INT_MIN;
        this->next = NULL;
        this->child = NULL;
    }

    ChildNode(int key,ChildNode* next, BPTNodeLinkedList* child)
    {
      this->key = key;
      this->next = next;
      this->child =child;
      
    }

    ChildNode ( int key,ChildNode* next)
    {
        this->key = key;
        this->next = next;
        this->child =NULL;
    }

    ChildNode ( int key )
    {
        this->key = key;
        this->next = NULL;
        this->child = NULL;
    }
     

};




void BPTNodeLinkedList :: insertNode(ChildNode * node)
{
   int curSize = this->Size();
   int size = this->size;
   ChildNode * cur;
   ChildNode * prev;

   if (curSize<size)
   {
      cur = this->head;
       prev = NULL;
      

      while(cur!=NULL && cur->next!=NULL)
      {
         if (cur->key>node->key)
         {
           break;
         }
         else if (cur->key<node->key)
         {
            prev=cur;
            cur=cur->next;
         }
         else 
         {
            return;
         }
      }
       
     if (cur==NULL)
     {
        this->head = &*node;
        this->curSize++;
        return;
     }
     else 
     {
        if (cur->key>node->key)
        {prev->next =node;
        node->next = cur;
        this->curSize++;
        }
        else if (cur->key<node->key)
        {
          cur->next = node;
          this->curSize++;
        }
        else 
        {
            return;
        }
     }


   }
   else  //split required
   {
     cout<<"Split Required"<<endl;
     BPTNodeLinkedList newList(this->size);

     //inserting new node into linkedList and splitting if number of element reaches (n/2)
     int count=0;
     cur = this->head;
     prev=NULL;
     bool placed =false;

     if (cur->key>node->key)
     {
      node->next = cur;
      placed =true;
      this->head = node;      /// updated head here 
      // code for forwarding this update is to be added here 

     }

     while(count<((size+1)/2))
     {
       
        if (!placed)
        {
           // checking if new node can be placed at this position or not 
           if (cur->key>node->key)
           {
             prev->next=node;
             node->next =cur;
           }
           else 
           {
             prev = cur;
             cur = cur->next;
           }
           count++;
        }
        else{
          prev =cur ;
          cur =cur->next;
          count++;
        }
     }

    if (node->key<cur->key)
    {
        newList.head = node;
        prev->next = node;
        node->next = cur;
        placed=true;

    } else 
    { 
      newList.head = cur;
    }
    

     while(cur->next!=NULL && !placed)
     {
      // defining head of newList
       if (!placed)
        {
           // checking if new node can be placed at this position or not 
           if (cur->key>node->key)
           {
             prev->next=node;
             node->next =cur;
             placed=true;
             return;
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
        if (cur->key>node->key)
        {
          prev->next = node;
          node->next = cur;
        }
        else 
        {
           cur->next = node;
        }
     }



     //

   }

   
}


int BPTNodeLinkedList :: Size()   //returns curSize which is filled.
{
    return this->curSize;
}


int main()
{
    BPTNodeLinkedList l1 = BPTNodeLinkedList(4);

    ChildNode node1(1);
    l1.insertNode(&node1);

    ChildNode node2(2);
    l1.insertNode(&node2);

    ChildNode node3(3);
    l1.insertNode(&node3);

    ChildNode node4(4);
    l1.insertNode(&node4);
   
   ChildNode *temp = l1.head;
     while(temp!=NULL)
    {
        cout<<temp->key<<endl;
        temp= temp->next;
    }

    ChildNode node5(5);
    l1.insertNode(&node5);


    temp = l1.head;
     while(temp!=NULL)
    {
        cout<<temp->key<<endl;
        temp= temp->next;
    }
 

  


  

  
  return 0;

    
}