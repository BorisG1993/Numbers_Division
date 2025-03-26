#ifndef PARTITIONS_UTILS_H
#define PARTITIONS_UTILS_H

#include <iostream>
#include <vector>
#include <cassert>




struct Prefix {
    std::vector<int> p;
    int n;
    int k;
    long snk;
};


Prefix build_p_from_line(const std::string& line);


inline int sum_arithmetic(int first_element, int step, int num_elements) {
    int sum=0;
    for(int i=0, elem = first_element; i<num_elements; i++, elem += step)
        sum += elem;
    int result = num_elements*(2*first_element+step*(num_elements-1))/2;
    assert(result == sum);
    return result;
}




#endif //PARTITIONS_UTILS_H
