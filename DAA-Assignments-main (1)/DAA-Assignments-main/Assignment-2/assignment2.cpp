#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

struct Movie {
    string title;
    int year;
    double rating;
    int voteCount;
};

const int MAX = 10000;
Movie movies[MAX];
int count = 0;

void swapMovies(int i, int j) {
    Movie temp = movies[i];
    movies[i] = movies[j];
    movies[j] = temp;
}

int partition(int low, int high, int sortBy) {
    double pivot;
    if (sortBy == 1) pivot = movies[high].rating;
    else if (sortBy == 2) pivot = movies[high].year;
    else pivot = movies[high].voteCount;

    int i = low - 1;
    for (int j = low; j < high; j++) {
        double value;
        if (sortBy == 1) value = movies[j].rating;
        else if (sortBy == 2) value = movies[j].year;
        else value = movies[j].voteCount;

        if (value >= pivot) {
            i++;
            swapMovies(i, j);
        }
    }
    swapMovies(i + 1, high);
    return i + 1;
}

void quicksort(int low, int high, int sortBy) {
    if (low < high) {
        int p = partition(low, high, sortBy);
        quicksort(low, p - 1, sortBy);
        quicksort(p + 1, high, sortBy);
    }
}

void splitLine(const string& line, string fields[], int maxFields) {
    int idx = 0;
    string field;
    bool inQuotes = false;

    for (int i = 0; i < (int)line.size(); i++) {
        char c = line[i];
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            if (idx < maxFields) fields[idx++] = field;
            field.clear();
        } else {
            field += c;
        }
    }
    if (idx < maxFields) fields[idx++] = field;
}

int main() {
    ifstream file("E:\\Shriya\\sem 5\\DAA\\movies.csv");
    if (!file.is_open()) {
        cout << "Could not open file. Put the CSV in the same folder.\n";
        return 0;
    }

    string line;
    getline(file, line); 

    const int NUM_COLS = 8;

    while (getline(file, line) && count < MAX) {
        string fields[NUM_COLS];
        splitLine(line, fields, NUM_COLS);

        try {
            string title = fields[2];
            string releaseDate = fields[4];
            string ratingStr = fields[6];
            string voteCountStr = fields[7];

            if (title.empty() || releaseDate.size() < 4) continue;

            int year = stoi(releaseDate.substr(0, 4));
            double rating = stod(ratingStr);
            int voteCount = voteCountStr.empty() ? 0 : (int)stod(voteCountStr);

            movies[count].title = title;
            movies[count].year = year;
            movies[count].rating = rating;
            movies[count].voteCount = voteCount;
            count++;
        } catch (...) {
            continue;
        }
    }

    cout << "Loaded " << count << " movies.\n";

    int choice;
    cout << "Sort by: 1-Rating  2-Year  3-Vote Count\nEnter choice: ";
    cin >> choice;

    quicksort(0, count - 1, choice);

    cout << "\nTop 10 Movies:\n";
    for (int i = 0; i < 10 && i < count; i++) {
        cout << movies[i].title << " (" << movies[i].year << ") | "
             << "Rating: " << movies[i].rating << " | "
             << "Vote Count: " << movies[i].voteCount << "\n";
    }

    return 0;
}
