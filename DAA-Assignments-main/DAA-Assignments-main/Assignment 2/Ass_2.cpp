#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;
 
struct Movie {
    string title;
    int releaseYear;
    double imdbRating;
    int watchTimeMinutes;
};
 
const int SMALL_SUBARRAY_LIMIT = 16; // below this size, switch to insertion sort
 
// Plain insertion sort, used as the fallback for small partitions
void insertionSort(vector<Movie>& movies, int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        Movie key = movies[i];
        int j = i - 1;
        while (j >= low && movies[j].imdbRating < key.imdbRating) {
            movies[j + 1] = movies[j];
            j--;
        }
        movies[j + 1] = key;
    }
}
 
// Median-of-three pivot selection: looks at the first, middle and last
// ratings of the current range and returns the index that holds the median.
int medianOfThreeIndex(vector<Movie>& movies, int low, int high) {
    int mid = low + (high - low) / 2;
    double a = movies[low].imdbRating, b = movies[mid].imdbRating, c = movies[high].imdbRating;
 
    if ((a > b) != (a > c)) return low;
    if ((b > a) != (b > c)) return mid;
    return high;
}
 
// Hoare partition scheme, sorting movies by IMDB rating in descending order
int hoarePartition(vector<Movie>& movies, int low, int high) {
    int pivotIdx = medianOfThreeIndex(movies, low, high);
    double pivotRating = movies[pivotIdx].imdbRating;
 
    int i = low - 1;
    int j = high + 1;
 
    while (true) {
        do { i++; } while (movies[i].imdbRating > pivotRating);
        do { j--; } while (movies[j].imdbRating < pivotRating);
        if (i >= j) return j;
        swap(movies[i], movies[j]);
    }
}
 
void quickSort(vector<Movie>& movies, int low, int high) {
    while (low < high) {
        // Hybrid optimization: fall back to insertion sort for small ranges
        if (high - low + 1 <= SMALL_SUBARRAY_LIMIT) {
            insertionSort(movies, low, high);
            return;
        }
 
        int splitPoint = hoarePartition(movies, low, high);
 
        // Tail-recursion elimination: recurse into the smaller side only,
        // and loop for the larger side instead of making a second call
        if (splitPoint - low < high - splitPoint) {
            quickSort(movies, low, splitPoint);
            low = splitPoint + 1;
        } else {
            quickSort(movies, splitPoint + 1, high);
            high = splitPoint;
        }
    }
}
 
int main() {
    ifstream infile("movies_dataset.csv");
    if (!infile) {
        cout << "Error: could not open movies dataset!" << endl;
        return 1;
    }
    cout << "Movies dataset opened successfully." << endl;
 
    string line;
    getline(infile, line); // skip header row
 
    vector<Movie> movies;
    while (getline(infile, line)) {
        stringstream ss(line);
        Movie m;
        string yearStr, ratingStr, watchStr;
        getline(ss, m.title, ',');
        getline(ss, yearStr, ',');
        getline(ss, ratingStr, ',');
        getline(ss, watchStr, ',');
        m.releaseYear = stoi(yearStr);
        m.imdbRating = stod(ratingStr);
        m.watchTimeMinutes = stoi(watchStr);
        movies.push_back(m);
    }
    infile.close();
 
    cout << "Total movies loaded: " << movies.size() << endl;
 
    quickSort(movies, 0, movies.size() - 1);
 
    cout << "\nTop 20 movies sorted by IMDB rating (highest to lowest):\n\n";
    cout << left
         << setw(5) << "No."
         << setw(30) << "Title"
         << setw(8) << "Year"
         << setw(10) << "Rating"
         << setw(12) << "Watch(min)"
         << endl;
    cout << "-------------------------------------------------------------------\n";
 
    int limit = min((int)movies.size(), 20);
    for (int i = 0; i < limit; i++) {
        cout << left
             << setw(5) << i + 1
             << setw(30) << movies[i].title.substr(0, 29)
             << setw(8) << movies[i].releaseYear
             << setw(10) << fixed << setprecision(1) << movies[i].imdbRating
             << setw(12) << movies[i].watchTimeMinutes
             << endl;
    }
 
    return 0;
} 
