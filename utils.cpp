#include "utils.h"

#include <vector>
#include <regex>
#include <cassert>




Prefix build_prefix_from_line(const std::string& line) {
        
    int n = 0, k = 0, snk = 0, criterion = 0;
    std::vector<int> p;

    std::regex rgx(R"(n=(\d+), k=(\d+), .*p=\[(.*?)\], Criterion (\d+))");
    std::smatch match;

    if (std::regex_search(line, match, rgx)) {

        n = std::stoi(match[1].str());
        k = std::stoi(match[2].str());
        std::string p_str = match[3].str();
        criterion = std::stoi(match[4].str());

        std::regex p_token_rgx("(\\d+)(?:\\^(\\d+))?");
        auto begin = std::sregex_iterator(p_str.begin(), p_str.end(), p_token_rgx);
        auto end = std::sregex_iterator();

        for (std::sregex_iterator i = begin; i != end; ++i) {
            std::smatch match = *i;
            int p_val = std::stoi(match[1].str());
            int p_pow = match[2].length() > 0 ? std::stoi(match[2].str()) : 1;

            for (int p_reps = 0; p_reps < p_pow; ++p_reps) {
                p.push_back(p_val);
            }
        }
    }
    snk = static_cast<long>(n) * (n + 1) / (2 * k);
    Prefix prefix  = {p, n, k, snk, criterion};
    
    return prefix;
}



std::string build_line_from_prefix(const Prefix& prefix) {

    std::stringstream ss;
    ss << "n=" << prefix.n << ", k=" << prefix.k << ", S=" << prefix.snk <<", p=[";

    int p_val_counter = 1;
    int current_p_val;

    for (size_t i = 0; i <prefix.p.size();) {

        current_p_val = prefix.p[i++];
        ss << current_p_val;
        
        while (i < prefix.p.size() && prefix.p[i] == current_p_val) {
            p_val_counter++;
            i++;
        }
    
        if (p_val_counter > 1) {
            ss << "^" << p_val_counter;
            p_val_counter = 1;
        }
            
        if (i == prefix.p.size()) {
            ss << "], Criterion " << prefix.criterion;
        }

        else {
                ss << " ";
            }
        }

    return ss.str();
}

