#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>

using namespace std;




class Node
{
    // node consist of vector of key and vector of vector of pointer

    public:
      vector<int>keys;

      
      // used union because it optimally uses memory and provide us any of the data memeber defined inside it to use anywhere and takes storage for only data member which has some value 
      /**
       * 
       * *************************************************************** Facts About Union *******************************************************
       * 1. Syntax very similar to class.
       * 2. Has space equal to - one data member having largest size among all data members 
       * 3. Stored value inside union is the last value updated/ stored inside union it can be any one the data members
       * 
       * 
       * **/

      union ptr{ 
        vector<Node *>treeptr; // for intermediate Node 
        vector<int>dataptr; // for leaf node 

        ptr();
        ~ptr();
      }childNode; //an alias name

      
      Node* next; // ptr to next node
      bool isLeaf;

      Node()
      {
        this->next = NULL;
        this->isLeaf =false;
        
      }; 
     
       friend class BPTree;


      
};



Node::ptr::ptr() {
}

Node::ptr::~ptr() {
}

class BPTree{

public :
Node* root ;
int leafNodeLen;
int internalNodeLen;

BPTree(int leafNodeLen, int internalNodeLen)
{
   this->leafNodeLen=leafNodeLen;
   this->internalNodeLen=internalNodeLen;
   this->root=NULL;
}

// functions 
void search(int key);
void insert(int key, int data);
void insertAtNonLeaf(int val, Node ** parentNode, Node ** childToBeInserted);
int remove();
int removeNonLeaf();
void printTree(Node * cur);
void printTreeLevelOrder(Node * cur);
Node* firstLeftNode(Node* cursor);


//helper functions
Node** findParent(Node* cur, Node* child);

// BPTree(int leafNodeLen, int internallNodeLen); -- giving error check why
// {
  
//   this->root =NULL;
//   this->leadNodeLen = leafNodeLen;
//   this->internalNodeLen = internalNodeLen;
// };


};





void BPTree :: insert(int key, int data)
{
   

   // *********************************** CASE 1 : if tree was empty ***************************

    if (root==NULL)
    {
     root = new Node;
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
       
        
       // CASE 2: Space available inside leaf node
       if (cur->keys.size()<leafNodeLen)   
       {
        // can insert value in this node 
        // finding the right position to insert new node

        int ix = upper_bound(cur->keys.begin(),cur->keys.end(),key) -cur->keys.begin();
        
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
         


         // created new node;
         Node* nNode =new Node;  // newNode 
         nNode->isLeaf=true;
         // defining new vector for new Node inside union 
        new (&nNode->childNode.dataptr)vector<int>;

             // linking Nodes 
         Node * tempp = cur->next;
         cur->next = nNode;
         nNode->next=tempp;

 // resizing vector - as we use size of vector as current size of the vector 
        
         cur->keys.resize((leafNodeLen) / 2+1);  
         cur->childNode.dataptr.resize((leafNodeLen)/2+1);
         
         for ( int i=0;i<=leafNodeLen/2;i++)
         {
          cur->keys[i]=tempkey[i];
          cur->childNode.dataptr[i]=tempData[i];
         }
         

      
        for ( int i =(leafNodeLen)/2+1;i<tempkey.size();i++)
        {
         nNode->keys.push_back(tempkey[i]);
         nNode->childNode.dataptr.push_back(tempkey[i]);
        }

 
    
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
        cout<<"Passing value from leaf to internal Node - insertion at internal Node begins here "<<endl;
        insertAtNonLeaf(nNode->keys[0],&parent,&nNode);
       }
       
       }

    }

}












// void BPTree :: insertAtNonLeaf(int val , Node** parentNode, Node ** childToBeInserted )
// {
//     Node * cur = (*parentNode);
//   if ((*parentNode)->keys.size()<internalNodeLen) /// *********** #BHUT JAGHA HAI **************************
//   {
//     cout<<"************* #BHUT JAGA HE -InternalNode ***************** "<<endl;
    
     
//      // finding the right position for inserting the value using Binary Search
//      int idx = upper_bound(cur->keys.begin(),cur->keys.end(),val)-cur->keys.begin();

//      cur->keys.push_back(val);
//      cur->childNode.treeptr.push_back(*childToBeInserted);
     

//      // sepearate loop for keys and treePtr inseration - because we have one extra nodePtr at the start 
//      for ( int i=cur->keys.size()-1;i>idx;i--)
//      {
//       cur->keys[i]=cur->keys[i-1];
//      }
//      cur->keys[idx]=val;


//      for ( int i =cur->childNode.treeptr.size()-1;i>idx-1;i--)
//      {
//       cur->childNode.treeptr[i]=cur->childNode.treeptr[i-1];
//      }
//      cur->childNode.treeptr[idx+1]=(*childToBeInserted);
   
//    cout<<"inserted into internal Nodes"<<endl;

//   }
//   else  /// **************** # NHI JAGHA HAI **********************************
//   {
    
//     cout<<"*********** #NHI JAGHA HAI - InternalNode ********************"<<endl;
   

//     // arranging and sorting new key and ptr
//     vector<int> tempkeys(cur->keys);
//     vector<Node *> tempTreePtr(cur->childNode.treeptr);

//     int idx = upper_bound(cur->keys.begin(),cur->keys.end(),val)-cur->keys.begin();

//     tempkeys.push_back(val);
//     tempTreePtr.push_back(*childToBeInserted);

//     for (int i =tempkeys.size()-1;i>idx;i--)
//     {
//       tempkeys[i]=tempkeys[i-1];
//     }
//     tempkeys[idx]=val;

//     for ( int i =tempTreePtr.size();i>idx+1;i--)
//     {
//       tempTreePtr[i]=tempTreePtr[i-1];
//     }
//     tempTreePtr[idx+1]=*childToBeInserted;


//     //splitting and creation of new Node 
    
//     // creating new Ndoe 
//     Node * newNode = new Node();
//     newNode->isLeaf=false;
//     new (&newNode->childNode.treeptr) vector<Node *>;  // defining and allocating space for union data member used in this node 


//     // property of B+TREE - internal node pass value to their parent while the leafNode pass the copy of the key to the parent 
//     //  this value would not be stored in any of current nodes it will be passed to the parent 
//     int splitValue = tempkeys[tempkeys.size()/2]; 
  
//     /// *******************************************  Spliting Keys **********************************************
//     for ( int i =0;i<tempkeys.size()/2;i++)
//     {
//       cur->keys[i]= tempkeys[i];
//     }


//      // not include tempkeys.size()/2 index key in any of the split to as we have to send that key to the next paretn in the tree structure
//     for ( int i= tempkeys.size()/2+1;i<tempkeys.size();i++)
//     {
//        newNode->keys.push_back(tempkeys[i]);
//     }
 

//     // ********************************************* spliting nodePtrs *********************************************
//     // if used tempTreePtr.size()/2 then it may not be equal to tempkeys.size()/2 thus using same tempkeys.size for both the splits
//     for ( int i =0; i<tempkeys.size()/2+1; i++)  // one extra pointer the first pointer (rest all pair )
//     {
//        cur->childNode.treeptr[i]=tempTreePtr[i];
//     }

//     for ( int i=tempkeys.size()/2+1;i<tempTreePtr.size();i++)
//     {
//       newNode->childNode.treeptr.push_back(tempTreePtr[i]);
//     }
    

//     // resizing cur Node - as we use size of keys vector to determine the available space inside a node
//     cur->keys.resize(tempkeys.size()/2);
//     cur->childNode.treeptr.resize(tempkeys.size()/2+1);

//     // creation of new root will take place as we have splitted the root
//     if (cur==root)
//     {
//       Node* newRoot = new Node();
//       newRoot->isLeaf =false;
//       new (&newRoot->childNode.treeptr) vector<Node *>;
//       newRoot->keys.push_back(splitValue);
//       newRoot->childNode.treeptr.push_back(cur);
//       newRoot->childNode.treeptr.push_back(newNode);
//       root = newRoot;
//       cout<<"Creation of new Node took place while Doing internal insertion operation."<<endl;

//     }
//     else 
//     {
//       cout<<"Internal Splitting Continues"<<endl;
//       insertAtNonLeaf(splitValue,findParent(root,cur), &newNode);
//     }

//   }

// }






void BPTree :: insertAtNonLeaf(int val , Node** cursor, Node ** childToBeInserted )
{
  if ((*cursor)->keys.size() < internalNodeLen - 1) {
        /*
			If cursor is not full find the position for the new key.
		*/
        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), val) - (*cursor)->keys.begin();
        (*cursor)->keys.push_back(val);
        //new (&(*cursor)->childNode.treeptr) std::vector<Node*>;
        //// now, root->childNode.treeptr is the active member of the union
        (*cursor)->childNode.treeptr.push_back(*childToBeInserted);

        if (i != (*cursor)->keys.size() - 1) {  // if there are more than one element
            // Different loops because size is different for both (i.e. diff of one)

            for (int j = (*cursor)->keys.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                (*cursor)->keys[j] = (*cursor)->keys[j - 1];
            }

            for (int j = (*cursor)->childNode.treeptr.size() - 1; j > (i + 1); j--) {
                (*cursor)->childNode.treeptr[j] = (*cursor)->childNode.treeptr[j - 1];
            }

            (*cursor)->keys[i] = val;
            (*cursor)->childNode.treeptr[i + 1] = *childToBeInserted;
        }
        cout << "Inserted key in the internal node :)" << endl;
    } else {  //splitting
        cout << "Inserted Node in internal node successful" << endl;
        cout << "Overflow in internal:( HAIYAA! splitting internal nodes" << endl;

        vector<int> virtualKeyNode((*cursor)->keys);
        vector<Node*> virtualTreePtrNode((*cursor)->childNode.treeptr);

        int i = std::upper_bound((*cursor)->keys.begin(), (*cursor)->keys.end(), val) - (*cursor)->keys.begin();  //finding the position for val
        virtualKeyNode.push_back(val);                                                                   // to create space
        virtualTreePtrNode.push_back(*childToBeInserted);                                                           // to create space

        if (i != virtualKeyNode.size() - 1) {
            for (int j = virtualKeyNode.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                virtualKeyNode[j] = virtualKeyNode[j - 1];
            }

            for (int j = virtualTreePtrNode.size() - 1; j > (i + 1); j--) {
                virtualTreePtrNode[j] = virtualTreePtrNode[j - 1];
            }

            virtualKeyNode[i] = val;
            virtualTreePtrNode[i + 1] = *childToBeInserted;
        }

        int partitionKey;                                            //exclude middle element while splitting
        partitionKey = virtualKeyNode[(virtualKeyNode.size() / 2)];  //right biased
        int partitionIdx = (virtualKeyNode.size() / 2);

        //resizing and copying the keys & TreePtr to OldNode
        (*cursor)->keys.resize(partitionIdx);
        (*cursor)->childNode.treeptr.resize(partitionIdx + 1);
        (*cursor)->childNode.treeptr.resize(partitionIdx + 1);
        for (int i = 0; i < partitionIdx; i++) {
            (*cursor)->keys[i] = virtualKeyNode[i];
        }

        for (int i = 0; i < partitionIdx + 1; i++) {
            (*cursor)->childNode.treeptr[i] = virtualTreePtrNode[i];
        }

        Node* newInternalNode = new Node;
        new (&newInternalNode->childNode.treeptr) std::vector<Node*>;
        //Pushing new keys & TreePtr to NewNode

        for (int i = partitionIdx + 1; i < virtualKeyNode.size(); i++) {
            newInternalNode->keys.push_back(virtualKeyNode[i]);
        }

        for (int i = partitionIdx + 1; i < virtualTreePtrNode.size(); i++) {  // because only key is excluded not the pointer
            newInternalNode->childNode.treeptr.push_back(virtualTreePtrNode[i]);
        }

        if ((*cursor) == root) {
            /*
				If cursor is a root we create a new Node
			*/
            Node* newRoot = new Node;
            newRoot->keys.push_back(partitionKey);
            new (&newRoot->childNode.treeptr) std::vector<Node*>;
            newRoot->childNode.treeptr.push_back(*cursor);
            //// now, newRoot->childNode.treeptr is the active member of the union
            newRoot->childNode.treeptr.push_back(newInternalNode);

            root = newRoot;
            cout << "Created new ROOT!" << endl;
        } else {
            /*
				::Recursion::
			*/
            insertAtNonLeaf(partitionKey, findParent(root, *cursor), &newInternalNode);
        }
    }
}






Node* parent; // making it global to avoid redundent creation of parent node in each recursive call 


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

// Node** BPTree::findParent(Node* cursor, Node* child) {
//     /*
// 		Finds parent using depth first traversal and ignores leaf nodes as they cannot be parents
// 		also ignores second last level because we will never find parent of a leaf node during insertion using this function
// 	*/

//     if (cursor->isLeaf || cursor->childNode.treeptr[0]->isLeaf)
//         return NULL;
 
    
//     for (int i = 0; i < cursor->childNode.treeptr.size(); i++) {
//         if (cursor->childNode.treeptr[i] == child) {
//             parent = cursor;
//         } else {
           
//             Node* tmpCursor = cursor->childNode.treeptr[i];
//             findParent(tmpCursor, child);
//         }
//     }

//     return &parent;
// }




Node* BPTree::firstLeftNode(Node* cursor) {
  if (cursor==NULL)
  {
    cout<<"Node is NULL"<<endl;
    return NULL;
  }
    if (cursor->isLeaf)
        return cursor;
    for (int i = 0; i < cursor->childNode.treeptr.size(); i++)
     {if (cursor->childNode.treeptr[i] != NULL)
            return firstLeftNode(cursor->childNode.treeptr[i]); 
     }
    return NULL;
}

void BPTree :: printTree(Node * cursor)
{
   Node* firstLeft = firstLeftNode(cursor);
  cout<<"FirstNode Value"<<cursor->keys[0]<<endl;

    if (firstLeft == NULL) {
        cout << "No Data in the Database yet!" << endl;
        return;
    }
    while (firstLeft != NULL) {
        for (int i = 0; i < firstLeft->keys.size(); i++) {
            cout << firstLeft->keys[i] << " ";
        }

        firstLeft = firstLeft->next;
    }
    cout << endl;
}

void BPTree :: search(int key)
{
     if (root == NULL) {
        cout << "NO Tuples Inserted yet" << endl;
        return;
    } else {
        Node* cursor = root;
        while (cursor->isLeaf == false) {
            /*
				upper_bound returns an iterator pointing to the first element in the range
				[first,last) which has a value greater than key.(Because we are storing the
				same value in the right node;(STL is doing Binary search at back end)
			*/
            int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->childNode.treeptr[idx];  //upper_bound takes care of all the edge cases
        }

        int idx = std::lower_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();  //Binary search

        if (idx == cursor->keys.size() || cursor->keys[idx] != key) {
            cout << "HUH!! Key NOT FOUND" << endl;
            return;
        }
        else 
        {
          cout<<"Key is present and it's value is : "<<cursor->childNode.dataptr[idx]<<" Key is"<<key<<endl;
        }

}
}


void BPTree :: printTreeLevelOrder(Node* cursor)
{
      if (cursor == NULL) return;
    queue<Node*> q;
    q.push(cursor);

    while (!q.empty()) {
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            Node* u = q.front(); q.pop();

            //printing keys in self
            for (int val : u->keys)
                cout << val << " ";

            cout << "|| ";//to seperate next adjacent nodes
            
            if (u->isLeaf != true) {
                for (Node* v : u->childNode.treeptr) {
                    q.push(v);
                }
            }
        }
        cout << endl;
    }
}