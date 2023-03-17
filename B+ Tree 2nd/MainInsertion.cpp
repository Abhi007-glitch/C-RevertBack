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


