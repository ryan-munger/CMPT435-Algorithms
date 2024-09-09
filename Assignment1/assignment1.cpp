#include <iostream>
#include <string>
#include <fstream>
#include <vector>
using namespace std;

class Node {
    public:
        string value;
        Node* next;

        // Default constructor
        Node() {
            value = "";  // default to empty value
            next = nullptr;  
        }

        Node(string value) {
            value = value;
            next = nullptr; // next is not ready when making current
        }
};

class Stack { 
    private:
        Node* top;
        int size;

    public: 
        // constructor
        Stack() {
            top = nullptr; 
            size = 0;
        }
        // Destructor to free memory from nodes within stack if stack is deleted
        ~Stack() {
            while (!isEmpty()) {
                pop();
            }
        }

        bool isEmpty() {
            return top == nullptr;
        }
    
        void push(const string& value) {
            // create new node, make it the top & point to the old top
            Node* newItem = new Node();
            newItem->value = value;
            newItem->next = top;
            top = newItem;
            size++;
        }

        string pop() {
            if(!isEmpty()) {
                string value = top->value;
                Node* newTop = top->next;
                delete top;
                top = newTop;
                size--;

                return value;
            }
            return nullptr;
        }

        string peek() {
            return top->value;
        }    

        // const bc function does not modify member variables of class
        int getSize() const { 
            return size;
        }
};

// class Queue {
//     private:


//     public:

// }

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
};

int main() {
    // // Testing Node class
    // Node* firstNode = new Node("Magic Book");
    // Node* secondNode = new Node("Dior Droid");

    // firstNode->next = secondNode;

    // cout << "First Node Value" << firstNode->value << endl;
    // cout << "Second Node Value" << secondNode->value << endl;

    // if (firstNode->next == secondNode) {
    //     cout << "First node points to the second node." << endl;
    // }

    // // garbage collect them
    // delete firstNode;
    // delete secondNode;

    // vector<string> magicItems = getMagicItems("magicItems.txt");
    // for (const auto& ln : magicItems) {
    //     std::cout << ln << std::endl;
    // }

    Stack stack;
    stack.push("Hello");
    stack.push("World");
    stack.push("!");

    int size = stack.getSize();
    for (int i = 0; i < size; i++) {
        cout << stack.pop() << endl;
    }

    return 0;
};