#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

const string MAGICITEMS_PATH = "magicitems.txt";

// from assignment 1
vector<string> getMagicItems(const string& filename) {
    vector<string> magicItems; 
    ifstream file(filename); //input file stream
    if (!file) {
        cerr << "File opening failed." << endl;
    }
    string line;
    while (getline(file, line)) {
        magicItems.push_back(line); // add line to vector
    }
    file.close();
    return magicItems;
};

string toLowerCase(const string& str) {
    string lowerStr = str;
    // Transforms a string to lowercase, citation: ChatGPT
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
};

template <typename T>
struct BinaryNode {
    T value;
    BinaryNode<T>* leftChild;
    BinaryNode<T>* rightChild;
};

class BinarySearchTree {
    private:
        BinaryNode<string>* root;

        // recursively destroy each child and its children
        void destroyTree(BinaryNode<string>* node) {
            if (node != nullptr) {
                destroyTree(node->leftChild);
                destroyTree(node->rightChild);
                delete node; // orphans created :(
            }; 
        };  

    public:
        // constructor
        BinarySearchTree() {
            root = nullptr;
        };
        // alternate constructor
        BinarySearchTree(string str) {
            // I have just now learned that creating a new node
            // does NOT set left and right to nullptr but instead they
            // are totally uninitialized. led to some weird bugs.
            root = new BinaryNode<string>{str, nullptr, nullptr};
        };
        // destructor
        ~BinarySearchTree() {
            destroyTree(root);
            root = nullptr;
        }; 

        // can give detailed (each node value) output or just L R !; ! meaning placement
        string insert(string str, bool shorthand) {
            BinaryNode<string>* newItem = new BinaryNode<string>{str, nullptr, nullptr};
            string moves = "\"" + str + "\" Insert Moves; ";
            if (root == nullptr) {
                root = newItem;
                if (!shorthand) { return moves += "Nullptr -> !"; }
                else { return moves += "!"; };
            }; 

            BinaryNode<string>* searchLocation = root;
            // when it becomes null we can place our new item!
            while (searchLocation != nullptr) {
                if (toLowerCase(newItem->value) < toLowerCase(searchLocation->value)) {
                    if (!shorthand) { moves += "(" + searchLocation->value + "):L -> "; } // go left
                    else { moves += " L"; };

                    if (searchLocation->leftChild == nullptr) {
                        searchLocation->leftChild = newItem;
                        if (!shorthand) { return moves += "Nullptr -> !"; }
                        else { return moves += " !"; };
                    }
                    searchLocation = searchLocation->leftChild;
                } else { // less than equal to
                    if (!shorthand) { moves += "(" + searchLocation->value + "):R -> "; } // go right
                    else { moves += " R"; };

                    if (searchLocation->rightChild == nullptr) {
                        searchLocation->rightChild = newItem;
                        if (!shorthand) { return moves += "Nullptr -> !"; }
                        else { return moves += " !"; };
                    }
                    searchLocation = searchLocation->rightChild;
                };
            };
            delete newItem; // we have bigger problems than this if this runs...
            return "Something went VERY wrong...";
        };

        // can give detailed (each node value) output or just L R !; ! meaning found
        string search(string str, bool shorthand) {
            BinaryNode<string>* searchItem = root;
            string path = "\"" + str + "\" Search Path; ";

            while (searchItem != nullptr) {
                if (searchItem->value == str) {
                    if (!shorthand) { return path += "(" + searchItem->value + ") -> !"; }
                    else { return path += " !"; };
                } else if (toLowerCase(str) < toLowerCase(searchItem->value)) {
                    if (!shorthand) { path += "(" + searchItem->value + "):L -> "; } // search left
                    else { path += " L"; };
                    searchItem = searchItem->leftChild;
                } else {
                    if (!shorthand) { path += "(" + searchItem->value + "):R -> "; } // search right
                    else { path += " R"; };
                    searchItem = searchItem->rightChild;
                };
            };
            if (!shorthand) { return path += "Nullptr -> Not Found"; }
            else { return path += "NF"; };
        };
};

int main() {
    cout << "BST Testing: " << endl;
    BinarySearchTree BST;
    cout << BST.insert("Root!", false) << endl; // root
    cout << BST.insert("Alpha", false) << endl; // L
    cout << BST.insert("Beta", false) << endl; // L R
    cout << BST.insert("Zebra", false) << endl; // R
    cout << BST.insert("Autumn", false) << endl; // L R L
    cout << BST.search("Root!", false) << endl;
    cout << BST.search("Alpha", false) << endl;
    cout << BST.search("Autumn", false) << endl;
    cout << BST.search("Not inserted", false) << endl;

    // load magic items
    cout << "\nLoading Magic Items into a BST:\n" << endl;
    vector<string> magicItems = getMagicItems(MAGICITEMS_PATH);
    BinarySearchTree* magicItemTree = new BinarySearchTree;
    for (string item : magicItems) {
        cout << magicItemTree->insert(item, true) << endl;
    };

    return 0;
};
