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
       
      
       // CASE 2: Space available inside leaf node
       if (cur->keys.size()<leafNodeLen)   
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
         


         // created new node;
         Node* nNode =new Node;  // newNode 
         nNode->isLeaf=true;
         // defining new vector for new Node inside union 
        new (&nNode->childNode.dataptr)vector<int>;

             // linking Nodes 
         Node * tempp = cur->next;
         cur->next = nNode;
         nNode->next=tempp;


         cur->keys.resize((leafNodeLen) / 2+1);  
         cur->childNode.dataptr.resize((leafNodeLen)/2+1);
         
         for ( int i=0;i<=leafNodeLen/2;i++)
         {
          cur->keys[i]=tempkey[i];
          cur->childNode.dataptr[i]=tempData[i];
         }

         // resizing vector - as we use size of vector as current size of the vector 
        

       

      
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
        cout<<"Passing value from leaf to internal Node - insertion at internal Node begins here"<<endl;
        insertAtNonLeaf(nNode->keys[0],&parent,&nNode);
       }
       
       }

    }

}






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

};



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