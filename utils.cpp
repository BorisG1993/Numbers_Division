#include "utils.h"

#include <vector>
#include <regex>


Prefix build_p_from_line(const std::string& line) {
        
    int n = 0, k = 0, snk = 0;
    std::vector<int> p;

    std::regex rgx(R"(n=(\d+), k=(\d+), .*p=\[(.*?)\])");
    std::smatch match;

    if (std::regex_search(line, match, rgx)) {

        n = std::stoi(match[1].str());
        k = std::stoi(match[2].str());
        std::string p_str = match[3].str();

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
    Prefix prefix  = {p, n, k, snk};
    
    return prefix;
}




