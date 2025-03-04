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
    };
    
    std::vector<Prefix> prefixes;

    int get_pow_of_two(const Prefix& prefix);
    
public:

    std::vector<GapPowTwos> gaps_pow_twos;

    AnalyzeNoSolutions(){}
    
    int read_file(const std::string& filepath);
    int write_to_file(const std::string &filepath);
    void make_gaps_of_twos();

    friend std::ostream& operator<< (std::ostream& os, const  GapPowTwos& gpt) {
        os << "n: " << gpt.n << " k: " << gpt.k << " S: " << gpt.snk << " pow2gap: " << gpt.gap;
        return os;
    }

};

#endif //ANALYZE_NO_SOLUTIONS_H
