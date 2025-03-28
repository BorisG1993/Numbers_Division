#ifndef ANALYZE_NO_SOLUTIONS_H
#define ANALYZE_NO_SOLUTIONS_H

#include "utils.h"

#include <string>
#include <vector>
#include <map>
#include <array>


class AnalyzeNoSolutions {

private:

    struct GapPowTwos {

        int n;
        int k;
        long snk;

        int gap;

        friend std::ostream& operator<< (std::ostream& os, const  GapPowTwos& gpt) {
            os << "n: " << gpt.n << " k: " << gpt.k << " S: " << gpt.snk << " pow2gap: " << gpt.gap;
            return os;
        }
    };
    
    std::vector<Prefix> prefixes;

    int get_pow_of_two(const Prefix& prefix);
    int read_file(const std::string& filepath);

    template<typename T>
    int write_to_file(const std::string &filepath, const std::vector<T>& target);
    
public:

    AnalyzeNoSolutions(){}
    
    void make_gaps_of_twos(const std::string& src_filepath, const std::string& dst_filepath);
    void filter_by_criterion(const std::string& src_filepath, const std::string& dst_filepath, const int& criterion);
};

#endif //ANALYZE_NO_SOLUTIONS_H
