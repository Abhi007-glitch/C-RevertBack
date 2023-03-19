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
int remove(int key);
void removeNonLeaf(int val, Node* parentNode, Node* childTobeRemoved);
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






void BPTree :: insertAtNonLeaf(int val , Node** parentNode, Node ** childToBeInserted )
{
    Node * cur = (*parentNode);
  if ((*parentNode)->keys.size()<internalNodeLen) /// *********** #BHUT JAGHA HAI **************************
  {
    cout<<"************* Space available in InternalNode for insertion  ***************** "<<endl;
    
     
     // finding the right position for inserting the value using Binary Search
     int idx = upper_bound(cur->keys.begin(),cur->keys.end(),val)-cur->keys.begin();

     cur->keys.push_back(val);
     cur->childNode.treeptr.push_back(*childToBeInserted);
     

     // sepearate loop for keys and treePtr insertion - because we have one extra nodePtr at the start 
     for ( int i=cur->keys.size()-1;i>idx;i--)
     {
      cur->keys[i]=cur->keys[i-1];
     }
     cur->keys[idx]=val;

     // critical steps understand it well here main rearrangemet of pointers is being done -(understand why id+1 - by dry run)
     for ( int i =cur->childNode.treeptr.size()-1;i>idx+1;i--) 
     {
      cur->childNode.treeptr[i]=cur->childNode.treeptr[i-1];
     }
     cur->childNode.treeptr[idx+1]=(*childToBeInserted);
   
   cout<<"inserted into internal Nodes"<<endl;

  }
  else  /// **************** # NHI JAGHA HAI **********************************
  {
    
    cout<<"*********** Space not available in InternalNode ********************"<<endl;
   

    // arranging and sorting new key and ptr
    vector<int> tempkeys(cur->keys);
    vector<Node *> tempTreePtr(cur->childNode.treeptr);

    int idx = upper_bound(cur->keys.begin(),cur->keys.end(),val)-cur->keys.begin();

    tempkeys.push_back(val);
    tempTreePtr.push_back(*childToBeInserted);

    for (int i =tempkeys.size()-1;i>idx;i--)
    {
      tempkeys[i]=tempkeys[i-1];
    }

    tempkeys[idx]=val;

    for ( int i =tempTreePtr.size()-1;i>idx+1;i--)
    {
      tempTreePtr[i]=tempTreePtr[i-1];
    }
    tempTreePtr[idx+1]=*childToBeInserted;


    //splitting and creation of new Node 
    
    // creating new Ndoe 
    Node * newNode = new Node();
    newNode->isLeaf=false;
    new (&newNode->childNode.treeptr) vector<Node *>;  // defining and allocating space for union data member used in this node 


    // property of B+TREE - internal node pass value to their parent while the leafNode pass the copy of the key to the parent 
    //  this value would not be stored in any of current nodes it will be passed to the parent 
    int splitValue = tempkeys[tempkeys.size()/2]; 
  
    /// *******************************************  Spliting Keys **********************************************
    for ( int i =0;i<tempkeys.size()/2;i++)
    {
      cur->keys[i]= tempkeys[i];
    }


     // not include tempkeys.size()/2 index key in any of the split to as we have to send that key to the next parent in the tree structure
    for ( int i= tempkeys.size()/2+1;i<tempkeys.size();i++)
    {
       newNode->keys.push_back(tempkeys[i]);
    }
 

    // ********************************************* spliting nodePtrs *********************************************
    // if used tempTreePtr.size()/2 then it may not be equal to tempkeys.size()/2 thus using same tempkeys.size for both the splits
    for ( int i =0; i<tempkeys.size()/2+1; i++)  // one extra pointer the first pointer (rest all pair )
    {
       cur->childNode.treeptr[i]=tempTreePtr[i];
    }

    for ( int i=tempkeys.size()/2+1;i<tempTreePtr.size();i++)
    {
      newNode->childNode.treeptr.push_back(tempTreePtr[i]);
    }
    

    // resizing cur Node - as we use size of keys vector to determine the available space inside a node
    cur->keys.resize(tempkeys.size()/2);
    cur->childNode.treeptr.resize(tempkeys.size()/2+1);

    // creation of new root will take place as we have splitted the root
    if (cur==root)
    {
      Node* newRoot = new Node();
      newRoot->isLeaf =false;
      new (&newRoot->childNode.treeptr) vector<Node *>;
      newRoot->keys.push_back(splitValue);
      newRoot->childNode.treeptr.push_back(cur);
      newRoot->childNode.treeptr.push_back(newNode);
      root = newRoot;
      cout<<"Creation of new Node took place while Doing internal insertion operation."<<endl;

    }
    else 
    {
      cout<<"Internal Splitting Continues"<<endl;
      insertAtNonLeaf(splitValue,findParent(root,cur), &newNode);
    }

  }

};










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






int BPTree :: remove(int val)
{
 
 // finding the node containing key val 

Node * cur =root;
Node * parent =NULL;
int parentIndex = -1;

if (root == NULL) {
		cout << "B+ Tree is Empty" << endl;
		return 1;
	}

while(!cur->isLeaf)
{
   for (int i = 0; i < cur->keys.size(); i++) {
			parent = cur;
		
			if (val < cur->keys[i]) {
				cur = cur->childNode.treeptr[i];
				break;
			}
			if (i == cur->keys.size() - 1) {
				cur = cur->childNode.treeptr[i+1];
				break;
			}
		}
}
int index;
bool found=false ;// checking if value is present in the given node or not 
for ( index =0;index<cur->keys.size();index++)
{
   if (cur->keys[index]==val)
   {
    found=true;
    break;
   }
}

 // 
 

 /**
  *   deleting the key
  *  checking if it break the thershold condition
    if does not breaks condition then just stop
    if it breaks condition :
      a. look into sibling : (seprate code for left and right sibling)
      b. if sibling can't share - then merge with sibling and recursive call to delete the previos split node in parent 
  * 
  * 
  * **/





if (found==false)
{
cout<<val << " is not present in the B+ tree "<<endl; 
return -1; // return value to represent that given value is not present in the tree
}
else{
   
     cout<<"Deleted Node with key " <<val<< " and value "<<cur->childNode.dataptr[index]<<endl;
    //  shifting the key and dataptr

      for ( int i =index;i<cur->keys.size()-1;i++)
      {
        cur->keys[i]=cur->keys[i+1];
         cur->childNode.dataptr[i]=cur->childNode.dataptr[i+1];
      }
      
  
      cur->childNode.treeptr.resize(cur->keys.size()-1);
      cur->keys.resize(cur->keys.size()-1);

      if (cur==root)
      {
         if (cur->keys.size()==0)
         {
            root=NULL;
            cout<<"Our B+ tree got empty!!"<<endl;
            return 1;
         }
      }

      if(cur->keys.size()>=(leafNodeLen+1)/2)
      {
        return 1;
      }
      else 
      {
         // asking for key from sibling;
        
            // asking from left sibling 
            
            if (parentIndex>0 && parent->childNode.treeptr[parentIndex-1]->keys.size()>=(leafNodeLen+1)/2 +1)
            {
               Node* leftNode = parent->childNode.treeptr[parentIndex-1];
               //inserting data to node
               cur->keys.insert(cur->keys.begin(),leftNode->keys[leftNode->keys.size()-1]);
               cur->childNode.dataptr.insert(cur->childNode.dataptr.begin(),leftNode->childNode.dataptr[leftNode->keys.size()-1]);
               //resizing left silbing node
               leftNode->keys.resize(leftNode->keys.size()-1);
               leftNode->childNode.dataptr.resize(leftNode->keys.size()-1);
               // updating parent 
               parent->keys[index-1]=cur->keys[0];
               cout<<"Re-arrangement of data with left sibling done. "<<endl;
              return 1;
            }
           
           // asking from right sibling 
          
           if (parentIndex<parent->childNode.treeptr.size()-1 && parent->childNode.treeptr[parentIndex+1]->keys.size()>=(leafNodeLen+1)/2+1)
           {
               Node* rightNode =  parent->childNode.treeptr[parentIndex+1];
               
               //inserting value in cur node from rigth sibling
               cur->keys.push_back(rightNode->keys[0]);
               cur->childNode.dataptr.push_back(rightNode->childNode.dataptr[0]);

              //  // shifting value in right sibling node to reove first node value
              //  for ( int i =0;i<rightNode->keys.size()-1;i++)
              //  {
              //   rightNode->keys[i]=rightNode->keys[i+1];
              //   rightNode->childNode.dataptr[i]=rightNode->childNode.dataptr[i+1];  
              //  }

              rightNode->keys.erase(rightNode->keys.begin());
			        rightNode->childNode.dataptr.erase(rightNode->childNode.dataptr.begin());
              
               //resizing node
              //  rightNode->keys.resize(rightNode->keys.size()-1);
              //  rightNode->childNode.dataptr.resize(rightNode->keys.size()-1);


               // updaing parent 
               parent->keys[index]=rightNode->keys[0];
               cout<<"Re-arrangement of data with right sibling done. "<<endl;
               return 1;
           }


           // merging of node 

           // merging with left sibling node 

           if (parentIndex>0)
           {
               Node* leftNode = parent->childNode.treeptr[parentIndex-1];

               for ( int i =0;i<cur->keys.size();i++)
               {
                leftNode->keys.push_back(cur->keys[i]);
                leftNode->childNode.dataptr.push_back(cur->childNode.dataptr[i]);
               } 

               leftNode->next = cur->next; // maintaining linking between leaf node 

               cout<<"Merged node with it's left sibling"<<endl;
               removeNonLeaf(parent->keys[parentIndex-1],parent,cur);
               return 1;
           }
           else if (parentIndex<parent->keys.size()-1){
              Node* rightNode =  parent->childNode.treeptr[parentIndex+1];
              
              //shifting rigth sibling node value to cur node ( if want to shift data from cur to rigth sibling then will required to have previous node of cur -{which actually does not exists - as it would be present then we would have mergerd our node into it}) 
              for ( int i =0;i<rightNode->keys.size();i++)
              {
                cur->keys.push_back(rightNode->keys[i]);
                cur->childNode.dataptr.push_back(rightNode->childNode.dataptr[i]);
              }

              cur->next= rightNode->next;

              cout<<"Merged node with it's right sibling node"<<endl;
              removeNonLeaf(parent->keys[index],parent,rightNode);
              return 1;
             
           }

           return 1;
           

      }
   

}

    return 0;
}




void BPTree :: removeNonLeaf(int val , Node* cur, Node* childToBeRemoved)
{
 
 //checking if it's a root Node and has a size of 1 -> if so then we will have to create a new root

 if (cur == root)
 {
   if (cur->keys.size()==1)
   {
     if (cur->childNode.treeptr[0]==childToBeRemoved)
     {
       root = cur->childNode.treeptr[1];
       cout<<"New Root Created, Previous Root removed"<<endl;
       return;
     }
     else if (cur->childNode.treeptr[1]==childToBeRemoved)
     {
       root = cur->childNode.treeptr[0];
       cout<<"New Root Created, Previous Root removed"<<endl;
       return;
     }
     
   }
 }


 // removing key form internal node 

 int idx =0;
 for ( int i =0;i<cur->keys.size();i++)
 {
  if (cur->keys[i]==val)
  {
    idx=i;
    break;
  }
 }

 for ( int i=idx;i<cur->keys.size()-1;i++)
 {
     cur->keys[i]=cur->keys[i+1];
 }

 idx=0;

 for( int i=0;i<cur->childNode.treeptr.size();i++)
 {
    if (cur->childNode.treeptr[i]==childToBeRemoved)
    {
      idx= i;
      break;
    }
 }

 for ( int i=idx;i<cur->childNode.treeptr.size()-1;i++)
 {
  cur->childNode.treeptr[i]=cur->childNode.treeptr[i+1];
 }

 cur->keys.resize(cur->keys.size()-1);
 cur->childNode.treeptr.resize(cur->childNode.treeptr.size()-1);

 if (cur->keys.size()>= (internalNodeLen+1)/2 -1)
 {
  cout<<"Deletion completed"<<endl;
  return;
 }
 else 
 {
   // threshold value of the node breaked

  if (cur==root)  // root can have even 1 key only
  {
    return ;
  }


  // finding current nodes parent to get access to left chiild and right child 

  
  Node** p1 = findParent(root, cur);
	Node* parent = *p1;
  int parentIndex =0;
  
  for ( int i =0;i<parent->childNode.treeptr.size();i++)
  {
   if (parent->childNode.treeptr[i]==cur) 
   {
    parentIndex = i;
    break;
   }
  }



  

  // re-arragement form leftChild;
  
   if (parentIndex>0) //leftChild is present 
   {
    Node* leftNode = parent->childNode.treeptr[parentIndex-1];
    if (leftNode->keys.size()>=(internalNodeLen+1)/2)
    {
      // data flow == parent->cur then leftNode->parent
      //               
      cur->keys.insert(cur->keys.begin(),parent->keys[parentIndex-1]);
      parent->keys[parentIndex-1]= leftNode->keys[leftNode->keys.size()-1];
      leftNode->keys.resize(leftNode->keys.size()-1);
      

      cur->childNode.treeptr.insert(cur->childNode.treeptr.begin(),leftNode->childNode.treeptr[leftNode->childNode.treeptr.size()-1]);
      leftNode->childNode.treeptr.resize(leftNode->childNode.treeptr.size()-1);
      return ;

    }
   }
   else if (parentIndex<parent->childNode.treeptr.size()-1)
   {
      Node* rightNode = parent->childNode.treeptr[parentIndex+1];

      if (rightNode->keys.size()>= ((internalNodeLen+1)/2))
      {
        // dataflow ==   parent->cur  then rightNode ->parent 

        cur->keys.push_back(parent->keys[parentIndex]);
        parent->keys[parentIndex] = rightNode->keys[0];

        cur->childNode.treeptr.push_back(rightNode->childNode.treeptr[0]);

        for ( int i =0;i<rightNode->keys.size()-1;i++)
        {
            rightNode->keys[i]=rightNode->keys[i+1];
        }

        for ( int i =0;i<rightNode->childNode.treeptr.size()-1;i++)
        {
            rightNode->childNode.treeptr[i]=rightNode->childNode.treeptr[i+1];
        }
    
    rightNode->keys.resize(rightNode->keys.size()-1);
    rightNode->childNode.treeptr.resize(rightNode->childNode.treeptr.size()-1);

  return;
      }
   }
   else 
   {
    //----------------------------------- merging of internal node required --------------------------------

    if (parentIndex > 0) {
		//leftNode + parent key + cur
		Node* leftNode = parent->childNode.treeptr[parentIndex-1];
		leftNode->keys.push_back(parent->keys[parentIndex-1]);

		for (int val : cur->keys) {
			leftNode->keys.push_back(val);
		}

		for (int i = 0; i < cur->childNode.treeptr.size(); i++) {
			leftNode->childNode.treeptr.push_back(cur->childNode.treeptr[i]);
			cur->childNode.treeptr[i] = NULL;
		}

		cur->childNode.treeptr.resize(0);
		cur->keys.resize(0);

		removeNonLeaf(parent->keys[parentIndex-1], parent, cur);
		cout << "Merged with left sibling Node"<<endl;
	}
	else if (parentIndex+1 < parent->childNode.treeptr.size()) {
		//cur + parentkey +rightNode
		Node* rightNode = parent->childNode.treeptr[parentIndex+1];
		cur->keys.push_back(parent->keys[parentIndex]);

		for (int val : rightNode->keys) {
			cur->keys.push_back(val);
		}

		for (int i = 0; i < rightNode->childNode.treeptr.size(); i++) {
			cur->childNode.treeptr.push_back(rightNode->childNode.treeptr[i]);
			rightNode->childNode.treeptr[i] = NULL;
		}

		rightNode->childNode.treeptr.resize(0);
		rightNode->keys.resize(0);

		removeNonLeaf(parent->keys[parentIndex], parent, rightNode);
		cout << "Merged with right sibling Node"<<endl;;
	}

   }

  


 }




}



