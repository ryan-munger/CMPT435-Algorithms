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

        bool get(string str) {
            int hashCode = makeHash(str);
            if (hashTable[hashCode] == nullptr) {
                return false;
            } else {
                Node<string>* currentNode = hashTable[hashCode];
                while (currentNode != nullptr) {
                    if (currentNode->value == str) { return true; }
                    currentNode = currentNode->next;
                }
                return false;
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
};

int main() {
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

    // since the random sample is in order (relative to the sorted array), as we progress through the sample, comparisons will always increase!
    int totalComparisons = 0;
    int foundIdx;
    cout << "\nSequential/Linear Search:\n" << endl;
    for (string item : randomSample) {
        foundIdx = sequentialSearch(magicItems, item);
        if(foundIdx != -1){
            cout << "\"" << item << "\" was found in magicItems at index: " << foundIdx 
                << ". It took " << foundIdx + 1 << " Comparisons." << endl;
        } else {
            cout << "\"" << item << "\" was not found in magicItems. Comparisons: " << magicItems.size() << endl;
            foundIdx = magicItems.size() - 1; // since we are adding one later
        }
        totalComparisons += foundIdx + 1;
    }
    cout << "\nSequential/Linear search took an average of " 
        << fixed << setprecision(2) // Set fixed-point notation and precision
        << static_cast<double>(totalComparisons) / randomSample.size() // cast double so we don't lose our decimal accuracy
        << " comparisons to find each element." << endl;

    int comparisons = 0;
    totalComparisons = 0;
    cout << "\n\nBinary Search:\n" << endl;
    for (string item : randomSample) {
        foundIdx = binarySearch(magicItems, item, comparisons);
        if(foundIdx != -1){
            cout << "\"" << item << "\" was found in magicItems at index: " << foundIdx 
                << ". It took " << comparisons << " Comparisons." << endl;
        } else {
            cout << "\"" << item << "\" was not found in magicItems. Comparisons: " << comparisons << endl;
        }
        totalComparisons += comparisons;
        comparisons = 0;
    }
    cout << "\nBinary search took an average of " 
        << fixed << setprecision(2) 
        << static_cast<double>(totalComparisons) / randomSample.size() 
        << " comparisons to find each element." << endl;

    return 0;
};