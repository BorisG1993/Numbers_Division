#include "PartitionGenerator.h"
#include <cassert>

PartitionGeneratorIterator::PartitionGeneratorIterator(int _n, int _k, int _min_p, bool at_end)
    : n(_n), k(_k), min_p(_min_p), _p(k,min_p) {
    assert(min_p >= 1 && k >= 1 && n >= 1);
    if (at_end || min_p*k > _n)
        _p.clear();
    else
        _p[k-1] = n-min_p*(k-1);
    assert_valid();
}

void PartitionGeneratorIterator::assert_valid() const {
    if (at_end()) return;
    assert(static_cast<int>(_p.size()) == k);
    assert(k >= 1 && n >= 1 && _p[0] >= 1);
    for(int i=0; i<k-1; i++) assert(_p[i] <= _p[i+1]);
    int sum = 0;
    for(int i: _p) sum += i;
    assert(sum == n);
}

const std::vector<int>& PartitionGeneratorIterator::p() const {
    assert(!at_end());
    return _p;
}

bool PartitionGeneratorIterator::at_end() const {
    return _p.empty();
}

void PartitionGeneratorIterator::next() {
    for(int i = k-2; i >= 0; i--) {
        if (_p[k-1] - _p[i] >= 2) {
            _p[i]++;
            int carry = -1;
            for(int j=i+1; j<k-1; j++) {
                assert(_p[j] >= _p[i]);
                carry += (_p[j] - _p[i]);
                _p[j] = _p[i];
            }
            _p[k-1] += carry;
            return;
        }
    }
    _p.clear();
}
