#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class Node {
    public:
        string value;
        Node* next;

    Node(string value) {
        value = value;
        next = nullptr; // next is not ready when making current
    }
};

vector<string> getMagicItems(const string&filename) {
    vector<string> magicItems; 
    ifstream file("magicItems.txt"); //input file stream
    if (!file) {
        cerr << "File opening failed." << endl;
    }
    string line;
    while (getline(file, line)) {
        magicItems.push_back(line); // add line to vector
    }
    file.close();
    return magicItems;
}

int main() {
    // Testing Node class
    Node* firstNode = new Node("Magic Book");
    Node* secondNode = new Node("Dior Droid");

    firstNode->next = secondNode;

    cout << "First Node Value" << firstNode->value << endl;
    cout << "Second Node Value" << secondNode->value << endl;

    if (firstNode->next == secondNode) {
        cout << "First node points to the second node." << endl;
    }

    // garbage collect them
    delete firstNode;
    delete secondNode;

    vector<string> magicItems = getMagicItems("magicItems.txt");
    for (const auto& ln : magicItems) {
        std::cout << ln << std::endl;
    }
    
    return 0;
};