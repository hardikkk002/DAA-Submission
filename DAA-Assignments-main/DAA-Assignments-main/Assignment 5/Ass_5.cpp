#include <iostream>
#include <vector>
#include <string>
using namespace std;
 
struct Road {
    int to;     // next city
    int cost;   // delivery cost of this road
};
 
const int INF = 1000000;
 
// Cities are numbered stage by stage: 0 = source, 9 = destination.
string cityName[] = {"Warehouse", "Hub-A", "Hub-B", "Hub-C", "Zone-D",
                     "Zone-E", "Zone-F", "Depot-G", "Depot-H", "Customer"};
 
// Finds the cheapest route to the destination (working backwards)
void findBestRoute(vector<vector<Road>>& graph) {
    int n = graph.size();
    vector<int> dp(n, INF);     // dp[u] = cheapest cost from u to destination
    vector<int> next(n, -1);    // next[u] = city to visit after u
 
    dp[n - 1] = 0;              // destination to destination costs nothing
 
    // Go backwards: from second last city up to the source
    for (int u = n - 2; u >= 0; u--) {
        for (Road r : graph[u]) {
            if (r.cost + dp[r.to] < dp[u]) {
                dp[u] = r.cost + dp[r.to];
                next[u] = r.to;
            }
        }
    }
 
    cout << "City        Cheapest cost to Customer   Next city\n";
    cout << "---------------------------------------------------\n";
    for (int u = 0; u < n; u++) {
        cout << cityName[u] << string(12 - cityName[u].size(), ' ')
             << dp[u] << string(28 - to_string(dp[u]).size(), ' ')
             << (next[u] == -1 ? "-" : cityName[next[u]]) << "\n";
    }
 
    cout << "\nBest route: ";
    for (int u = 0; u != -1; u = next[u]) {
        cout << cityName[u];
        if (next[u] != -1) cout << " -> ";
    }
    cout << "\nTotal cost: " << dp[0] << "\n";
}
 
int main() {
    vector<vector<Road>> graph(10);
 
    // Stage 1 -> Stage 2
    graph[0] = {{1, 4}, {2, 3}, {3, 5}};
    // Stage 2 -> Stage 3
    graph[1] = {{4, 3}, {5, 6}};
    graph[2] = {{4, 7}, {5, 2}, {6, 4}};
    graph[3] = {{5, 3}, {6, 5}};
    // Stage 3 -> Stage 4
    graph[4] = {{7, 6}, {8, 2}};
    graph[5] = {{7, 4}, {8, 5}};
    graph[6] = {{7, 3}, {8, 6}};
    // Stage 4 -> Stage 5
    graph[7] = {{9, 5}};
    graph[8] = {{9, 3}};
 
    cout << "===== Normal conditions =====\n";
    findBestRoute(graph);
 
    // Traffic jam: road Hub-A -> Zone-D now costs 10 instead of 3
    graph[1][0].cost = 10;
 
    cout << "\n===== After traffic update (Hub-A -> Zone-D cost = 10) =====\n";
    findBestRoute(graph);
 
    return 0;
}
 
