#ifndef PARTITIONS_INIT_ASSIGNMENT_STRATEGY_H
#define PARTITIONS_INIT_ASSIGNMENT_STRATEGY_H

#include <string>
#include <vector>
#include <iostream>

#include "Partition.h"

class InitAssignmentStrategy {
public:
    explicit InitAssignmentStrategy(const std::string& strategies);
    explicit operator bool() const { return _it != _strategies.end(); }
    void operator() (Partition &part) const { (*_it)(part); }
    InitAssignmentStrategy& operator++() { _it++; return *this; }
    void reset() { _it = _strategies.begin(); }
    void Print(std::ostream& os) const;

private:
    typedef void (*InitStrategy)(Partition& part);
    static void whole_set_populate_assignment(Partition &part);    // "WS"
    static void round_robin_populate_assignment(Partition &part);  // "RR"
    static void random_populate_assignment(Partition &part);       // "RND"
    static void adaptive_populate_assignment(Partition &part);       // "GRD"

    void append_strategy(const std::string& strategy);
    std::vector<InitStrategy> _strategies;
    std::vector<InitStrategy>::iterator _it;
};

inline std::ostream& operator<<(std::ostream& os, const InitAssignmentStrategy& init_assignment_strategies) {
    init_assignment_strategies.Print(os);
    return os;
}

#endif //PARTITIONS_INIT_ASSIGNMENT_STRATEGY_H
