#include "Partition.h"
#include <cassert>
#include <iostream>

using namespace std;

void test_partition() {
    Partition part(3,2,{1,2});
    assert(!part.is_complete());
    assert(part.n == 3);
    assert(part.k == 2);
    assert(part.p == vector<int>({1,2}));
    assert(part.S == 3);
    part.assert_valid();
    assert(part.part_size(-1) == 3);
    assert(part.part_sum(-1) == 1+2+3);
    for(int j=0; j<2; j++) assert(part.part_size(j) == 0);
    for(int j=0; j<2; j++) assert(part.part_sum(j) == 0);
    for(int i=1; i<=3; i++) assert(part.assignment(i) == -1);
    part.assign(3,1);
    assert(part.part_size(-1) == 2);
    assert(part.part_sum(-1) == 1+2);
    assert(part.part_size(1) == 1);
    assert(part.part_sum(1) == 3);
    part.assign(2,0);
    part.assign(1,1);
    assert(part.is_complete());
    assert(part.diff_brute_force() == 2);
    part.assign(3,-1);
    part.assign(2,-1);
    part.assign(3,0);
    part.assign(2,1);
    assert(part.is_complete());
    assert(part.diff_brute_force() == 0);
    part.swap(1,2); // {3}, {1,2}
    assert(part.is_complete());
    assert(part.diff_brute_force() == 0);
    part.swap(1,3); // {1}, {2,3}
    assert(part.is_complete());
    assert(part.diff_brute_force() == 8);
}