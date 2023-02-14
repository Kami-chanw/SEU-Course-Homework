#include <iostream>
#include <vector>

using namespace std;

class HashTable {
private:
    int          D;             // divisor for division hash function
    vector<int>* table;         // pointer to array of buckets
    int          num_elements;  // number of elements in the hash table

    // inner class for nodes in binary search tree
    struct TreeNode {
        int       key;
        TreeNode* left;
        TreeNode* right;
        TreeNode(int k) : key(k), left(nullptr), right(nullptr) {}
    };

    // array doubling function
    void doubleTable() {
        int          new_D     = 2 * D + 1;               // new divisor for division hash function
        vector<int>* new_table = new vector<int>[new_D];  // allocate new table
        for (int i = 0; i < D; i++) {
            for (int j = 0; j < table[i].size(); j++) {
                // rehash each element into new table
                int key   = table[i][j];
                int index = key % new_D;
                new_table[index].push_back(key);
            }
        }
        delete[] table;     // deallocate old table
        table = new_table;  // update pointer to new table
        D     = new_D;      // update divisor
    }

public:
    HashTable(int d) : D(d), num_elements(0) { table = new vector<int>[D]; }

    // insert an element into the hash table
    void insert(int key) {
        int index = key % D;
        if (table[index].empty()) {
            // if bucket is empty, add key directly to array
            table[index].push_back(key);
        }
        else {
            // otherwise, insert key into binary search tree
            TreeNode* root = new TreeNode(table[index][0]);
            for (int i = 1; i < table[index].size(); i++) {
                TreeNode* node = root;
                while (true) {
                    if (table[index][i] < node->key) {
                        if (node->left == nullptr) {
                            node->left = new TreeNode(table[index][i]);
                            break;
                        }
                        else {
                            node = node->left;
                        }
                    }
                    else {
                        if (node->right == nullptr) {
                            node->right = new TreeNode(table[index][i]);
                            break;
                        }
                        else {
                            node = node->right;
                        }
                    }
                }
            }
            // insert new key into binary search tree
            TreeNode* node = root;
            while (true) {
                if (key < node->key) {
                    if (node->left == nullptr) {
                        node->left = new TreeNode(key);
                        break;
                    }
                    else {
                        node = node->left;
                    }
                }
                else {
                    if (node->right == nullptr) {
                        node->right = new TreeNode(key);
                        break;
                    }
                    else {
                        node = node->right;
                    }
                }
            }
            // update bucket with new root of binary search tree
            table[index].clear();
            while (root != nullptr) {
                table[index].push_back(root->key);
                TreeNode* temp = root;
                root           = root->right;
                delete temp;
            }
        }
        num_elements++;
        if (num_elements > D) {
            if (num_elements > 2 * D) {
                // if loading density is too high, double the table size
                doubleTable();
            }
            else {
                // otherwise, just rehash all elements into new table
                vector<int>* new_table = new vector<int>[D];
                for (int i = 0; i < D; i++) {
                    for (int j = 0; j < table[i].size(); j++) {
                        int key   = table[i][j];
                        int index = key % D;
                        new_table[index].push_back(key);
                    }
                }
                delete[] table;
                table = new_table;
            }
        }
    }
    bool search(int key) {
        int index = key % D;
        if (table[index].empty()) {
            return false;
        }
        else {
            TreeNode* node = new TreeNode(table[index][0]);
            for (int i = 1; i < table[index].size(); i++) {
                TreeNode* temp = node;
                while (temp != nullptr) {
                    if (table[index][i] == temp->key) {
                        break;
                    }
                    else if (table[index][i] < temp->key) {
                        temp = temp->left;
                    }
                    else {
                        temp = temp->right;
                    }
                }
                if (temp == nullptr) {
                    // element not found in binary search tree
                    return false;
                }
            }
            // search for key in binary search tree
            while (node != nullptr) {
                if (key == node->key) {
                    return true;
                }
                else if (key < node->key) {
                    node = node->left;
                }
                else {
                    node = node->right;
                }
            }
            // key not found
            return false;
        }
    }
    // remove an element from the hash table
    void remove(int key) {
        int index = key % D;
        if (table[index].empty()) {
            return;
        }
        else {
            TreeNode* root = new TreeNode(table[index][0]);
            for (int i = 1; i < table[index].size(); i++) {
                TreeNode* node = root;
                while (true) {
                    if (table[index][i] < node->key) {
                        if (node->left == nullptr) {
                            node->left = new TreeNode(table[index][i]);
                            break;
                        }
                        else {
                            node = node->left;
                        }
                    }
                    else {
                        if (node->right == nullptr) {
                            node->right = new TreeNode(table[index][i]);
                            break;
                        }
                        else {
                            node = node->right;
                        }
                    }
                }
            }
            // search for node to remove in binary search tree
            TreeNode* node   = root;
            TreeNode* parent = nullptr;
            while (node != nullptr) {
                if (key == node->key) {
                    break;
                }
                else if (key < node->key) {
                    parent = node;
                    node   = node->left;
                }
                else {
                    parent = node;
                    node   = node->right;
                }
            }
            if (node == nullptr) {
                // key not found in binary search tree
                return;
            }
            // remove node from binary search tree
            if (node->left == nullptr && node->right == nullptr) {
                // node is a leaf
                if (parent == nullptr) {
                    // root node of binary search tree
                    table[index].clear();
                }
                else if (node == parent->left) {
                    parent->left = nullptr;
                }
                else {
                    parent->right = nullptr;
                }
            }
            else if (node->left == nullptr) {
                // node has only right child
                if (parent == nullptr) {
                    // root node of binary search tree
                    table[index].clear();
                    table[index].push_back(node->right->key);
                    delete node->right;
                }
                else if (node == parent->left) {
                    parent->left = node->right;
                }
                else {
                    parent->right = node->right;
                }
            }
            else if (node->right == nullptr) {
                // node has only left child
                if (parent == nullptr) {
                    // root node of binary search tree
                    table[index].clear();
                    table[index].push_back(node->left->key);
                    delete node->left;
                }
                else if (node == parent->left) {
                    parent->left = node->left;
                }
                else {
                    parent->right = node->left;
                }
            }
            else {
                // node has both left and right children
                TreeNode* temp = node->left;
                while (temp->right != nullptr) {
                    temp = temp->right;
                }
                node->key       = temp->key;
                int temp_key    = temp->key;
                temp->key       = node->left->key;
                node->left->key = temp_key;
                remove(temp->key);
            }
        }
    }
};

int main() {
    int       D = 11;
    HashTable hash_table(D);
    hash_table.insert(7);
    hash_table.insert(25);
    hash_table.insert(18);
    hash_table.insert(10);
    hash_table.insert(13);
    hash_table.insert(5);
    hash_table.insert(4);
    hash_table.insert(2);
    hash_table.insert(3);
    hash_table.insert(12);
    hash_table.insert(8);
    hash_table.insert(15);
    hash_table.insert(14);
    hash_table.insert(17);
    hash_table.insert(9);
    hash_table.insert(16);
    hash_table.insert(19);
    hash_table.insert(21);
    hash_table.insert(24);
    hash_table.insert(23);
    hash_table.insert(20);
    hash_table.insert(1);
    hash_table.insert(22);
    cout << hash_table.search(15) << endl;  // output: 1 (true)
    hash_table.remove(15);
    cout << hash_table.search(15) << endl;  // output: 0 (false)
    return 0;
}
