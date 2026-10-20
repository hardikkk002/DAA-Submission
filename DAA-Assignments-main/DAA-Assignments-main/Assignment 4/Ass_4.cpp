#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <unordered_set>
using namespace std;
 
struct Road {
    int to;
    int travelTime; // minutes, may change with traffic
};
 
// Reads the road network from a CSV of (u, v, travelTimeMinutes) rows
vector<vector<Road>> loadRoadNetwork(const string& filename, int numIntersections) {
    vector<vector<Road>> graph(numIntersections);
    ifstream infile(filename);
    string line;
    getline(infile, line); // skip header
 
    while (getline(infile, line)) {
        stringstream ss(line);
        string uStr, vStr, wStr;
        getline(ss, uStr, ',');
        getline(ss, vStr, ',');
        getline(ss, wStr, ',');
        int u = stoi(uStr), v = stoi(vStr), w = stoi(wStr);
        graph[u].push_back({v, w});
        graph[v].push_back({u, w}); // roads are two-way
    }
    return graph;
}
 
// Standard Dijkstra using a min-priority queue; returns shortest travel
// time from source to every intersection
vector<int> shortestTravelTimes(const vector<vector<Road>>& graph, int source) {
    int n = graph.size();
    vector<int> dist(n, INT_MAX);
    dist[source] = 0;
 
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;
    pq.push({0, source});
 
    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue; // stale entry
 
        for (const Road& r : graph[u]) {
            int newDist = dist[u] + r.travelTime;
            if (newDist < dist[r.to]) {
                dist[r.to] = newDist;
                pq.push({newDist, r.to});
            }
        }
    }
    return dist;
}
 
// Applies a live traffic-update file (intersectionA, intersectionB, newTravelTime)
// on top of an existing graph, modelling dynamic congestion changes
void applyTrafficUpdates(vector<vector<Road>>& graph, const string& filename) {
    ifstream infile(filename);
    if (!infile) return; // no updates this run
    string line;
    getline(infile, line); // skip header
 
    while (getline(infile, line)) {
        stringstream ss(line);
        string uStr, vStr, wStr;
        getline(ss, uStr, ',');
        getline(ss, vStr, ',');
        getline(ss, wStr, ',');
        int u = stoi(uStr), v = stoi(vStr), newW = stoi(wStr);
 
        for (auto& road : graph[u]) if (road.to == v) road.travelTime = newW;
        for (auto& road : graph[v]) if (road.to == u) road.travelTime = newW;
    }
}
 
int main() {
    int numIntersections = 8;
    vector<vector<Road>> graph = loadRoadNetwork("road_network.csv", numIntersections);
 
    int ambulanceSource = 0;
    cout << "Road network loaded: " << numIntersections << " intersections.\n";
    cout << "Ambulance current location: intersection " << ambulanceSource << "\n";
 
    vector<int> hospitals = {5, 7};
 
    cout << "\n--- Pass 1: normal traffic conditions ---\n";
    vector<int> dist1 = shortestTravelTimes(graph, ambulanceSource);
    int best1 = -1, bestTime1 = INT_MAX;
    for (int h : hospitals) {
        cout << "Hospital at intersection " << h << ": "
             << (dist1[h] == INT_MAX ? -1 : dist1[h]) << " min\n";
        if (dist1[h] < bestTime1) { bestTime1 = dist1[h]; best1 = h; }
    }
    cout << "Nearest hospital (normal traffic): intersection " << best1
         << ", travel time " << bestTime1 << " minutes.\n";
 
    // Simulate a live traffic update (e.g. congestion on a previously fast route)
    cout << "\n--- Live traffic update applied ---\n";
    applyTrafficUpdates(graph, "traffic_update.csv");
 
    cout << "\n--- Pass 2: after dynamic traffic update ---\n";
    vector<int> dist2 = shortestTravelTimes(graph, ambulanceSource);
    int best2 = -1, bestTime2 = INT_MAX;
    for (int h : hospitals) {
        cout << "Hospital at intersection " << h << ": "
             << (dist2[h] == INT_MAX ? -1 : dist2[h]) << " min\n";
        if (dist2[h] < bestTime2) { bestTime2 = dist2[h]; best2 = h; }
    }
    cout << "Nearest hospital (updated traffic): intersection " << best2
         << ", travel time " << bestTime2 << " minutes.\n";
 
    if (best1 != best2) {
        cout << "\nRouting decision changed due to live traffic: "
             << "now routing to intersection " << best2
             << " instead of " << best1 << ".\n";
    } else {
        cout << "\nRouting decision unchanged despite traffic update.\n";
    }
 
    return 0;
}
 
