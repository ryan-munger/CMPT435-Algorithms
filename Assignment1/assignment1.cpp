#include <iostream>
#include <string>
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

    return 0;
};