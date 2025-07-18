#ifndef CRITERIA_H
#define CRITERIA_H

#include "shlomo_src/Partition.h"


// New criteria extensions will be here.
class Criteria {
    
public:
    
    Criteria() = delete;
    ~Criteria() = delete;

    static bool is_unsolvable_criteria_2_1(const Partition &part);

private:

    static int cut_lb(const int& nextX, int& numNextX, const int& lb, const Partition &part);
};


#endif // CRITERIA_H

