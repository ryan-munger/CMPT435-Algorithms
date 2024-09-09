#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

class Node {
    public:
        char value;
        Node* next;

        // Default constructor
        Node() {
            value = '\0';  // default to empty value
            next = nullptr;  
        }

        Node(char value) {
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
    
        void push(const char& value) {
            // create new node, make it the top & point to the old top
            Node* newItem = new Node();
            newItem->value = value;
            newItem->next = top;
            top = newItem;
            size++;
        }

        char pop() {
            if(isEmpty()) {
                return '\0';
            }

            char value = top->value;
            Node* newTop = top->next;
            delete top;
            top = newTop;
            size--;

            return value;
        }

        char peek() {
            return top->value;
        }    

        // const bc function does not modify member variables of class
        int getSize() const { 
            return size;
        }
};

class Queue {
    private:
        Node* head;
        Node* tail;
        int size;

    public:
        Queue() {
            head = nullptr;
            tail = nullptr;
            size = 0;
        }

        ~Queue() {
            while (!isEmpty()) {
                dequeue();
            }
        }

        bool isEmpty() {
            return (head == nullptr) && (tail == nullptr);
        }

        void enqueue(const char& value) {
            // make a new node
            Node* newItem = new Node();
            newItem->value= value;
            newItem->next = nullptr;

            // add the node to the back of the queue
            if (isEmpty()) {
                head = tail = newItem;
            } else {
                tail->next = newItem;
                tail = newItem;
            }
            size++;
        }

        char dequeue() {
            if(isEmpty()) {
                return '\0';
            }

            char value = head->value;
            Node* oldHead = head;
            head = oldHead->next;
            delete oldHead;
            size--;

            if (head == nullptr) {
                tail = nullptr;
            }
            return value;
        }

        char front() {
            return head->value;
        }

        char rear() {
            return tail->value;
        }

        int getSize() {
            return size;
        }
};

void knuthShuffle(vector<string>& arr) {
    int n = arr.size();
    for (int i = n-1; i > 0; i--) {
        int random = rand() % (i + 1); // inclusive to swap in place
        swap(arr[i], arr[random]);
    }
};

vector<string> getMagicItems(const string& filename) {
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

bool isPalindrome(const string& word) {
    Stack s; Queue q;
    for (char letter : word) {
        if (!isspace(letter)) {
            if (isalpha(letter)) {
                letter = tolower(letter);
            }
            s.push(letter);
            q.enqueue(letter);
        }
    }

    int size = s.getSize();
    for (int i = 0; i < size; i++) {
        if (!(s.pop() == q.dequeue())) {
            return false;
        }
    }
    return true;
};

string toLowerCase(const string& str) {
    string lowerStr = str;
    // Transforms a string to lowercase, citation: ChatGPT
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
};

// Referenced w3 schools
// Sorts ignoring case
int selectionSort(vector<string>& arr) {
    int n = arr.size();
    int comparisons = 0;
    
    for (int i = 0; i < n; i++){
        int minIdx = i;
        for (int j = i+1; j < n; j++) {
            comparisons++;
            if (toLowerCase(arr[j]) < toLowerCase(arr[minIdx])){
                minIdx = j;
            }
        }
        if (minIdx != i) {
            swap(arr[i], arr[minIdx]);
        }
    }
    return comparisons;
};

// referenced w3 schools
int insertionSort(vector<string>& arr) {
    int n = arr.size();
    int comparisons = 0;

    for (int i = 1; i < n; i++) {
        int insertIdx = i;
        string currentCheck = arr[i];
        for (int j = i-1; j >= 0; j--) {
            comparisons++;
            if (toLowerCase(arr[j]) > toLowerCase(currentCheck)) {
                arr[j+1] = arr[j];
                insertIdx = j;
            } else {
                break;
            }
        }
        arr[insertIdx] = currentCheck;
    }
    return comparisons;
};

void mergeSort() {

};

void quickSort() {

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

    // Stack stack;
    // stack.push("Hello");
    // stack.push("World");
    // stack.push("!");

    // int size = stack.getSize();
    // for (int i = 0; i < size; i++) {
    //     cout << stack.pop() << endl;
    // }

    // Queue q;
    // q.enqueue("Hello");
    // q.enqueue("World");
    // q.enqueue("!");
    
    // int size = q.getSize();
    // for (int i = 0; i < size; i++) {
    //     cout << q.dequeue() << endl;
    // }

    // vector<string> magicItems = getMagicItems("magicItems.txt");
    // for (auto & magicItem : magicItems) {
    //     if (isPalindrome(magicItem)) {
    //         cout << magicItem << endl;
    //     }
    // }

    // seed the random number generator using the current time 
    // citation: Generated by ChatGPT
    srand(static_cast<unsigned>(time(0)));
    
    vector<string> magicItems = getMagicItems("magicItems.txt");
    // for (const auto& ln : magicItems) {
    //     cout << ln << " ";
    // }
    // cout << "\n\n---------------------------------------\n\n" << endl;
    // knuthShuffle(magicItems);
    // for (string item : magicItems) {
    //     cout << item << " ";
    // }

    // int selectionComps = selectionSort(magicItems);
    // for (string item : magicItems) {
    //     cout << item << endl;
    // }
    // cout << "Comparisons: " << selectionComps << endl;
    knuthShuffle(magicItems);
    for (string item : magicItems) {
        cout << item << endl;
    }
    int insertComps = insertionSort(magicItems);
    for (string item : magicItems) {
        cout << item << endl;
    }
    cout << "Comparisons: " << insertComps << endl;

    return 0;
};