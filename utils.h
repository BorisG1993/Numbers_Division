#ifndef PARTITIONS_UTILS_H
#define PARTITIONS_UTILS_H

#include <iostream>
#include <vector>
#include <cassert>
#include <functional>




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


inline long long binom_coeff(int n, int r) {

    if (r > n - r) r = n - r;
    
    long long res = 1;
    for (int i = 0; i < r; i++) {
        res *= (n-i);
        res /= (i+1);
    }
    return res;
}


#endif //PARTITIONS_UTILS_H
