#include<iostream>
#include<vector>
#include<algorithm>
#include <B+tree.h>

using namespace std;


int BPTree :: remove(int val)
{
 
 // finding the node containing key val 

Node * cur =root;
Node * parent =NULL;
int parentIndex = -1;

while(!cur->isLeaf)
{
    parent=cur;
    int idx = upper_bound(cur->keys.begin(),cur->keys.end(),val)-cur->keys.begin();
    parentIndex=idx;
    cur = cur->childNode.treeptr[idx];
}

int index =-1; // checking if value is present in the given node or not 
for ( int i =0;i<cur->keys.size();i++)
{
   if (cur->keys[i]==val)
   {
    index=i;
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





if (index==-1)
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
      }
      
      for ( int i=index;i<cur->childNode.dataptr.size()-1;i++) 
      {
        cur->childNode.dataptr[i]=cur->childNode.dataptr[i+1];
      }
      cur->childNode.treeptr.resize(cur->childNode.dataptr.size()-1);
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
               cur->childNode.dataptr.insert(cur->childNode.dataptr.begin(),leftNode->keys[leftNode->keys.size()-1]);
               //resizing left silbing node
               leftNode->keys.resize(leftNode->keys.size()-1);
               leftNode->childNode.dataptr.resize(leftNode->keys.size()-1);
               // updating parent 
               parent->keys[index]=cur->keys[0];
               cout<<"Re-arrangement of data with left sibling done. "<<endl;
              return 1;
            }
           
           // asking from right sibling 
          
           if (parentIndex<parent->keys.size()-1 && parent->childNode.treeptr[parentIndex+1]->keys.size()>=(leafNodeLen+1)/2+1)
           {
               Node* rightNode =  parent->childNode.treeptr[parentIndex+1];
               
               //inserting value in cur node from rigth sibling
               cur->keys.push_back(rightNode->keys[0]);
               cur->childNode.dataptr.push_back(rightNode->childNode.dataptr[0]);

               // shifting value in right sibling node to reove first node value
               for ( int i =0;i<rightNode->keys.size()-1;i++)
               {
                rightNode->keys[i]=rightNode->keys[i+1];
                rightNode->childNode.dataptr[i]=rightNode->childNode.dataptr[i+1];  
               }
              
               //resizing node
               rightNode->keys.resize(rightNode->keys.size()-1);
               rightNode->childNode.dataptr.resize(rightNode->keys.size()-1);


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

               cout<<"Merged node with it's left sibling";
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
