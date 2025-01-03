#include "Partition.h"

#include "utils.h"
#include <cassert>
#include <iostream>
#include <limits>

using namespace std;

Partition::Partition(int _n, int _k, const std::vector<int> &_p) :
        n(_n), k(_k), S(static_cast<long>(n) * (n + 1) / (2 * k)), p(_p),
        _assignment(n, -1), _part_size(k + 1), _part_sum(k + 1) {
    cout << "n=" << n << ", k=" << k << endl;
    assert(is_valid(n, k, p));
    _part_size[0] = n;
    _part_sum[0] = static_cast<long>(n) * (n + 1) / 2;
    assert_valid();
}

bool Partition::is_valid(int n, int k) {
    assert(n >= k && k >= 1);
    return static_cast<long>(n) * (n + 1) / 2 % k == 0;
}

bool Partition::is_valid(int n, int k, const std::vector<int> &p) {
    if (!is_valid(n, k) || static_cast<int>(p.size()) != k) return false;
    int sum = 0;
    for (int i : p) sum += i;
    if (sum != n) return false;
    for (int i = 0; i < k - 1; i++)
        if (p[i] > p[i + 1]) return false;
    if (p[0] < 0) return false;
    return true;
}

void Partition::assert_valid() const {
    assert(is_valid(n,k,p));
    assert(static_cast<int>(_assignment.size()) == n);
    assert(static_cast<int>(_part_size.size()) == k+1);
    assert(static_cast<int>(_part_sum.size()) == k+1);
#ifndef NDEBUG
    vector<int> num_assigned(k+1);
    vector<long> sum_assigned(k+1);
    for(int i = 0; i<n; i++) {
        int j = _assignment[i];
        assert(-1 <= j && j < k);
        num_assigned[j+1]++;
        sum_assigned[j+1] += i+1;
    }
    for(int j=0; j<k; j++)
        assert(num_assigned[j + 1] <= p[j]);
    for(int j=-1; j<k; j++) {
        assert(num_assigned[j + 1] == _part_size[j + 1]);
        assert(sum_assigned[j + 1] == _part_sum[j + 1]);
    }
#endif
}

void Partition::copy_assignment_from(const Partition& other) {
    assert(n == other.n && k == other.k && p == other.p);
    _assignment = other._assignment;
    _part_size = other._part_size;
    _part_sum = other._part_sum;
    _diff = other._diff;
    assert_valid();
}

void Partition::clear_assignment() {
    std::fill(_assignment.begin(), _assignment.end(), -1);
    std::fill(_part_size.begin(), _part_size.end(), 0);
    std::fill(_part_sum.begin(), _part_sum.end(), 0);
    _part_size[0] = n;
    _part_sum[0] = static_cast<long>(n) * (n + 1) / 2;
    _diff = -1;
}

void Partition::_print() const {
    cout << "n=" << n << ", k=" << k << ", S=" << S << ", p = " << p << endl;
    cout << "_assignment = " << _assignment << endl;
    cout << "_part_size = " << _part_size << endl;
    cout << "_part_sum = " << _part_sum << endl;
}

void Partition::assign(int i, int j) {
    assert(1 <= i && i <= n);
    assert(-1 <= j && j < k);
    _part_size[_assignment[i-1]+1]--;
    _part_size[j+1]++;
    _part_sum[_assignment[i-1]+1] -= i;
    _part_sum[j+1] += i;
    _assignment[i-1] = j;
    _diff = -1;
//    assert_valid();
}

void Partition::swap(int i1, int i2) {
    assert(1 <= i1 && i1 <= n);
    assert(1 <= i2 && i2 <= n);
    assert(is_complete());
    int j1 = _assignment[i1-1];
    int j2 = _assignment[i2-1];
    if (j1 == j2) return; // nothing to do
    _assignment[i1-1] = j2;
    _assignment[i2-1] = j1;
    int S1 = _part_sum[j1+1];
    int S2 = _part_sum[j2+1];
    int di = i2 - i1;
    _part_sum[j1+1] += di;
    _part_sum[j2+1] -= di;
    if (_diff != -1) {
        // di = i2-i1
        // S1, S2 are the original sums and S1' = S1 + i2-i1, S2' = S2 - (i2-i1) are the new sums.
        //
        // delta_diff = (S1' - S)^2 + (S2' - S)^2 - (S1 - S)^2 - (S2 - S)^2
        //            = (S1 + di - S)^2 + (S2 - di -S)^2 - (S1-S)^2 - (S2-S)^2
        //            = (S1 + di)^2 + (S2 - di)^2 - S1^2 - S2^2 + 2*S*(S1+di+S2-di-S1-S2)
        //            = (S1 + di)^2 + (S2 - di)^2 - S1^2 - S2^2
        //            = 2*di^2 + 2*di*(S1-S2) = 2*di*(di+S1-S2)
        _diff += 2*di*(di+S1-S2);
    }
//    assert_valid();
}

long Partition::diff_brute_force() const {
    assert(is_complete());
    long d = 0;
    for(int j=0; j<k; j++)
        d += (part_sum(j) - S)*(part_sum(j) - S);
    return d;
}

long Partition::slack() const {
    long slack = numeric_limits<long>::max();
    long curr_slack = 0;
    int curr_num = n;
    for(int j=0; j<k-1; j++) {
        curr_slack += (curr_num + curr_num - p[j] + 1)*p[j]/2 - S;
        curr_num -= p[j];
        slack = min(slack, curr_slack);
    }
    assert(curr_num == p[k-1]);
    assert(curr_slack + (curr_num + 1)*p[k-1]/2 - S == 0);
    return slack;
}

void Partition::print_assignment(std::ostream& os) const {
    for(int j=0; j<k; j++) {
        vector<int> curr;
        for(int i=1; i<=n; i++)
            if (assignment(i) ==j) curr.push_back(i);
        assert(static_cast<int>(curr.size()) <= p[j]);
        os << "[" << curr;
        for(int i=static_cast<int>(curr.size()); i<p[j]; i++) os << " -";
        os << "] ";
        if (part_sum(j) != S) os << "(" << part_sum(j) - S << ")";
        os << " ";
    }
    if (is_complete()) os << " -- D=" << diff_brute_force();
    os << endl;
    assert_valid();
}
