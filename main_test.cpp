#include <iostream>
#include <ostream>

#include "analyze_no_solutions.h"

int main() {
     
    std::string src = "trimmed.txt";
    std::string dst = "trimmed_filtered_by_criterion_2.txt";
    AnalyzeNoSolutions ans;
    ans.filter_by_criterion(src,dst, 2);
 
    return 0;
}
