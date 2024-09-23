#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
using namespace std;

template <typename T>
struct Node {
    T value;
    Node<T>* next;
};

template <typename T>
class Stack { 
    private:
        Node<T>* top;
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
    
        void push(const T value) {
            // create new node, make it the top & point to the old top
            Node<T>* newItem = new Node<T>;
            newItem->value = value;
            newItem->next = top;
            top = newItem;
            size++;
        }

        T pop() {
            if(isEmpty()) {
                throw std::runtime_error("Stack is empty, cannot pop.");
            }

            T value = top->value;
            Node<T>* newTop = top->next;
            delete top;
            top = newTop;
            size--;

            return value;
        }

        T peek() {
            return top->value;
        }    

        // const bc function does not modify member variables of class
        int getSize() const { 
            return size;
        }
};

template <typename T>
class Queue {
    private:
        Node<T>* head;
        Node<T>* tail;
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

        void enqueue(const T value) {
            // make a new node
            Node<T>* newItem = new Node<T>;
            newItem->value = value;
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

        T dequeue() {
            if(isEmpty()) {
                throw std::runtime_error("Queue is empty, cannot dequeue.");
            }

            T value = head->value;
            Node<T>* oldHead = head;
            head = oldHead->next;
            delete oldHead;
            size--;

            if (head == nullptr) {
                tail = nullptr;
            }
            return value;
        }

        T front() {
            return head->value;
        }

        T rear() {
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

bool isPalindrome(const string& word) {
    Stack<char> s; Queue<char> q;
    for (char letter: word) {
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
        if (s.pop() != q.dequeue()) {
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

// referenced geeksforgeeks.org
void merge(vector<string>& arr, int left, int mid, int right, int& comparisons) {
    int nLeft = mid - left + 1; // size of left array
    int nRight = right - mid; // size of right array

    // dec temp arrays
    vector<string> leftArr(nLeft), rightArr(nRight);

    // populate temp vectors 
    for (int i = 0; i < nLeft; i++)
        leftArr[i] = arr[left + i];
    for (int j = 0; j < nRight; j++)
        rightArr[j] = arr[mid + 1 + j];

    int i = 0, j = 0;
    int k = left;

    // merge temp vectors back into arr[left..right]
    while (i < nLeft && j < nRight) {
        comparisons++;
        if (toLowerCase(leftArr[i]) <= toLowerCase(rightArr[j])) {
            arr[k] = leftArr[i];
            i++;
        }
        else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    // copy the remaining elements of leftArr after merging 
    while (i < nLeft) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    // copy the remaining elements of right after merging
    while (j < nRight) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
};

// referenced geeksforgeeks.org
void mergeSort(vector<string>& arr, int left, int right, int& comparisons) {
    // base case
    if (left >= right) 
        return;
         
    // left < right, array can be divided further
    // calculate mid of subarray/array to split array
    int mid = left + (right - left) / 2; 

    // recursively sort the halves, update comparisons
    mergeSort(arr, left, mid, comparisons);
    mergeSort(arr, mid + 1, right, comparisons);

    // merge sorted halves, update comparisons
    merge(arr, left, mid, right, comparisons);
};

// referenced geeksforgeeks.org
int partition(vector<string>& arr, int low, int high, int& comparisons) {
    // choose pivot element
    string pivot = toLowerCase(arr[high]);
    // Index of smaller element + right position of pivot found so far
    int i = low - 1;

    // rearranges array so that elements less than or equal to the pivot are to the left of it
    // greater than are to the right
    for (int j = low; j <= high - 1; j++) {
        comparisons++;
        if (toLowerCase(arr[j]) < pivot) { 
            i++;
            swap(arr[i], arr[j]);
        }
    }
    
    // move pivot after smaller elements 
    swap(arr[i + 1], arr[high]);  
    return i + 1;
};

// referenced geeksforgeeks.org
void quickSort(vector<string>& arr, int low, int high, int& comparisons) {
    // if low < high then we are not done
    if (low < high) {
        int pIdx = partition(arr, low, high, comparisons); // Partitioning index

        quickSort(arr, low, pIdx - 1, comparisons);  // recursively sort the elements before partition
        quickSort(arr, pIdx + 1, high, comparisons); // recursively sort the elements after partition
    }
};

int main() {
    // Testing Node class
    Node<string>* firstNode = new Node<string>;
    firstNode->value = "Magic Item1";
    Node<string>* secondNode = new Node<string>;
    secondNode->value = "Nogard Dragon"; 
    firstNode->next = secondNode;
    cout << "First Node Value: " << firstNode->value << endl;
    cout << "Second Node Value: " << secondNode->value << endl;
    if (firstNode->next == secondNode) {
        cout << "First node points to the second node." << endl;
    } else {
        cout << "First node does not point to the second node." << endl;
    }
    // garbage collect them
    delete firstNode;
    delete secondNode;

    Stack<string> stack;
    stack.push("Hello");
    stack.push("World");
    stack.push("!");
    int size = stack.getSize();
    for (int i = 0; i < size; i++) {
        cout << stack.pop() << endl;
    }

    Queue<string> q;
    q.enqueue("Hello");
    q.enqueue("World");
    q.enqueue("!");
    size = q.getSize();
    for (int i = 0; i < size; i++) {
        cout << q.dequeue() << endl;
    }

    vector<string> magicItems = getMagicItems("../magicItems.txt");
    for (auto & magicItem : magicItems) {
        if (isPalindrome(magicItem)) {
            cout << magicItem << " Is a palindrome!" << endl;
        }
    }

    // seed the random number generator using the current time 
    // citation: Generated by ChatGPT
    srand(static_cast<unsigned>(time(0)));

    // no need to sort before selection as it always checks every element
    int selectionComps = selectionSort(magicItems);
    // for (string item : magicItems) {
    //     cout << item << endl;
    // }
    cout << "Selection Sort Comparisons: " << selectionComps << endl;
    knuthShuffle(magicItems);

    int insertComps = insertionSort(magicItems);
    // for (string item : magicItems) {
    //     cout << item << endl;
    // }
    cout << "Insertion Sort Comparisons: " << insertComps << endl;
    knuthShuffle(magicItems);

    int mergeComps = 0;
    mergeSort(magicItems, 0, magicItems.size() - 1, mergeComps);
    // for (string item : magicItems) {
    //     cout << item << endl;
    // }
    cout << "Merge Sort Comparisons: " << mergeComps << endl;
    knuthShuffle(magicItems);

    int quickComps = 0;
    quickSort(magicItems, 0, magicItems.size() - 1, quickComps);
    // for (string item : magicItems) {
    //     cout << item << endl;
    // }
    cout << "Quick Sort Comparisons: " << quickComps << endl;

    return 0;
};