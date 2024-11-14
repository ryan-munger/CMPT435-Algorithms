#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <tuple>
using namespace std;

const string GRAPH_PATH = "graphs2.txt";
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

int main() {
    // read in the instructions and create the associated graphs
    createGraphs(GRAPH_PATH); 

    return 0;
};