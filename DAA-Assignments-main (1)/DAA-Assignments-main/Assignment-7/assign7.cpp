
#include <bits/stdc++.h>
using namespace std;

int main() {
    ifstream in("input.txt");
    ofstream out("output.txt");

    if (!in) {
        cout << "Error: Cannot open input.txt\n";
        return 1;
    }

    if (!out) {
        cout << "Error: Cannot create output.txt\n";
        return 1;
    }

    int courses, students;
    in >> courses >> students;

    if (!in || courses <= 0 || students < 0) {
        cout << "Error: Invalid input data\n";
        return 1;
    }

    // Step 1: Create conflict graph using adjacency sets.
    vector<set<int>> graph(courses + 1);

    for (int s = 0; s < students; s++) {
        int count;
        in >> count;

        if (!in || count < 0) {
            cout << "Error: Invalid enrolment data\n";
            return 1;
        }

        vector<int> enrolled(count);

        for (int i = 0; i < count; i++) {
            in >> enrolled[i];

            if (!in || enrolled[i] < 1 ||
                enrolled[i] > courses) {
                cout << "Error: Invalid course number\n";
                return 1;
            }
        }

        // Courses taken by the same student conflict.
        for (int i = 0; i < count; i++) {
            for (int j = i + 1; j < count; j++) {
                int a = enrolled[i];
                int b = enrolled[j];

                if (a != b) {
                    graph[a].insert(b);
                    graph[b].insert(a);
                }
            }
        }
    }

    // Step 2: Sort courses by descending degree.
    vector<int> order(courses);
    iota(order.begin(), order.end(), 1);

    sort(order.begin(), order.end(),
         [&](int a, int b) {
             if (graph[a].size() != graph[b].size())
                 return graph[a].size() > graph[b].size();

             return a < b;
         });

    // Step 3: Assign colours using Welsh-Powell.
    vector<int> colour(courses + 1, 0);
    int maxColour = 0;

    for (int course : order) {
        set<int> usedColours;

        for (int neighbour : graph[course]) {
            if (colour[neighbour] != 0)
                usedColours.insert(colour[neighbour]);
        }

        int c = 1;

        while (usedColours.count(c))
            c++;

        colour[course] = c;
        maxColour = max(maxColour, c);
    }

    // Step 4: Assign names to exam-slot colours.
    vector<string> colourNames = {
        "",
        "BLUE", "GREEN", "RED", "ORANGE",
        "PURPLE", "CYAN", "MAGENTA", "YELLOW",
        "PINK", "BROWN", "GOLD", "TEAL"
    };

    auto getColourName = [&](int c) -> string {
        if (c < (int)colourNames.size())
            return colourNames[c];

        return "COLOUR_" + to_string(c);
    };

    // Step 5: Display and save the timetable.
    cout << "\n===== EXAM TIMETABLE =====\n";
    cout << "Number of courses: " << courses << "\n";
    cout << "Number of exam slots used: "
         << maxColour << "\n\n";

    out << "===== EXAM TIMETABLE =====\n";
    out << "Number of courses: " << courses << "\n";
    out << "Number of exam slots used: "
        << maxColour << "\n\n";

    for (int c = 1; c <= maxColour; c++) {
        cout << "Slot " << c << " ["
             << getColourName(c) << "]: ";

        out << "Slot " << c << " ["
            << getColourName(c) << "]: ";

        bool first = true;

        for (int course = 1; course <= courses; course++) {
            if (colour[course] == c) {
                if (!first) {
                    cout << ", ";
                    out << ", ";
                }

                cout << "Course " << course;
                out << "Course " << course;

                first = false;
            }
        }

        cout << "\n";
        out << "\n";
    }

    // Step 6: Verify that conflicting courses
    // do not have the same colour.
    bool valid = true;

    for (int u = 1; u <= courses; u++) {
        for (int v : graph[u]) {
            if (colour[u] == colour[v])
                valid = false;
        }
    }

    if (valid) {
        cout << "\nConflict check: PASS\n";
        out << "\nConflict check: PASS\n";
    } else {
        cout << "\nConflict check: FAIL\n";
        out << "\nConflict check: FAIL\n";
    }

    cout << "\nResults saved to output.txt\n";

    in.close();
    out.close();

    return 0;
}
