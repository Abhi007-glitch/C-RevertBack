#include<iostream>
#include<vector>
#include<algorithm>
#include<B+tree.h>

using namespace std;


void BPTree :: search(int key)
{
 Node * cur = root;
 
 if (root==NULL)
 {
    cout<<"Tree is empty - no value present"<<endl;
 }

 while(!cur->isLeaf)
 {
    int idx = upper_bound(cur->keys.begin(),cur->keys.end(), key) - cur->keys.begin();
    cur =cur->childNode.treeptr[idx];
 }

 int idx= lower_bound(cur->keys.begin(),cur->keys.end(),key)-cur->keys.begin();

 if (idx==cur->keys.size() || cur->keys[idx]!=key)
 {
    cout<<"Value is not present in the b+Tree"<<endl;
 }

}