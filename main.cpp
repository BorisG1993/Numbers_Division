#include <iostream>
#include "analyze_no_solutions.h"

int main() {
    
    AnalyzeNoSolutions ans;
    std::string src_filepath = "/home/borisg/Projects/NumbersDivision/output.txt";
    ans.read_file(src_filepath);
    ans.make_gaps_of_twos();
    ans.write_to_file("gaps_pow_twos.txt");

    return 0;
}
