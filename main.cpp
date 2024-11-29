#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <utility>
#include <cstdio>

using namespace std;
using VEC = vector<int>;
using MAT = vector<VEC>;
using STATE = tuple<MAT, VEC, MAT, MAT>; // <MAX> <AVAIL> <ALLOC> <NEED>

STATE banker(STATE state, VEC request, int pid);
bool safety(STATE state);
void dump(STATE state);
void print_mat_formatted(MAT mat, string name);

int main() 
{
    // Get input
    string file_name = "input.txt";
    ifstream file(file_name);
    int n, m;
    file >> n >> m;
    // Read n x m MAX vector
    MAT max(n, VEC(m, 0));
    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
            file >> max[i][j];

    // Read AVAIALBLE vector
    VEC avail(m, 0);
    for (int j = 0; j < m; j++) 
        file >> avail[j];
    
    // Initialize other vectors
    MAT alloc(n, VEC(m, 0));
    MAT need = max;

    STATE state = {max, avail, alloc, need};

    while(1)
    {
        // Get request from user
        int pid;
        cout << "What process? ";
        cin >> pid;
        // Exit on negative pid
        if (pid < 0) {
            dump(state);
            break;
        }
        VEC request;
        cout << "Request vector? ";
        string tmp;
        int itmp;
        getline(cin, tmp);
        stringstream ss(tmp);
        for (int j = 0; j < m; j++) {
            ss >> tmp;
            request[j] = stoi(tmp);
        }

        // Run banker's algorithm
        state = banker(state, request, pid);

        // Dump all vectors
        dump(state);
    }
}


STATE banker(STATE state, VEC request, int pid) {
    // Unpack tuple
    MAT max, alloc, need;
    VEC avail;
    tie(max, avail, alloc, need) = state;

    // Check request validity
    if (request > need[pid]) {
        cout << "Request denied - Exceeded maximum claim\n";
        return state;
    }

    if (request > avail) {
        cout << "Request denied - Not enough available resources\n";
        return state;
    }

    // Safety is checked after simulating request

    avail = avail - request;
    alloc[pid] = alloc[pid] + request;
    need[pid] = max[pid] - alloc[pid];

    STATE old_state = state; // save old state incase new one is unsafe
    state = {max, avail, alloc, need};

    if (!safety(state)) {
        cout << "Request denied - Will leave system in unsafe state\n";
        return old_state;
    }

    cout << "Request granted\n";
    return state;
}


bool safety(STATE state) {
    // Unpack tuple
    MAT max, alloc, need;
    VEC avail;
    tie(max, avail, alloc, need) = state;
    int n = max.size();

    // Create other vectors needed for the algorithm
    VEC work = avail;
    vector<bool> finish(n, false);

    bool found = true;
    while(found == true) { // Repeat until no more processes can be completed
        found = false;
        // Find a process that could complete
        for (int i = 0; i < n; i++) {
            if (finish[i] == false && need[i] <= work) {
                work = work + alloc[i];
                finish[i] = true;
                found = true;
            }
        }
    }
    // Check that all can finish
    bool all_good = true;
    for (int i = 0; i < n; i++) {
        if (finish[i] == false) {
            all_good = false;
            break;
        }
    }

    return all_good;
}


void dump(STATE state) {
    // Unpack tuple
    MAT max, alloc, need;
    VEC avail;
    tie(max, avail, alloc, need) = state;
    int n = max.size();
    int m = max[0].size();

    // Print max
    print_mat_formatted(max, string("MAX"));

    cout << "\n";

    // Print avail
    cout << "--- AVAILABLE ---\n";
    cout << "R -> ";
    for (int j = 0; j < m; j++)
        printf("%2d ", m);
    cout << "\n";
    cout << "     "; // to match R -> 
    for (int j = 0; j < m; j++)
        printf("%2d ", avail[m]);

    cout << "\n";

    // Print alloc
    print_mat_formatted(max, string("ALLOCATED"));

    cout << "\n";

    // Print need
    print_mat_formatted(max, string("NEED"));

    cout << "\n";

    return;
}

// Specific to the banker's algorithm. Prints the process ids (such as P1), the resource indices, and the name of the matrix
void print_mat_formatted(MAT mat, string name) {
    int n = mat.size();
    int m = mat[0].size();

    cout << "--- " << name << " ---\n";
    cout << "R -> ";
    for (int j = 0; j < m; j++)
        printf("%2d ", m);
    cout << "\n";
    for (int i = 0; i < n; i++) {
        cout << "P%d -> ", n;
        for (int j = 0; j < m; j++) {
            printf("%2d ", mat[i][j]);
        }
        cout << "\n";
    }
}


// vec1 < iff vec1[i] < vec2[i] for all i = 1 to size
bool operator<(const vector<int>& vec1, const vector<int>& vec2) {
    // Check that both vectors have the same size
    if (vec1.size() != vec2.size()) {
        return false;  // We can decide that vectors of different sizes should return false
    }

    // Compare elements one by one
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] >= vec2[i]) {
            return false; // If any element of vec1 is not strictly less than vec2, return false
        }
    }

    return true; // If all elements of vec1 are strictly less than vec2, return true
}

// vec1[i] + vec2[i] for all i from 1 to size
vector<int> operator+(const vector<int>& vec1, const vector<int>& vec2) {
    // Check if the vectors are of the same size
    if (vec1.size() != vec2.size()) {
        throw invalid_argument("Vectors must have the same size for addition");
    }

    std::vector<int> result(vec1.size());

    // Add corresponding elements
    for (size_t i = 0; i < vec1.size(); ++i) {
        result[i] = vec1[i] + vec2[i];
    }

    return result;
}

// vec1[i] - vec2[i] for all i from 1 to size
vector<int> operator-(const vector<int>& vec1, const vector<int>& vec2) {
    // Check if the vectors are of the same size
    if (vec1.size() != vec2.size()) {
        throw invalid_argument("Vectors must have the same size for subtraction");
    }

    vector<int> result(vec1.size());

    // Subtract corresponding elements
    for (size_t i = 0; i < vec1.size(); ++i) {
        result[i] = vec1[i] - vec2[i];
    }

    return result;
}

// vec1 <= iff vec1[i] <= vec2[i] for all i = 1 to size
bool operator<=(const vector<int>& vec1, const vector<int>& vec2) {
    // Check if the vectors are of the same size
    if (vec1.size() != vec2.size()) {
        throw invalid_argument("Vectors must have the same size for comparison");
    }

    // Compare element-wise: each element in vec1 should be <= the corresponding element in vec2
    for (size_t i = 0; i < vec1.size(); ++i) {
        if (vec1[i] > vec2[i]) {
            return false;  // If any element of vec1 is greater than vec2, return false
        }
    }

    return true;  // Return true if all elements in vec1 are less than or equal to vec2
}