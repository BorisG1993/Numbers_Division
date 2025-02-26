#ifndef PARTITIONS_PARTITION_H
#define PARTITIONS_PARTITION_H

#include <cassert>
#include <vector>
#include <iostream>
//#include "utils.h" // XXX

class Partition {
public:
    const int n, k;
    const long S;
    const std::vector<int> p; // length k, sum up to n, sorted ascending

    Partition(int _n, int _k, const std::vector<int> &_p);

    static bool is_valid(int n, int k);
    static bool is_valid(int n, int k, const std::vector<int> &p);

    // checks current partial assignment is valid and no part j gets more than p[j] elements
    void assert_valid() const;

    // Copy the assignment from a partition with the same n, k, p.
    void copy_assignment_from(const Partition& other);

    // Clear current assignment
    void clear_assignment();

    // Given i in {1,...,n} return the part it is assigned to {-1,0,...,k-1} where -1 means unassigned
    int assignment(int i) const { assert(1 <= i && i <= n); return _assignment[i-1]; }

    // Assigns i in {1,...,n} to j in {-1,0,...,k-1}. j == -1 means de-assignment of i.
    void assign(int i, int j);

    // Swaps i1 with i2 where both are in {1,...,n}.
    void swap(int i1, int i2);

    // Return number/sum of assigned elements to part j, where j is in {-1,0,...,k-1}.
    // For j == -1 the number/sum of unassigned elements is returned.
    int part_size(int j) const { assert(-1 <= j && j < k); return _part_size[j+1]; }
    int part_sum(int j) const { assert(-1 <= j && j < k); return _part_sum[j+1]; }

    inline bool is_complete() const { return part_size(-1) == 0; }
    // Sum square deviations from S.
    long diff_brute_force() const;
    long diff() const {
        if (_diff == -1) _diff = diff_brute_force();
        return _diff;
    }

    long slack() const;
    void print_assignment(std::ostream& os) const;

    void _print() const;
private:
    std::vector<int> _assignment; // maps {1,...n} to {-1,0,...,k-1} where -1 means unassigned
    std::vector<int> _part_size; // _part_size[j] is the number of elements with assigned to j
    std::vector<long> _part_sum; // _part_sum[i] is the sum of elements assigned to j
    mutable long _diff = -1;
};


#endif //PARTITIONS_PARTITION_H
