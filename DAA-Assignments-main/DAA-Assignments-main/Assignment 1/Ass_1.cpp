#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
using namespace std;
 
struct CustomerOrder {
    string orderID;
    string orderTimestamp;
};
 
// Merges two adjacent, already-sorted runs [start..mid) and [mid..end) into one sorted run
void mergeRuns(vector<CustomerOrder>& data, vector<CustomerOrder>& buffer, int start, int mid, int end) {
    int left = start, right = mid, pos = start;
 
    while (left < mid && right < end) {
        if (data[left].orderTimestamp <= data[right].orderTimestamp)
            buffer[pos++] = data[left++];
        else
            buffer[pos++] = data[right++];
    }
    while (left < mid)  buffer[pos++] = data[left++];
    while (right < end) buffer[pos++] = data[right++];
 
    for (int i = start; i < end; i++)
        data[i] = buffer[i];
}
 
// Bottom-up (iterative) merge sort: start with runs of size 1 and keep doubling the run size
void iterativeMergeSort(vector<CustomerOrder>& data) {
    int n = data.size();
    vector<CustomerOrder> buffer(n);
 
    for (int runSize = 1; runSize < n; runSize *= 2) {
        for (int start = 0; start < n; start += 2 * runSize) {
            int mid = min(start + runSize, n);
            int end = min(start + 2 * runSize, n);
            mergeRuns(data, buffer, start, mid, end);
        }
    }
}
 
int main() {
    ifstream infile("orders_dataset.csv");
    if (!infile) {
        cout << "Error: could not open dataset file!" << endl;
        return 1;
    }
    cout << "Dataset file opened successfully." << endl;
 
    string line;
    getline(infile, line); // skip header row
 
    vector<CustomerOrder> orders;
    while (getline(infile, line)) {
        stringstream rowStream(line);
        CustomerOrder ord;
        getline(rowStream, ord.orderID, ',');
        getline(rowStream, ord.orderTimestamp, ',');
        orders.push_back(ord);
    }
    infile.close();
 
    cout << "Total customer orders loaded: " << orders.size() << endl;
 
    iterativeMergeSort(orders);
 
    cout << "\nFirst 20 orders sorted by timestamp:\n\n";
    cout << "Order ID\tOrder Timestamp\n";
    cout << "-------------------------------------------------\n";
    for (int i = 0; i < 20 && i < (int)orders.size(); i++) {
        cout << orders[i].orderID << "\t" << orders[i].orderTimestamp << endl;
    }
 
    return 0;
}
