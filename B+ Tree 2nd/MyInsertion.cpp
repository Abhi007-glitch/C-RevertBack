#include <iostream>
#include <vector>
#include <B+tree.h>
#include <algorithm>


using namespace std;
void BPTree :: insert(int key, int data)
{
   

   // *********************************** CASE 1 : if tree was empty ***************************

    if (root==NULL)
    {
     root = new Node();
     root->isLeaf=true;
     root->keys.push_back(key);
     new (&root->childNode.dataptr)vector<int>; // alocating memory to child
     root->childNode.dataptr.push_back(data);
     cout<<"Created a root with value " << key <<endl;
     return;
    }
    else   
    {  
        Node* cur = root;
        Node* parent=NULL;
        

        //finding the right leaf node to insert key
        while(!cur->isLeaf)
        {   
          parent=cur;
            int idx = upper_bound(cur->keys.begin(),cur->keys.end(),key) -cur->keys.begin(); // searing for first value who is greate than key - because ptr at i index stores value lesser than key value at i in keys vector
            cur = cur->childNode.treeptr[idx];
        }
       
       int size = cur->keys.size();
       // CASE 2: Space available inside leaf node
       if (size<leafNodeLen)   
       {
        // can insert value in this node 
        // finding the right position to insert new node

        int ix = upper_bound(cur->keys.begin(),cur->keys.end(),key) -cur->keys.end();
        
        cout<<"index where to insert new value "<<ix<<endl;
        //inserting at end 
        cur->keys.push_back(key);
        cur->childNode.dataptr.push_back(data);
        
        //shifting values to place new inserted value at right position 
      if (ix!=cur->keys.size()-1) 
        {
        for ( int i = cur->keys.size()-1 ;i>ix;i--)
        {
            cur->keys[i]=cur->keys[i-1];
            cur->childNode.dataptr[i]=cur->childNode.dataptr[i-1];
        }
        cur->keys[ix]=key;
        cur->childNode.dataptr[ix]=data;
         }
      cout<<"inserted Node Successfully"<<endl;
       }
       else 
       {
        // need to create a new node - overflow split of node will take place 
         
         

         //creating temproray storage to store all data (new values too ) then split this data between older and newNdoe 
         vector<int> tempkey(cur->keys);
         vector<int> tempData(cur->childNode.dataptr);   // THESE VECTOR CAN BE REMOVED -NOT REQUIRED

         //pushing new value 
           int ix = std::upper_bound(cur->keys.begin(), cur->keys.end(), key) - cur->keys.begin();
         tempkey.push_back(key);
         tempData.push_back(data);
         int temp;


         if(ix!=tempkey.size()-1){
          
          for (int i =tempkey.size()-1;i>ix;i--)
         {
               
              tempkey[i] = tempkey[i - 1];
              tempData[i] = tempData[i - 1];
          
         
         }
          tempData[ix]=data;
          tempkey[ix]=key;
         }
         

         for ( int i=0;i<=leafNodeLen/2;i++)
         {
          cur->keys[i]=tempkey[i];
          cur->childNode.dataptr[i]=tempData[i];
         }

         // resizing vector - as we use size of vector as current size of the vector 
         cur->keys.resize((leafNodeLen) / 2+1);  
         cur->childNode.dataptr.resize((leafNodeLen)/2+1);
         

         // created new node;
         Node* nNode =new Node();  // newNode 
         nNode->isLeaf=true;

        // defining new vector for new Node inside union 
        new (&nNode->childNode.dataptr)vector<int>;
        for ( int i =leafNodeLen/2+1;i<tempkey.size();i++)
        {
         nNode->keys.push_back(tempkey[i]);
         nNode->childNode.dataptr.push_back(tempkey[i]);
        }

 
        // linking Nodes 
       Node * tempp = cur->next;
       cur->next = nNode;
       nNode->next=tempp;
       
       // we need to create a new root of the tree
       if (cur==root)
       {
         Node * newRoot = new Node();
         newRoot->isLeaf=false;
         newRoot->keys.push_back(nNode->keys[0]);
         new (&newRoot->childNode.treeptr)vector<Node *>;
         newRoot->childNode.treeptr.push_back(cur);
         newRoot->childNode.treeptr.push_back(nNode);
         root = newRoot;
         cout<<"Created a new Root after insertion of new key value"<<endl;
       }
       else 
       {
        // passing updates to parent nodes;
        cout<<"Passing value from leaf to internal Node - insertion at internal Node begins here"<<endl;
        insertAtNonLeaf(nNode->keys[0],&parent,&nNode);
       }
       
       }

    }

}




Node** BPTree :: findParent(Node* cur, Node* child)
{
   
  if(cur->isLeaf || cur->childNode.treeptr[0]->isLeaf)
  {
    return NULL;
  }
  
  // one possible optimization (smarter call during recursion) - using Binary search to find first value greater than child vlaue - applicable for case when we do not have repeted keys
  for ( int i =0;i<cur->childNode.treeptr.size();i++)
  {
    
    if (cur->childNode.treeptr[i]==child)
    {
        parent =cur;   
    }
    else 
    {
       Node* temp = cur->childNode.treeptr[i];
       findParent(temp,child);
    }
  }

  return &parent;
};

