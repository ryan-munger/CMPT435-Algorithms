#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <tuple>
using namespace std;

const string GRAPH_PATH = "graphs2.txt";
const string SPICE_PATH = "spice.txt";
const int vertexTupleIdx = 0;
const int weightTupleIdx = 1;

struct linkedVertex {
    string id;
    bool processed;
    vector<tuple<linkedVertex*, int>> neighbors; // no limit to neighbors!
};

// debug helper function
void printLinkedVertex(linkedVertex v) {
    cout << "LinkedVertex " << v.id << "; Neighbors: " << endl;
    if (v.neighbors.empty()) {
        cout << "\tNo Neighbors" << endl;
    } else {
        for (const auto& tuple : v.neighbors) {
            cout << "\tVertex: " << get<vertexTupleIdx>(tuple)->id << " Weight: " << get<weightTupleIdx>(tuple) << endl;
        }
    }
};

class Graph {
    private:
        string graphID;
        map<string, linkedVertex> linkedObjs;

        // if we want to traverse again
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
            linkedObjs[vertex] = linkedVertex{vertex}; // store by value
        };

        void addEdge(string vertex1, string vertex2, int weight) {
            this->linkedObjs[vertex1].neighbors.push_back(make_tuple(&linkedObjs[vertex2], weight));
        };

        bool isEmpty() {
            return this->linkedObjs.empty();
        };

        void displayGraph() {
            for (const auto& pair : this->linkedObjs) {
                printLinkedVertex(pair.second);
            }
        };

        void SSSP() {
            cout << "I don't know SSSP yet!" << endl;
        };
};

void createGraphs(const string& filename) {
    int graphCount = 1;
    cout << "\n\nGRAPH" << graphCount << "\n" << endl;
    Graph currentGraph(to_string(graphCount));
    // I am taking formal lang so regex it'll be!
    // this will take care of any small whitespace errors as well
    regex newGraphRe(R"(new graph)");
    regex addVertexRe(R"(add\s*vertex\s*(\S+))");
    regex addEdgeRe(R"(add\s*edge\s*(\S+)\s*-\s*(\S+)\s+(-?\d+))");

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
            // check to see if the current graph has anything in it
            // if not, no need to start a new one
            if (!currentGraph.isEmpty()) {
                currentGraph.SSSP();
                currentGraph.displayGraph();
                graphCount++;
                cout << "\n\nGRAPH" << graphCount << "\n" << endl;
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
                int weight = stoi(match[3].str());
                currentGraph.addEdge(v1, v2, weight);
            };
        };
    };
    file.close();
    currentGraph.SSSP(); // don't forget the last one!!
    currentGraph.displayGraph();
};

struct Spice {
    string color;
    float total_price;
    float quantity;
    float unit_price;
};

void printSpice(Spice s) {
    cout << "Spice: " << endl;
    cout << "\tColor: " << s.color << endl;
    cout << "\tTotal Price: " << s.total_price << endl;
    cout << "\tQuantity: " << s.quantity << endl;
    cout << "\tUnit Price: " << s.unit_price << endl;
};

void spiceSort(vector<Spice>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; i++) {
        int insertIdx = i;
        Spice currentCheck = arr[i];
        for (int j = i-1; j >= 0; j--) {
            if (arr[j].unit_price < currentCheck.unit_price) {
                arr[j+1] = arr[j];
                insertIdx = j;
            } else {
                break;
            }
        }
        arr[insertIdx] = currentCheck;
    }
};

void maximizeTake(float knapsack) {

};

void spiceHeist(const string& filename) {
    regex spiceRe(R"(\s*spice\s*name\s*=\s*(\S*)\s*;\s*total_price\s*=\s*(\d*.?\d*)\s*;\s*qty\s*=\s*(\d*.?\d*)\s*;)");
    regex knapsackRe(R"(knapsack\s*capacity\s*=\s*(\d*.?\d*)\s*;)");

    // store spices and knapsacks
    vector<Spice> spiceInventory;
    vector<float> knapsacks;

    ifstream file(filename); // input file stream
    if (!file) {
        cerr << "File opening failed." << endl;
    }
    string instruction;

    while (getline(file, instruction)) {
        smatch match; // captures subexpressions/groups
        // case 1: adding a spice
        if (regex_match(instruction, match, spiceRe)) {
            string color = match[1].str();
            float total_price = stof(match[2].str());
            float quantity = stof(match[3].str());
            float unit_price = total_price / quantity;
            Spice newSpice = Spice{color, total_price, quantity, unit_price};
            printSpice(newSpice);
            spiceInventory.push_back(newSpice);
        } else if (regex_match(instruction, match, knapsackRe)) { // case 2: knapsack
            float newKnapsackCapacity = stof(match[1].str());
            knapsacks.push_back(newKnapsackCapacity);
            cout << "Sack: " << newKnapsackCapacity << endl;
        };
    };
    file.close();
    // sort our spices based on unit price
    spiceSort(spiceInventory);
    // maximize take for each knapsack!
    for (float knapsack : knapsacks) {
        maximizeTake(knapsack);
    }
};

int main() {
    createGraphs(GRAPH_PATH); 

    spiceHeist(SPICE_PATH);

    return 0;
};
