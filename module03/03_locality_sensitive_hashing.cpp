#include <bits/stdc++.h>

using namespace std;

// Just a prototype to satisfy the intellisense
vector<int> minhash(const set<int> &s, const vector<function<int(int)>> &hash_functions);

int hash_range(vector<int>::const_iterator &begin, vector<int>::const_iterator &end) {
    int ans = 0;
    while (begin != end) {
        ans = ans * 13 + *(begin++);
    }
    return ans;
}

vector<int> locality_sensitive_hash(const vector<int> &minhashes, int blocksize) {
    vector<int> soln((minhashes.size() + blocksize - 1) / blocksize);

    for (int i = 0, j = 0; i < (int)minhashes.size(); i += blocksize, j++) {
        vector<int>::const_iterator begin_iter = minhashes.cbegin() + i;
        vector<int>::const_iterator end_iter = min(minhashes.cbegin() + i + blocksize, minhashes.end());
        soln[j] = hash_range(begin_iter, end_iter);
    }

    return soln;
}

// blocksize --> 1 to reduce chance of false negatives
// blocksize --> infinity (or size of a minhash vector) to reduce chance of false positive
// When we have a blocksize of `b` with `r` different blocks (b * r ~= n), then the chance of matching 
//     with something with Jacard Similarity t=(1/b)^(1/r) is approximately 1/e ~= 0.37
// 
// Assume that when n is large, we have b * r = n
// t = (1/b)^(1/r)
// t = (1/b)^(b/n)
// t^n = (1/b)^b
// 1/t^n = b^b
// To solve for `b` use Lampert's W function, however it is usually approximate and can be found online
vector<pair<int, int>> approx_matches_from_minhashes_list(const vector<vector<int>> &minhashes_list, int blocksize, int hashtable_size) {
    if (minhashes_list.empty()) {
        return {};
    }

    int n_vectors = minhashes_list.size();
    int n_blocks = (minhashes_list[0].size() + blocksize - 1) / blocksize;

    vector<vector<vector<int>>> lsh_tables(n_blocks, vector<vector<int>>(hashtable_size));

    for (int i = 0; i < n_vectors; i++) {
        vector<int> lhs = locality_sensitive_hash(minhashes_list[i], blocksize);
        for (int j = 0; j < n_blocks; i++) {
            int idx = lhs[j] % hashtable_size;
            if (idx < 0) {
                idx += hashtable_size;
            }
            lsh_tables[j][idx].push_back(i);
        }
    }

    vector<pair<int, int>> pairs;
    for (int i = 0; i < n_blocks; i++) {
        for (int j = 0; j < hashtable_size; j++) {
            const vector<int> &slot = lsh_tables[i][j];
            for (int k1 = 0; k1 < (int)slot.size(); k1++) {
                for (int k2 = k1 + 1; k2 < (int)slot.size(); k2++) {
                    pairs.push_back(make_pair(slot[k1], slot[k2]));
                }
            }
        }
    }

    sort(pairs.begin(), pairs.end());
    pairs.erase(unique(pairs.begin(), pairs.end()), pairs.end());

    return pairs;
}

vector<pair<int, int>> approx_matches(const vector<set<int>> &data, int blocksize, int hashtable_size, )