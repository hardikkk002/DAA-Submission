#include <bits/stdc++.h>
using namespace std;
 
int main() {
    vector<string> subjects = {"DAA", "AIML", "SPM", "EAC", "MDM-3"};
    // Colour k (1-based) is shown with a name; colour k = exam slot k.
    vector<string> colourName = {"Blue", "Orange", "Green", "Red", "Purple",
                                 "Yellow", "Pink", "Brown", "Grey", "Cyan"};
    int n = subjects.size(), m;
    vector<vector<int>> graph(n);
 
    cout << "Subjects:\n";
    for (int i = 0; i < n; i++) cout << i << " - " << subjects[i] << "\n";
    cout << "Enter number of conflicts: ";
    cin >> m;
    cout << "Enter conflicting pairs (u v):\n";
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        if (u < 0 || u >= n || v < 0 || v >= n || u == v) {
            cout << "Invalid conflict!\n";
            return 1;
        }
        graph[u].push_back(v);
        graph[v].push_back(u);
    }
 
    // Greedy colouring: smallest colour not used by an already coloured neighbour.
    vector<int> slot(n, -1);
    for (int u = 0; u < n; u++) {
        vector<bool> used(n + 1, false);
        for (int v : graph[u])
            if (slot[v] != -1) used[slot[v]] = true;
        int c = 0;
        while (used[c]) c++;
        slot[u] = c;
    }
    int totalSlots = *max_element(slot.begin(), slot.end()) + 1;
 
    // ---- Final colour of every vertex ----
    cout << "\n----- GRAPH COLOURING RESULT -----\n";
    cout << left << setw(10) << "Vertex" << setw(10) << "Course"
         << setw(10) << "Colour#" << setw(10) << "Colour" << "Exam slot\n";
    for (int i = 0; i < n; i++) {
        string cname = slot[i] < (int)colourName.size() ? colourName[slot[i]]
                                                        : "C" + to_string(slot[i] + 1);
        cout << left << setw(10) << i << setw(10) << subjects[i]
             << setw(10) << slot[i] + 1 << setw(10) << cname
             << "Slot " << slot[i] + 1 << "\n";
    }
    cout << "Colours used (chromatic number found): " << totalSlots << "\n";
 
    // Verify: no edge joins two vertices of the same colour.
    bool valid = true;
    for (int u = 0; u < n; u++)
        for (int v : graph[u])
            if (slot[u] == slot[v]) valid = false;
    cout << "Colouring check: " << (valid ? "VALID (no adjacent vertices share a colour)"
                                           : "INVALID") << "\n";
 
    int roomCount;
    cout << "\nEnter number of rooms: ";
    cin >> roomCount;
    vector<int> capacity(roomCount);
    cout << "Enter capacity of each room:\n";
    for (int &c : capacity) cin >> c;
    vector<int> students(n);
    cout << "Enter student count for DAA, AIML, SPM, EAC, MDM-3:\n";
    for (int &s : students) cin >> s;
 
    cout << "\n----- EXAM TIMETABLE -----\n";
    for (int t = 0; t < totalSlots; t++) {
        string cname = t < (int)colourName.size() ? colourName[t] : "C" + to_string(t + 1);
        cout << "\nSlot " << t + 1 << " (" << cname << "):\n";
        vector<bool> occupied(roomCount, false);
        for (int i = 0; i < n; i++) {
            if (slot[i] != t) continue;
            int chosen = -1;
            for (int r = 0; r < roomCount; r++) {
                if (!occupied[r] && capacity[r] >= students[i] &&
                    (chosen == -1 || capacity[r] < capacity[chosen]))
                    chosen = r;
            }
            cout << subjects[i] << " (" << students[i] << " students) - ";
            if (chosen == -1) cout << "NO SUITABLE ROOM\n";
            else {
                occupied[chosen] = true;
                cout << "Room " << chosen + 1 << "\n";
            }
        }
    }
    cout << "\nTotal exam slots: " << totalSlots << "\n";
    return 0;
}
 
