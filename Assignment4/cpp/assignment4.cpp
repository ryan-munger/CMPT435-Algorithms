#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <tuple>
#include <iomanip>
#include <stack>
using namespace std;

const string GRAPH_PATH = "graphs2.txt";
const string SPICE_PATH = "spice.txt";
const int vertexTupleIdx = 0;
const int weightTupleIdx = 1;
const int functionalInfinity = 1e9;

struct linkedVertex {
    string id;
    bool processed;
    int distance;
    linkedVertex* predecessor;
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

        void initSingleSource(linkedVertex* s) {
            // set all vertices to distance infinite (large but not max)
            // no predecessors yet
            for (auto& pair : this->linkedObjs) {
                pair.second.distance = functionalInfinity;
                pair.second.predecessor = nullptr;
            }
            // set single source
            s->distance = 0;
        };

        void relax(linkedVertex* source, linkedVertex* destination, int weight) {
            if (destination->distance > (source->distance + weight)) {
                destination->distance = (source->distance + weight);
                destination->predecessor = source;
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
            // print graph objects to ensure validity
            for (const auto& pair : this->linkedObjs) {
                printLinkedVertex(pair.second);
            }

            this->SSSP();
        };

        bool bellmanFord(linkedVertex* startVertex) {
            this->initSingleSource(startVertex);

            // Relax all edges |V| - 1 times
            for (size_t i = 1; i < this->linkedObjs.size(); ++i) {
                for (auto& pair : this->linkedObjs) {
                    linkedVertex* current = &pair.second;
                    for (auto& edge : current->neighbors) {
                        linkedVertex* destination = get<vertexTupleIdx>(edge);
                        int weight = get<weightTupleIdx>(edge);
                        relax(current, destination, weight);
                    }
                }
            }

            // Detect negative weight cycles
            for (auto& pair : this->linkedObjs) {
                linkedVertex* current = &pair.second;
                for (auto& edge : current->neighbors) {
                    linkedVertex* destination = get<vertexTupleIdx>(edge);
                    int weight = get<weightTupleIdx>(edge);

                    if (destination->distance > (current->distance + weight)) {
                        return false; // Negative weight cycle found
                    }
                }
            }

            return true; // No negative weight cycles
        };

        string getShortestPath(linkedVertex* destination) {
            stack<string> pathStack;
            linkedVertex* predecessor = destination;
            while (predecessor != nullptr) {
                pathStack.push(predecessor->id);
                predecessor = predecessor->predecessor;
            }

            ostringstream pathstr;
            while (!pathStack.empty()) {
                // Check if something is already in the stream for ->
                if (pathstr.tellp() > 0) {
                    pathstr << "->";
                }
                pathstr << pathStack.top();
                pathStack.pop();
            }
            return pathstr.str();
        };

        void SSSP() {
            linkedVertex* startVertex = &this->linkedObjs.begin()->second;
            // set distances, predecessors, etc
            bellmanFord(startVertex);

            cout << "SSSP: " << endl;
            for (auto& pair : this->linkedObjs) {
                linkedVertex* current = &pair.second;
                cout << startVertex->id << "->" << current->id << " cost is " << setw(2) << 
                    current->distance << "; shortest path is " << getShortestPath(current) << endl;
            }
        };
};

void createGraphs(const string& filename) {
    int graphCount = 1;
    cout << "Graph #" << graphCount << ":\n" << endl;
    Graph currentGraph(to_string(graphCount));
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
                currentGraph.displayGraph();
                graphCount++;
                cout << "\n\nGraph #" << graphCount << ": \n" << endl;
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

// Insertion sort to get descending order based on unit price
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

void maximizeTake(float knapsack, vector<Spice> spices) {
    float knapValue = 0;
    if (knapsack == 0) {
        cout << "Knapsack of Capacity " << fixed << setprecision(2) << knapsack << " is worth " << 
            fixed << setprecision(2) << knapValue << " quatloos and contains no scoops." << endl; 
        return;
    }
    ostringstream scoops;
    float capacityLeft = knapsack;
    for (Spice spice : spices) {
        if (capacityLeft == 0) {
            break; // no more spice!!
        } else if(capacityLeft >= spice.quantity) { // take all the spice
            capacityLeft -= spice.quantity;
            knapValue += spice.total_price;
            scoops << fixed << setprecision(2) << spice.quantity << " scoops of " << spice.color << ", ";
        } else if (capacityLeft < spice.quantity) { // take what we can fit
            knapValue += capacityLeft * spice.unit_price;
            scoops << fixed << setprecision(2) << capacityLeft << " scoops of " << spice.color << ", ";
            capacityLeft = 0;
        } 
    }
    string scoopString = scoops.str();
    // replace last comma with period
    scoopString.pop_back(); 
    scoopString.back() = '.';

    // report the value of our knapsack and scoops taken
    cout << "Knapsack of Capacity " << fixed << setprecision(2) << knapsack << " is worth " << 
        fixed << setprecision(2) << knapValue << " quatloos and contains " << scoopString << endl; 
}; 

void spiceHeist(const string& filename) {
    cout << "\n\nLoading in Spices and Knapsacks!" << endl;
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
            cout << "New Knapsack: " << newKnapsackCapacity << endl;
        };
    };
    file.close();
    // sort our spices based on unit price
    spiceSort(spiceInventory);
    // maximize take for each knapsack!
    cout << "\nMaximizing Take:" << endl;
    for (float knapsack : knapsacks) {
        maximizeTake(knapsack, spiceInventory);
    }
};

int main() {
    createGraphs(GRAPH_PATH); 

    spiceHeist(SPICE_PATH);

    return 0;
};
