#include <bits/stdc++.h>

using namespace std;

// returns the minhash value given a set of hash functions
vector<int> minhash(const set<int> &s, const vector<function<int(int)>> &hash_functions) {
    int n = hash_functions.size();
    vector<int> soln(n, INT_MAX);

    for (int i : s) {
        for (int j = 0; j < n; j++) {
            soln[j] = min(soln[j], hash_functions[j](i));
        }
    }

    return soln;
}