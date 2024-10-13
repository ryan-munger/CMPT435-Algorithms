#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
using namespace std;

const int HASH_TABLE_SIZE = 250;
const string MAGICITEMS_PATH = "../magicitems.txt";

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

string toLowerCase(const string& str) {
    string lowerStr = str;
    // Transforms a string to lowercase, citation: ChatGPT
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
};

// From assignment1
void merge(vector<string>& arr, int left, int mid, int right) {
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

// From assignment1
void mergeSort(vector<string>& arr, int left, int right) {
    // base case
    if (left >= right) 
        return;
         
    // left < right, array can be divided further
    // calculate mid of subarray/array to split array
    int mid = left + (right - left) / 2; 

    // recursively sort the halves, 
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    // merge sorted halves,
    merge(arr, left, mid, right);
};

// returns first index of the item if it is in the array
// we don't need a comparison counter as it is 1 more than the index the item is found at (since indexing starts at 0). 
template <typename T>
int sequentialSearch(const vector<T>& arr, const T& target) {
    for(size_t i = 0; i < arr.size(); ++i) {
        if(arr[i] == target){return static_cast<int>(i);}
    }
    return -1;
};

// returns first index of the item if it is in the array
int binarySearch(const std::vector<string>& arr, const string& target, int& comparisons) {
    int left = 0;
    int right = arr.size() - 1;

    // if left > right we did not find it
    while (left <= right) {
        int mid = left + (right - left) / 2;

        comparisons++;
        if (toLowerCase(arr[mid]) == toLowerCase(target)) {
            return mid; 
        } 
        else if (toLowerCase(arr[mid]) < toLowerCase(target)) {
            left = mid + 1; // search right half
        } 
        else {
            right = mid - 1; // search left half
        }
    }
    return -1; 
};

template <typename T>
struct Node {
    T value;
    Node<T>* next;
};

class HashTable {
    private:
        Node<string>* hashTable[HASH_TABLE_SIZE];
    
    public:
        // constructor
        HashTable() {
            // initialize the start of table chains to null 
            for (int i = 0; i < HASH_TABLE_SIZE; i++) {
                hashTable[i] = nullptr;
            }
        };

        // destructor 
        ~HashTable() {
            for (int i = 0; i < HASH_TABLE_SIZE; i++) {
                Node<string>* current = hashTable[i];
                while (current != nullptr) {
                    Node<string>* temp = current;
                    current = current->next;
                    delete temp;  // deallocate
                }
            }
        };

        void put(string str) {
            int hashCode = makeHash(str);
            Node<string>* newItem = new Node<string>;
            newItem->value = str;
            if (hashTable[hashCode] == nullptr) {
                newItem->next = nullptr;
                hashTable[hashCode] = newItem;
            } else {
                Node<string>* oldFront = hashTable[hashCode];
                newItem->next = oldFront;
                hashTable[hashCode] = newItem;
            };
        };

        int get(string str) {
            int hashCode = makeHash(str);
            if (hashTable[hashCode] == nullptr) {
                return -1;
            } else {
                int getComps = 1; // get is one compare plus chain
                Node<string>* currentNode = hashTable[hashCode];
                while (currentNode != nullptr) {
                    getComps++;
                    if (currentNode->value == str) { return getComps; }
                    currentNode = currentNode->next;
                }
                return -1;
            };
        };

        int makeHash(string value) {
            value = toLowerCase(value);
            int asciiTotal = 0;
            for(char letter : value) {
                asciiTotal += int(letter); // sorry Alan
                // cout << letter << "    .    " << int(letter) << endl;
            }
            int hashCode = (asciiTotal * 1031) % HASH_TABLE_SIZE; // using a prime, 1031 in honor of halloween!
            return hashCode;
        };

        void generateHistogram() {
            int pigeons;
            int pigeonHoles = HASH_TABLE_SIZE; // i just wanted to say it
           
            for (int i = 0; i < pigeonHoles; i++) {
                pigeons = 0;
                Node<string>* currentNode = hashTable[i];
                while (currentNode != nullptr) {
                    pigeons++;
                    currentNode = currentNode->next;
                }
                cout << setw(15) << "Pigeonhole " << i << ": " << string(pigeons, '*') << endl;
            }
        };
};

int main() {
    HashTable* hashy = new HashTable;
    hashy->put("Hello!");
    // it will add again, not worth traversing chain just to avoid duplicate
    // a classic space vs time 
    hashy->put("Hello!"); 
    hashy->put("test!");
    hashy->put("Something with a different hash");
    cout << "\"Hello!\" Found in table with " << hashy->get("Hello!") << " comparisons." << endl;
    cout << "\"Not in table...\" not found in table with " << hashy->get("Not in table...") << " copmarisons." << endl;
    // hashy->generateHistogram();

    vector<string> magicItems = getMagicItems(MAGICITEMS_PATH);
    mergeSort(magicItems, 0, magicItems.size() - 1);

    /* taken from stack overflow. Used ChatGPT to find out I needed c++ version 17.
    sample() takes range of elements and number to select, selects randomly without repetition.
    selected elements are added to the randomSample container with back_inserter(). 
    Rand generator (mt19937) ensures sample is random. After some ChatGPT and google, I found out that
    the reason the new selection is sorted is because sample() maintains the relative ordering of the elements it selected. */
    vector<string> randomSample;
    const int sample_size = 42;
    sample(magicItems.begin(), magicItems.end(), back_inserter(randomSample), sample_size, mt19937{random_device{}()});

    // load hash table
    HashTable* magicTable = new HashTable;
    for (string item : magicItems) {
        magicTable->put(item);
    }
    magicTable->generateHistogram();
    
    // since the random sample is in order (relative to the sorted array), as we progress through the sample, comparisons will always increase!
    int totalSeqComps = 0, totalBinComps = 0, totalHashGet = 0;
    int binaryComps = 0, hashGet = 0;
    int foundIdx;
    cout << "\nSequential/Linear Search:\n" << endl;
    for (string item : randomSample) {
        foundIdx = sequentialSearch(magicItems, item);
        if(foundIdx != -1){
            cout << "\"" << item << "\" found with Sequential Search at idx: " << foundIdx 
                << ". It took " << foundIdx + 1 << " Comparisons." << endl;
        } else {
            cout << "\"" << item << "\" was not found in magicItems. Comparisons: " << magicItems.size() << endl;
            foundIdx = magicItems.size() - 1; // since we are adding one later
        }
        totalSeqComps += foundIdx + 1;

        foundIdx = binarySearch(magicItems, item, binaryComps);
        if(foundIdx != -1){
            cout << "\"" << item << "\" found with Binary search at idx: " << foundIdx 
                << ". It took " << binaryComps << " Comparisons." << endl;
        } else {
            cout << "\"" << item << "\" was not found in magicItems. Comparisons: " << binaryComps << endl;
        }
        totalBinComps += binaryComps;
        binaryComps = 0;

        hashGet = magicTable->get(item);
        if(hashGet != -1){
            cout << "\"" << item << "\" found with Hash Table with " << hashGet 
                << " get+ comparisons." << endl;
        } else {
            cout << "\"" << item << "\" was not found in magicItems w/ hash table." << endl;
        }
        totalHashGet += hashGet;
    }
    cout << "\nSequential/Linear search took an average of " 
        << fixed << setprecision(2) // Set fixed-point notation and precision
        << static_cast<double>(totalSeqComps) / randomSample.size() // cast double so we don't lose our decimal accuracy
        << " comparisons to find each element." << endl;

    cout << "\nBinary search took an average of " 
        << fixed << setprecision(2) 
        << static_cast<double>(totalBinComps) / randomSample.size() 
        << " comparisons to find each element." << endl;

    cout << "\nHash Table took an average of " 
        << fixed << setprecision(2) 
        << static_cast<double>(totalHashGet) / randomSample.size() 
        << " comparisons to find each element." << endl;
    return 0;
};