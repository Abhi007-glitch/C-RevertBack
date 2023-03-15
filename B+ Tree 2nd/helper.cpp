#include<iostream>
#include<vector>
#include<B+tree.h>
#include<queue>

using namespace std;

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
    if (cursor->isLeaf)
        return cursor;
    
        if (cursor->childNode.treeptr[0] != NULL)
            return firstLeftNode(cursor->childNode.treeptr[0]);

    return NULL;
}

void BPTree :: printTree(Node * cursor)
{
   Node* firstLeft = firstLeftNode(cursor);

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