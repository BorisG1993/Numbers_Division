#include <iostream>
#include "trim_no_solutions.h"

int main() {
    
    TrimNoSolutions tns;
    std::string src_filepath = "/home/borisg/Projects/NumbersDivision/NoSolution.txt";
    std::string dst_filepath = "/home/borisg/Projects/NumbersDivision/output.txt";
    tns.trim_no_solutions(src_filepath, dst_filepath);

    return 0;
}
