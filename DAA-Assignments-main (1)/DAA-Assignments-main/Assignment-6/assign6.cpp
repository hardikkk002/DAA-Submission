#include <bits/stdc++.h>
using namespace std;

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");
    if (!in) { cerr << "Cannot open input.txt\n"; return 1; }
    int n, capacity; in >> n >> capacity;
    vector<int> weight(n + 1), value(n + 1);
    for (int i = 1; i <= n; ++i) in >> weight[i] >> value[i];
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
    for (int i = 1; i <= n; ++i) {
        for (int w = 0; w <= capacity; ++w) {
            dp[i][w] = dp[i - 1][w];
            if (weight[i] <= w)
                dp[i][w] = max(dp[i][w], value[i] + dp[i - 1][w - weight[i]]);
        }
    }
    out << "Maximum utility: " << dp[n][capacity] << "\n";
    out << "Selected items (item: weight, utility):\n";
    int w = capacity;
    vector<int> selected;
    for (int i = n; i >= 1; --i) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected.push_back(i);
            w -= weight[i];
        }
    }
    reverse(selected.begin(), selected.end());
    int totalWeight = 0;
    if (selected.empty()) out << "None\n";
    for (int i : selected) {
        out << "Item " << i << ": " << weight[i] << " kg, utility " << value[i] << "\n";
        totalWeight += weight[i];
    }
    out << "Total weight: " << totalWeight << " kg / " << capacity << " kg\n";
    return 0;
}
