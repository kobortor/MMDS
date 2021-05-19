#include <bits/stdc++.h>

using namespace std;

// Dummy function
int jacard_similarity(const set<int> &s1, const set<int> &s2);

// Naive O(n^2) solution
vector<pair<int, int>> similar_sets(const vector<set<int>> &v1, const vector<set<int>> &v2, double threshold) {
    vector<pair<int, int>> similar_pairs;
    for (int i = 0; i < (int)v1.size(); i++) {
        for (int j = 0; j < (int)v2.size(); j++) {
            if (jacard_similarity(v1[i], v2[j]) > threshold) {
                similar_pairs.push_back(make_pair(i, j));
            }
        }
    }

    return similar_pairs;
}