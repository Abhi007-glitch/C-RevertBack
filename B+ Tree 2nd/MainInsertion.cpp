#include <iostream>
#include <vector>
#include <B+tree.h>
#include <algorithm>


using namespace std;



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



