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




void BPTree::insert(int key, int data) {  //in Leaf Node
    /*
		1. If the node has an empty space, insert the key/reference pair into the node.
		2. If the node is already full, split it into two nodes, distributing the keys
		evenly between the two nodes. If the node is a leaf, take a copy of the minimum
		value in the second of these two nodes and repeat this insertion algorithm to
		insert it into the parent node. If the node is a non-leaf, exclude the middle
		value during the split and repeat this insertion algorithm to insert this excluded
		value into the parent node.
	*/

    if (root == NULL) {
        root = new Node;
        root->isLeaf = true;
        root->keys.push_back(key);
        new (&root->childNode.dataptr) std::vector<int>;
        //// now, root->childNode.dataPtr is the active member of the union
        root->childNode.dataptr.push_back(data);

        cout << key << ": I AM ROOT!!" << endl;
        return;
    } else {
        Node* cursor = root;
        Node* parent = NULL;
        //searching for the possible position for the given key by doing the same procedure we did in search
        while (cursor->isLeaf == false) {
            parent = cursor;
            int idx = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor = cursor->childNode.treeptr[idx];
        }

        //now cursor is the leaf node in which we'll insert the new key
        if (cursor->keys.size() < leafNodeLen) {
            /*
				If current leaf Node is not FULL, find the correct position for the new key and insert!
			*/
            int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();
            cursor->keys.push_back(key);
            cursor->childNode.dataptr.push_back(data);

            if (i != cursor->keys.size() - 1) {
                for (int j = cursor->keys.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                    cursor->keys[j] = cursor->keys[j - 1];
                    cursor->childNode.dataptr[j] = cursor->childNode.dataptr[j - 1];
                }

                //since earlier step was just to inc. the size of vectors and making space, now we are simplying inserting
                cursor->keys[i] = key;
                cursor->childNode.dataptr[i] = data;
            }
            cout << "Inserted successfully: " << key << endl;
        } else {
            /*
				DAMN!! Node Overflowed :(
				HAIYYA! Splitting the Node .
			*/
            vector<int> virtualNode(cursor->keys);
            vector<int> virtualDataNode(cursor->childNode.dataptr);

            //finding the probable place to insert the key
            int i = std::upper_bound(cursor->keys.begin(), cursor->keys.end(), key) - cursor->keys.begin();

            virtualNode.push_back(key);          // to create space
            virtualDataNode.push_back(data);  // to create space

            if (i != virtualNode.size() - 1) {
                for (int j = virtualNode.size() - 1; j > i; j--) {  // shifting the position for keys and datapointer
                    virtualNode[j] = virtualNode[j - 1];
                    virtualDataNode[j] = virtualDataNode[j - 1];
                }

                //inserting
                virtualNode[i] = key;
                virtualDataNode[i] = data;
            }
            /*
				BAZINGA! I have the power to create new Leaf :)
			*/

            Node* newLeaf = new Node;
            newLeaf->isLeaf = true;
            new (&newLeaf->childNode.dataptr) std::vector<int>;
            //// now, newLeaf->childNode.treeptr is the active member of the union

            //swapping the next ptr
            Node* temp = cursor->next;
            cursor->next = newLeaf;
            newLeaf->next = temp;

            //resizing and copying the keys & dataPtr to OldNode
            cursor->keys.resize((leafNodeLen) / 2 +1);//check +1 or not while partitioning
            cursor->childNode.dataptr.resize((leafNodeLen) / 2 +1);
            for (int i = 0; i <= (leafNodeLen) / 2; i++) {
                cursor->keys[i] = virtualNode[i];
                cursor->childNode.dataptr[i] = virtualDataNode[i];
            }

            //Pushing new keys & dataPtr to NewNode
            for (int i = (leafNodeLen) / 2 + 1; i < virtualNode.size(); i++) {
                newLeaf->keys.push_back(virtualNode[i]);
                newLeaf->childNode.dataptr.push_back(virtualDataNode[i]);
            }

            if (cursor == root) {
                /*
					If cursor is root node we create new node
				*/

                Node* newRoot = new Node;
                newRoot->keys.push_back(newLeaf->keys[0]);
                new (&newRoot->childNode.treeptr) std::vector<Node*>;
                newRoot->childNode.treeptr.push_back(cursor);
                newRoot->childNode.treeptr.push_back(newLeaf);
                root = newRoot;
                cout << "Created new Root!" << endl;
            } else {
                // Insert new key in the parent
                insertAtNonLeaf(newLeaf->keys[0], &parent, &newLeaf);
            }
        }
    }
}







void BPTree :: insertAtNonLeaf(int val , Node** parentNode, Node ** childToBeInserted )
{
    Node * cur = (*parentNode);
  if ((*parentNode)->keys.size()<internalNodeLen) /// *********** #BHUT JAGHA HAI **************************
  {
    cout<<"************* #BHUT JAGA HE -InternalNode ***************** "<<endl;
    
     
     // finding the right position for inserting the value using Binary Search
     int idx = upper_bound(cur->keys.begin(),cur->keys.end(),val)-cur->keys.begin();

     cur->keys.push_back(val);
     cur->childNode.treeptr.push_back(*childToBeInserted);
     

     // sepearate loop for keys and treePtr inseration - because we have one extra nodePtr at the start 
     for ( int i=cur->keys.size()-1;i>idx;i--)
     {
      cur->keys[i]=cur->keys[i-1];
     }
     cur->keys[idx]=val;


     for ( int i =cur->childNode.treeptr.size()-1;i>idx-1;i--)
     {
      cur->childNode.treeptr[i]=cur->childNode.treeptr[i-1];
     }
     cur->childNode.treeptr[idx+1]=(*childToBeInserted);
   
   cout<<"inserted into internal Nodes"<<endl;

  }
  else  /// **************** # NHI JAGHA HAI **********************************
  {
    
    cout<<"*********** #NHI JAGHA HAI - InternalNode ********************"<<endl;
   

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

    for ( int i =tempTreePtr.size();i>idx+1;i--)
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


     // not include tempkeys.size()/2 index key in any of the split to as we have to send that key to the next paretn in the tree structure
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

}








Node* parent; // making it global to avoid redundent creation of parent node in each recursive call 
Node** BPTree :: findParent(Node* cur, Node* child)
{
   
  if(cur->isLeaf && cur->childNode.treeptr[0]->isLeaf)
  {
    return NULL;
  }
  
  // one possible optimization (smarter call during recursion) - using Binary search to find first value greater than child vlaue - applicable for case when we do not have repeted keys
  for ( int i =0;i<cur->childNode.treeptr.size();i++)
  {
    Node* temp;
    parent = cur->childNode.treeptr[i];
    if (cur->childNode.treeptr[i]==child)
    {
        parent =cur;
    }
    else 
    {
       temp = cur->childNode.treeptr[i];
       findParent(temp,child);
    }
  }

  return &parent;
};




Node* BPTree::firstLeftNode(Node* cursor) {
  if (cursor==NULL)
  {
    cout<<"Node is NULL"<<endl;
    return NULL;
  }
    if (cursor->isLeaf)
        return cursor;
    for (int i = 0; i < cursor->childNode.treeptr.size(); i++)
     {if (cursor->childNode.treeptr[0] != NULL)
            return firstLeftNode(cursor->childNode.treeptr[0]); 
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
				[first,last) which has a value greater than �val�.(Because we are storing the
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
          cout<<"Key is present and it's value is : "<<cursor->childNode.dataptr[idx]<<endl;
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