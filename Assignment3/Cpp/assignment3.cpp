#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <regex>
#include <iomanip>
using namespace std;

const string MAGICITEMS_PATH = "magicitems.txt";
const string ITEMS_2_FIND_PATH = "magicitems-find-in-bst.txt";
const string GRAPH_PATH = "graphs1.txt";

// from assignment 1
vector<string> getMagicItems(const string& filename) {
    vector<string> magicItems; 
    ifstream file(filename); // input file stream
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

        // awkward but need to start recursing
        // this will put items in order!
        void inOrderTraversal() { // left root right
            traverseInOrder(root);
        };

        // recursively visit left children, root, then right
        void traverseInOrder(BinaryNode<string>* root) {
            if (root == nullptr) { return; };
            traverseInOrder(root->leftChild);
            cout << "\"" << root->value << "\", ";
            traverseInOrder(root->rightChild);
        };
};

// we can just look at the path to tell comparisons
// ! counts as we need to check to make sure its the right item
int checkComparisons(const string& str) {
    int count = 0;
    bool afterSemicolon = false;
    for (char c : str) {
        if (c == ';') {
            afterSemicolon = true;
        } else if (afterSemicolon && (c == 'L' || c == 'R' || c == '!')) {
            count++;
        };
    };
    return count;
};

class Graph {
    private:
        // I used a map here as we may not have all vectors between 1 and 10, for example
        // Each map vertex stores a list of neighbors
        map<int, vector<int>> adjacencyRep; 

        // not taking map approach with this one
        vector<vector<int>> matrixRep;
    public:
        Graph() {
            // automatically create vertex 0
            this->matrixRep.push_back(vector<int>(1, 0)); 
        }

        void addVertex(int vertex) {
            this->adjacencyRep[vertex]; // start an adj for the vertex
            
            // default to no neighbors
            // add all vertices between the last one we have and the new one
            // ex. adding #9 but we only have up to 7 will add #8 as well
            // this will ensure everything stays in order and matrix is symmetrical 
            if (!(vertex < static_cast<int>(this->matrixRep.size()))) { // vertex was not created already
                int rowsNeeded = vertex - static_cast<int>(this->matrixRep.size()); // should always be 1 unless added out of order
                for (int i = 0; i <= rowsNeeded; ++i) {
                    this->matrixRep.push_back(vector<int>(matrixRep[0].size(), 0)); // new row
                    // update relations for new row (no relations unless edge added)
                    for (vector<int>& vertex : this->matrixRep) {
                        vertex.push_back(0);
                    }   
                }
            }
        };

        void addEdge(int vertex1, int vertex2) {
            // must do both as it is undirected
            this->adjacencyRep[vertex1].push_back(vertex2);
            this->adjacencyRep[vertex2].push_back(vertex1);

            this->matrixRep[vertex1][vertex2] = 1;
            this->matrixRep[vertex2][vertex1] = 1;
        };
        
        void displayAdj() {
            cout << "\nAdjacency List:" << endl;
            // Generated by ChatGPT as I was unfamiliar with printing maps
            for (const auto& pair : this->adjacencyRep) {
                cout << setw(3) << pair.first << ": ";
                for (int neighbor : pair.second) {
                    cout << neighbor << " ";
                }
                cout << endl;
            }
        };

        void displayMatrix() {
            cout << "\nMatrix:" << endl;
            for(size_t i = 0; i < this->matrixRep.size(); ++i) {
                cout << setw(3) << i << ": ";
                for(size_t j = 0; j < this->matrixRep[i].size(); ++j) {
                    cout << this->matrixRep[i][j] << " ";
                }
                cout << endl;
            }
        };

        void displayGraph() {
            this->displayAdj();
            this->displayMatrix();
        };

        bool isEmpty() {
            // if adj list has nothing graph empty
            return this->adjacencyRep.empty();
        };
};

vector<Graph> createGraphs(const string& filename) {
    vector<Graph> graphs;
    Graph currentGraph;
    // I am taking formal lang so regex it'll be!
    regex newGraphRe(R"(new graph)");
    regex addVertexRe(R"(add vertex (\d+))");
    regex addEdgeRe(R"(add edge (\d+)\s*-\s*(\d+))");

    ifstream file(filename); // input file stream
    if (!file) {
        cerr << "File opening failed." << endl;
    }
    string instruction;

    while (getline(file, instruction)) {
        // ignore any commands we don't know, empty lines, comments etc.
        // regex will allow some slack with white space, but assuming perfect syntax by user

        // case 1: start a new graph
        if (regex_match(instruction, newGraphRe)) {
            // check to see if the graph has anything in it
            if (!currentGraph.isEmpty()) {
                graphs.push_back(currentGraph); // save the old graph
                currentGraph = Graph(); // start a new graph
            }
        } else {
            smatch match; // captures subexpressions/groups

            if (regex_match(instruction, match, addVertexRe)) { // case 2: new vertex
                int newVertex = stoi(match[1].str());
                currentGraph.addVertex(newVertex);
            } else if (regex_match(instruction, match, addEdgeRe)) { // case 3: new edge
                int v1 = stoi(match[1].str());
                int v2 = stoi(match[2].str());
                currentGraph.addEdge(v1, v2);
            };
        };
    };
    file.close();
    return graphs;
};

int main() {
    cout << "\nBST Testing: " << endl;
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
    cout << "\nIn-order Traversal (Left Root Right):" << endl;
    BST.inOrderTraversal(); // they will be in order!

    // load magic items
    cout << "\nLoading Magic Items into a BST:\n" << endl;
    vector<string> magicItems = getMagicItems(MAGICITEMS_PATH);
    BinarySearchTree* magicItemTree = new BinarySearchTree;
    for (string item : magicItems) {
        cout << magicItemTree->insert(item, true) << endl;
    };
    cout << "\nIn-order Traversal (Left Root Right):" << endl;
    magicItemTree->inOrderTraversal();

    vector<string> itemsToFind = getMagicItems(ITEMS_2_FIND_PATH);
    int totalComps = 0;
    int comps;
    for (string item : itemsToFind) {
        string searchPath = magicItemTree->search(item, true);
        comps = checkComparisons(searchPath);
        totalComps += comps;
        cout << searchPath << " Comps: " << comps << endl;
    };
    cout << "\nAverage Comparisons Taken: " << totalComps / itemsToFind.size() << endl;

    vector<Graph> graphs = createGraphs(GRAPH_PATH);
    for (Graph g : graphs) {
        cout << "\nGraph Display: " << endl;
        g.displayGraph();
    }
    return 0;
};
