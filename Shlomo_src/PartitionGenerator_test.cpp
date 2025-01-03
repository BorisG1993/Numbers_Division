//
// Created by hoory on 05/10/2023.
//

#include "PartitionGenerator_test.h"
#include "PartitionGenerator.h"
#include "utils.h"
#include <cassert>
#include <iostream>

using namespace std;

static bool compare(const vector<vector<int>> &ref, PartitionGeneratorIterator &pg) {
    int ref_len = ref.size();
    for (int i = 0; i < ref_len; i++) {
        if (pg.p() != ref[i]) {
            cout << "i=" << i << ", actual_p=[" << pg.p() << "], ref_p=[" << ref[i] << "]" << endl;
            return false;
        }
        pg.next();
        bool actual_at_end = pg.at_end();
        bool ref_at_end = (i == ref_len - 1);
        if (actual_at_end != ref_at_end) {
            cout << "i=" << i << ", actual_at_end=" << actual_at_end << ", ref_at_end = " << ref_at_end << endl;
            return false;
        }
    }
    return true;
}

static void all_partition_rec(int n, int k, int min_p, vector<vector<int>> &res, vector<int> &prefix) {
    if (k == 0) {
        if (n == 0) res.push_back(prefix);
        return;
    }
    for (int p1 = min_p; p1 <= n; p1++) {
        prefix.push_back(p1);
        all_partition_rec(n - p1, k - 1, p1, res, prefix);
        prefix.pop_back();
    }
}

// All size k partitions
static vector<vector<int>> all_partition(int n, int k, int min_p) {
    vector<vector<int>> res;
    vector<int> prefix;
    all_partition_rec(n, k, min_p, res, prefix);
    return res;
}

static bool compare(int n, int k, int min_p) {
    PartitionGeneratorIterator pg(n, k, min_p);
    vector<vector<int>> ref = all_partition(n, k, min_p);
//    cout << "n=" << n << ", k=" << k << ", min_p=" << min_p << ", ref.size()=" << ref.size() << endl;
    return compare(ref,pg);
}

void test_partition_generator() {
    {
        int n = 10, k = 3, min_p = 1;
        PartitionGeneratorIterator pg(n, k, min_p);
        const vector<vector<int>> ref = {
                {1, 1, 8}, {1, 2, 7}, {1, 3, 6}, {1, 4, 5},
                {2, 2, 6}, {2, 3, 5}, {2, 4, 4}, {3, 3, 4},
        };
        assert(compare(ref, pg));
    }
    {
        int n = 12, k = 5, min_p = 1;
        PartitionGenerator pg(n, k, min_p);
        const vector<vector<int>> ref = {
                {1, 1, 1, 1, 8}, {1, 1, 1, 2, 7}, {1, 1, 1, 3, 6}, {1, 1, 1, 4, 5},
                {1, 1, 2, 2, 6}, {1, 1, 2, 3, 5}, {1, 1, 2, 4, 4}, {1, 1, 3, 3, 4},
                {1, 2, 2, 2, 5}, {1, 2, 2, 3, 4}, {1, 2, 3, 3, 3}, {2, 2, 2, 2, 4},
                {2, 2, 2, 3, 3}
        };
         vector<vector<int>> actual;
         for(auto p : pg) actual.push_back(p);
         assert(actual == ref);
        PartitionGeneratorIterator pgi = pg.begin();
          assert(compare(ref, pgi));
    }
    assert(compare(8,6,2)); // empty
    assert(compare(10,3,1));
    assert(compare(12,5,1));
    assert(compare(12,5,2));
    assert(compare(99,33,2));
}
