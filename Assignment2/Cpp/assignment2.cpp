#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
using namespace std;

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

int main() {
    vector<string> magicItems = getMagicItems("../magicItems.txt");
    mergeSort(magicItems, 0, magicItems.size() - 1);

    /* taken from stack overflow. Used ChatGPT to find out I needed c++ version 17.
    sample() takes range of elements and number to select, selects randomly without repetition.
    selected elements are added to the random_sample container with back_inserter(). 
    Rand generator (mt19937) ensures sample is random. After some ChatGPT and google, I found out that
    the reason the new selection is sorted is because sample() maintains the relative ordering of the elements it selected. */
    vector<string> random_sample;
    const int sample_size = 42;
    sample(magicItems.begin(), magicItems.end(), back_inserter(random_sample), sample_size, mt19937{random_device{}()});
    for (string item : random_sample) {
        cout << item << endl;
    }

    return 0;
};