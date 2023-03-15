#include <iostream>
#include <vector>
#include<algorithm>
#include<string>
#include "B+tree.h"

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
    

    cout << "\nHere is your File Structure" << endl;
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
        cout << "\tPress 1: Insertion \n\tPress 2: Search \n\tPress 3: Print Tree\n\tPress 4: ABORT!" << endl;
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
            default:
                flag = false;
                break;
        }
    }while (flag);

    return 0;
}