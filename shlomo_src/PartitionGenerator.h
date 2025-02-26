#ifndef PARTITIONS_PARTITION_GENERATOR_H
#define PARTITIONS_PARTITION_GENERATOR_H

#include <vector>

class PartitionGeneratorIterator {
public:
    PartitionGeneratorIterator(int n, int k, int min_p, bool at_end=false);
    const int n, k, min_p;

    const std::vector<int>& p() const;
    bool at_end() const;

    // Advance to next partition. If there is no next, mark at_end and return.
    void next();

    const std::vector<int>& operator*() const { return p(); }
    PartitionGeneratorIterator& operator++() { next(); return *this; }
    bool operator==(const PartitionGeneratorIterator& other) const { return _p == other._p; }
    bool operator!=(const PartitionGeneratorIterator& other) const { return _p != other._p; }
private:
    void assert_valid() const;
    std::vector<int> _p;
};

class PartitionGenerator {
public:
    PartitionGenerator(int _n, int _k, int _min_p) : n(_n), k(_k), min_p(_min_p) {}
    PartitionGeneratorIterator begin() const { return PartitionGeneratorIterator(n, k, min_p, false);  }
    PartitionGeneratorIterator end() const { return PartitionGeneratorIterator(n, k, min_p, true); }
    const int n, k, min_p;
private:
};


#endif //PARTITIONS_PARTITION_GENERATOR_H
