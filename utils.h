#ifndef PARTITIONS_UTILS_H
#define PARTITIONS_UTILS_H

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <vector>

template<class T>
std::ostream& operator<<(std::ostream &os, const std::vector<T>& vec) {
    const char *prefix = "";
    for(T i : vec) {
        os << prefix << i;
        prefix = " ";
    }
    return os;
}

template<class T>
std::string compress_run_length(const std::vector<T>& vec, char power_ch = '^', char sep_ch = ' ') {
    std::ostringstream os;
    int count = 0;
    bool is_first = true;
    T curr;
    auto print_curr = [&os, &curr, &count, &is_first, sep_ch, power_ch]() {
        if (count == 0) return;
        if (!is_first)
            os << sep_ch;
        is_first = false;
        if (count == 1)
            os << curr;
        else
            os << curr << power_ch << count;
    };
    for(const T& elem : vec) {
        if (count == 0 || elem != curr) {
            print_curr();
            curr = elem;
            count = 0;
        }
        count++;
    }
    print_curr();
    return os.str();
}

inline bool with_prob(double p) {
    return rand() < p*RAND_MAX;
}

inline int random_element(const std::vector<int>& vec) {
    return vec[rand() % vec.size()];
}

inline int random_uniform(int lb, int ub) {
    return lb + (rand() % (ub+1-lb));
}


struct Prefix {
    std::vector<int> p;
    int n;
    int k;
    long snk;
};

Prefix build_p_from_line(const std::string& line);


#endif //PARTITIONS_UTILS_H
