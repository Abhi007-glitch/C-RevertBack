#include <iostream>
#include <vector>
#include<algorithm>
#include<string>
#include "B+tree.h"
#include <cstdlib>

using namespace std;




void insertionMethod(BPTree** bPTree) {
    int id;
    int age;


    cout << "Please provide the Id: ";
    cin >> id;

    cout << "\nEnter your age";
    cin >> age;

    

    (*bPTree)->insert(id, age);
    
    cout << "Insertion of roll No: " << id << " Successful"<<endl;
}

void searchMethod(BPTree* tree) {
    int id;
    cout << "What's the id to Search? ";
    cin >> id;

    tree->search(id);
}



void printMethod(BPTree* bPTree) {
    int type;
    cout<<"1.Level Order Traversal"<<endl;
    cout<<"2.Seqential Traversal "<<endl;
    cout << "\nHere is your File Structure" << endl;
    
    cin>>type;
    if (type==1)
    {
    bPTree->printTreeLevelOrder(bPTree->root);
    }
    else{
   bPTree->printTree(bPTree->root);
    }   
        
}


void deleteMethod(BPTree* bPTree) {
    cout << "Showing you the Tree, Choose a key from here: " << endl;
    bPTree->printTree(bPTree->root);
 
    int keyTodelete;
    cout << "Enter a key to delete: " << endl;
    cin >> keyTodelete;
    bPTree->remove(keyTodelete);

    //Displaying
    bPTree->printTree(bPTree->root);
}


int main ()
{
    cout << "\n*** B+ tree visualisation**\n"
         << endl;

    bool flag = true;
    int option;

    int internalNodeLen = 4, leafNodeLen = 3;  // some random intial value
    cout << "Please provide the value to limit maximum child Internal Nodes can have: ";
    cin >> internalNodeLen;
    cout << "\nAnd Now Limit the value to limit maximum Nodes Leaf Nodes can have: ";
    cin >> leafNodeLen;

    BPTree* bPTree = new BPTree(leafNodeLen, internalNodeLen);

    do {
        cout << "\nPlease provide the queries with respective keys : " << endl;
        cout << "\tPress 1: Insertion \n\tPress 2: Search \n\tPress 3: Print Tree\n\tPress 4: Delete! \n\tPress 5: End the program"<<endl;
        cin >> option;

        switch (option) {
            case 1:
                insertionMethod(&bPTree);
                break;
            case 2:
                searchMethod(bPTree);
                break;
            case 3:
                printMethod(bPTree);
                break;
            case 4:
                deleteMethod(bPTree);
                break;
            case 5:
                flag = false;
                break;
            default :
               cout<<"Please enter a vaild command "<<endl;
               break;
        }
    }while (flag);

    vector<int>arr = {1,4,9,10,15,16,20,25,11,12,13};

    for ( int i =0;i<arr.size();i++)
    {
        bPTree->insert(arr[i],arr[i]);
    }


   
    // int n ;
    // cin>>n;
    // int temp=0;
    // for ( int i =0;i<n;i++)
    // {
    //     temp = rand()%10000;
    //    (bPTree)->insert(temp, temp);
    // }

    // printMethod(bPTree);
    // // printMethod(bPTree);


    // for (int i=0;i<arr.size()-1;i++)
    // {
    //      bPTree->remove(arr[i]);
    // bPTree->printTreeLevelOrder(bPTree->root);
    // }
   
    
    // //  bPTree->remove(20);
    // // bPTree->printTreeLevelOrder(bPTree->root);
    return 0;

}





