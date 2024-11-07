#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <regex>
#include <iomanip>
#include <queue>
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

struct linkedVertex {
    string id;
    bool processed;
    vector<linkedVertex*> neighbors;
};

// debug helper function
// void printLinkedVertex(linkedVertex* v) {
//     cout << v->id << ": ";
//     for (auto& n : v->neighbors) {
//         cout << n->id << " ";
//     }
//     cout << endl;
// };

class Graph {
    private:
        string graphID;
        // Each map vertex stores a list of neighbors
        map<string, vector<string>> adjacencyRep; 

        // map whatever user named the vertex to a matrix index
        map<string, int> vertexToMatrixID;
        map<int, string> matrixToVertexID; // so we can go backwards
        vector<vector<int>> matrixRep;

        // keep track of our vertex objects
        // use a map so we can actually look them up without checking them all
        map<string, linkedVertex> linkedObjs;

        void resetProcessedFlags() {
            for (auto& pair : linkedObjs) {
                pair.second.processed = false;
            }
        };

    public:
        Graph(string id) {
            this->graphID = id;
        };

        void addVertex(string vertex) {
            // add the vertex to the matrix using next available index
            if (vertexToMatrixID.count(vertex) == 0) { // make sure we haven't already added it!
                vertexToMatrixID[vertex] = matrixRep.size();
                matrixToVertexID[matrixRep.size()] = vertex;
                // default to no neighbors           
                if(this->isEmpty()) {
                    matrixRep.push_back(vector<int>(1, 0)); 
                } else {
                    this->matrixRep.push_back(vector<int>(matrixRep[0].size(), 0)); // new row
                    // update relations for new row (no relations unless edge added) - new col
                    for (vector<int>& vertex : this->matrixRep) {
                        vertex.push_back(0);
                    }   
                }
            }

            this->adjacencyRep[vertex]; // start an adj for the vertex

            linkedObjs[vertex] = linkedVertex{vertex}; // Store by value
        };

        void addEdge(string vertex1, string vertex2) {
            // must do both as it is undirected
            this->adjacencyRep[vertex1].push_back(vertex2);
            this->adjacencyRep[vertex2].push_back(vertex1);

            this->matrixRep[vertexToMatrixID[vertex1]][vertexToMatrixID[vertex2]] = 1;
            this->matrixRep[vertexToMatrixID[vertex2]][vertexToMatrixID[vertex1]] = 1;

            this->linkedObjs[vertex1].neighbors.push_back(&linkedObjs[vertex2]);
            this->linkedObjs[vertex2].neighbors.push_back(&linkedObjs[vertex1]);
        };
        
        void displayAdj() {
            cout << "\nAdjacency List:" << endl;
            // Generated by ChatGPT as I was unfamiliar with printing maps
            for (const auto& pair : this->adjacencyRep) {
                cout << setw(3) << pair.first << ": ";
                for (string neighbor : pair.second) {
                    cout << neighbor << " ";
                }
                cout << endl;
            }
        };

        void displayMatrix() {
            cout << "\nMatrix:" << endl;
            for(size_t i = 0; i < this->matrixRep.size(); ++i) {
                cout << setw(5) << matrixToVertexID[i] << ": ";
                for(size_t j = 0; j < this->matrixRep[i].size(); ++j) {
                    cout << this->matrixRep[i][j] << " ";
                }
                cout << endl;
            }
        };

        void depthFirstTraversal(linkedVertex* fromVertex) {
            if (fromVertex == nullptr) return;

            if (!fromVertex->processed) {
                cout << fromVertex->id << "->";
                fromVertex->processed = true;
            }
            for (linkedVertex* v : fromVertex->neighbors){
                if (v != nullptr && !v->processed) {
                    depthFirstTraversal(v);
                }
            }
        };

        void breadthFirstTraversal(linkedVertex* fromVertex) {
            cout << "\nBreadth First Traversal: ";
            linkedVertex* cv;
            queue<linkedVertex*> q;
            q.push(fromVertex);
            fromVertex->processed = true;
            while (!q.empty()) {
                cv = q.front();
                q.pop();
                cout << cv->id << "->";
                for (linkedVertex* v : cv->neighbors) {
                    if (!v->processed) {
                        q.push(v);
                        v->processed = true;
                    }
                }
            }
            cout << "End" << endl;
        };

        void displayGraph() {
            if (this->isEmpty()) {
                cout << "Graph " << this->graphID << " is empty silly!" << endl;
                return;
            }
            
            cout << "\n\nGraph " << this->graphID << " Display:" << endl;
            this->displayAdj();
            this->displayMatrix();

            // just start at the smallest vertex ID by default
            linkedVertex* defaultStart = &this->linkedObjs.begin()->second;
            this->resetProcessedFlags(); // remove any flags from prior traversal
            cout << "\nDepth First Traversal: ";
            this->depthFirstTraversal(defaultStart); 
            cout << "End" << endl;

            this->resetProcessedFlags(); 
            this->breadthFirstTraversal(defaultStart);
        };

        bool isEmpty() {
            // if adj list has nothing graph empty
            return this->adjacencyRep.empty();
        };
};

void createGraphs(const string& filename) {
    int graphCount = 1;
    Graph currentGraph(to_string(graphCount));
    // I am taking formal lang so regex it'll be!
    regex newGraphRe(R"(new graph)");
    regex addVertexRe(R"(add\s*vertex\s*(\S+))");
    regex addEdgeRe(R"(add\s*edge\s*(\S+)\s*-\s*(\S+))");

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
                currentGraph.displayGraph();
                graphCount++;
                currentGraph = Graph(to_string(graphCount)); // start a new graph
            }
        } else {
            smatch match; // captures subexpressions/groups

            if (regex_match(instruction, match, addVertexRe)) { // case 2: new vertex
                string newVertex = match[1].str();
                currentGraph.addVertex(newVertex);
            } else if (regex_match(instruction, match, addEdgeRe)) { // case 3: new edge
                string v1 = match[1].str();
                string v2 = match[2].str();
                currentGraph.addEdge(v1, v2);
            };
        };
    };
    file.close();
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

    createGraphs(GRAPH_PATH); 

    return 0;
};
