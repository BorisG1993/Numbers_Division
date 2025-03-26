#include "analyze_no_solutions.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "shlomo_src/utils.h"




int AnalyzeNoSolutions::read_file(const std::string& filepath) {
    
    std::ifstream file (filepath);
    if (!file) {
        std::cerr << "Error openning file: " << filepath << std::endl;
        return 1;
    }

    this->prefixes.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        Prefix prefix = build_p_from_line(line);
        this->prefixes.push_back(prefix);
    }   
    
    std::cout << "Writing done" << std::endl;
    return 0;
}


int AnalyzeNoSolutions::write_to_file(const std::string& filepath) {
     
    std::ofstream file(filepath);
    if (!file) {
        std::cerr << "Error opening file " << filepath << std::endl;
        return 1;
    }
    
    for (const GapPowTwos& gpt : gaps_pow_twos) {
        file << gpt << std::endl;
    }
    
    file.close();
    return 0;
}


void AnalyzeNoSolutions::make_gaps_of_twos(const std::string& src_filepath, const std::string& dst_filepath) {
        
    read_file(src_filepath);

    std::vector<Prefix>::iterator it = prefixes.begin();

    while (it != prefixes.end()) {

        GapPowTwos gap_pow_twos;
        gap_pow_twos.n = it->n;
        gap_pow_twos.k = it->k;
        gap_pow_twos.snk = it->snk;
        gap_pow_twos.gap = it->n - gap_pow_twos.snk/2;
        
        int max = gap_pow_twos.gap;
        int min = gap_pow_twos.gap;
        
        it++;
        while (it != prefixes.end()) {
            if (it->n == gap_pow_twos.n && it->k == gap_pow_twos.k && it->snk == gap_pow_twos.snk) {
                int next_pow_two = get_pow_of_two(*it);
                if (next_pow_two < min) min = next_pow_two;
                it++;
            }
            else break;
        }
        gap_pow_twos.gap = max - min;
        gaps_pow_twos.push_back(gap_pow_twos);
    }

    write_to_file(dst_filepath);
}


int AnalyzeNoSolutions::get_pow_of_two(const Prefix& prefix) {
   
    int i = 0;
    
    for (int num : prefix.p) {
        if (num == 2) i++;
        else break;
    }
    
    return i;
}

