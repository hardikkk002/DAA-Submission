
#include <bits/stdc++.h>
using namespace std;

struct Transaction {
    int customer_id;
    string orderId;
    string ProductInfo;
    double amount;
    string purDate;
    string location;
};

class Solution {
public:

    void merge(vector<Transaction>& arr, int low, int mid, int high) {

        vector<Transaction> temp;
        int left = low, right = mid + 1;

        while (left <= mid && right <= high) {
            if (arr[left].customer_id <= arr[right].customer_id)
                temp.push_back(arr[left++]);
            else
                temp.push_back(arr[right++]);
        }

        while (left <= mid)
            temp.push_back(arr[left++]);

        while (right <= high)
            temp.push_back(arr[right++]);

        for (int i = low; i <= high; i++)
            arr[i] = temp[i - low];
    }

    void mergeSort(vector<Transaction>& arr, int low, int high) {
        if (low >= high)
            return;

        int mid = (low + high) / 2;

        mergeSort(arr, low, mid);
        mergeSort(arr, mid + 1, high);
        merge(arr, low, mid, high);
    }
};

int main() {
    vector<Transaction> arr;

    ifstream file("D:\\COLLEGE\\DAA Assignments\\customer_data.csv");

    if (!file) {
        cout << "Error Opening File";
        return 1;
    }

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string item;
        Transaction t;

        if (getline(ss, item, ','))
            t.customer_id = stoi(item);

        if (getline(ss, t.orderId, ',')) {}

        if (getline(ss, t.ProductInfo, ',')) {}

        if (getline(ss, item, ','))
            t.amount = stod(item);

        if (getline(ss, t.purDate, ',')) {}

        if (getline(ss, t.location, ',')) {}

        arr.push_back(t);
    }

    file.close();

    if (arr.empty()) {
        cout << "No data was Parsed... Check your csv File" << endl;
        return 1;
    }

    cout << "Loaded.." << arr.size() << " Transactions. Sorting..." << endl;

    Solution sol;
    sol.mergeSort(arr, 0, arr.size() - 1);

    cout << "Sorting Complete.." << endl;

    int dispLimit = min(10, (int)arr.size());

    for (int i = 0; i < dispLimit; i++) {
        cout << "Customer Id : " << arr[i].customer_id
             << "| Order Id : " << arr[i].orderId
             << "| Product Info : " << arr[i].ProductInfo
             << "| Amount : " << arr[i].amount << endl;
    }

    ofstream outFile("sorted_customer_data.csv");

    if (!outFile) {
        cout << "Error opening output file!" << endl;
        return 1;
    }

    outFile << "customer_id,orderId,ProductInfo,amount,purDate,location\n";

    for (const auto& t : arr) {
        outFile << t.customer_id << ","
                << t.orderId << ","
                << t.ProductInfo << ","
                << t.amount << ","
                << t.purDate << ","
                << t.location << "\n";
    }

    outFile.close();

    cout << "Sorted data successfully written to sorted_customer_data.csv" << endl;

    return 0;
}
