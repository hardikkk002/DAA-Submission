#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <iomanip>
using namespace std;
 
struct ReliefItem {
    string label;
    double mass;       // kg
    double utility;    // relief value
    bool splittable;   // can be divided into fractions
 
    double ratio() const { return utility / mass; }
};
 
// Max-heap comparator: the item with the higher utility-per-kg ratio
// must come out of the priority_queue first.
struct RatioCompare {
    bool operator()(const ReliefItem& a, const ReliefItem& b) const {
        return a.ratio() < b.ratio();
    }
};
 
double distributeSupplies(vector<ReliefItem> stock, double boatCapacity, double& loadCarried) {
    priority_queue<ReliefItem, vector<ReliefItem>, RatioCompare> heap(stock.begin(), stock.end());
 
    double totalUtility = 0.0;
    loadCarried = 0.0;
 
    cout << "\nLoading order (extracted from max-heap by utility-per-kg):\n";
    cout << left << setw(16) << "Item" << setw(10) << "Weight" << setw(10) << "Utility"
         << setw(10) << "Ratio" << setw(12) << "Type" << "\n";
    cout << "---------------------------------------------------------------\n";
 
    while (!heap.empty() && boatCapacity > 0) {
        ReliefItem top = heap.top();
        heap.pop();
 
        cout << left << setw(16) << top.label << setw(10) << top.mass << setw(10) << top.utility
             << setw(10) << fixed << setprecision(2) << top.ratio()
             << setw(12) << (top.splittable ? "Divisible" : "Indivisible") << "\n";
 
        if (top.mass <= boatCapacity) {
            totalUtility += top.utility;
            boatCapacity -= top.mass;
            loadCarried += top.mass;
            cout << "   -> Loaded fully: " << top.mass << " kg, Utility gained = "
                 << top.utility << ", Remaining capacity = " << boatCapacity << " kg\n";
        } else if (top.splittable) {
            double fraction = boatCapacity / top.mass;
            double gained = fraction * top.utility;
            totalUtility += gained;
            loadCarried += boatCapacity;
            cout << "   -> Loaded partially: " << boatCapacity << " kg ("
                 << fixed << setprecision(1) << fraction * 100 << "%), Utility gained = "
                 << setprecision(2) << gained << "\n";
            boatCapacity = 0;
        } else {
            cout << "   -> Skipped: indivisible and does not fit in remaining capacity\n";
        }
    }
 
    return totalUtility;
}
 
int main() {
    int n;
    cout << "Enter number of relief supply types: ";
    cin >> n;
 
    vector<ReliefItem> stock;
    for (int i = 0; i < n; i++) {
        ReliefItem item;
        cout << "\nSupply #" << (i + 1) << ":\n";
        cin.ignore();
        cout << "Label: ";
        getline(cin, item.label);
        cout << "Weight (kg): ";
        cin >> item.mass;
        cout << "Utility value: ";
        cin >> item.utility;
        int splitInt;
        cout << "Divisible? (1 = Yes, 0 = No): ";
        cin >> splitInt;
        item.splittable = (splitInt == 1);
        stock.push_back(item);
    }
 
    double boatCapacity;
    cout << "\nEnter boat capacity (kg): ";
    cin >> boatCapacity;
 
    double loadCarried;
    double maxUtility = distributeSupplies(stock, boatCapacity, loadCarried);
 
    cout << "\n===== Relief Summary =====\n";
    cout << "Total weight carried: " << fixed << setprecision(2) << loadCarried << " kg\n";
    cout << "Total utility delivered: " << fixed << setprecision(2) << maxUtility << " units\n";
 
    return 0;
}
 
