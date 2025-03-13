#include <iostream>
#include <ostream>

#include "find_no_solutions.h"

int main() {
    

    Partition partition (27, 9, {2,2,3,3,3,3,3,3,5});
    FindNoSolutions::find_solution(partition);
    
    std::string filepath = "/home/borisg/Projects/NumbersDivision/search_no_solution.txt";   
    std::ofstream file(filepath);
    if (!file) std::cerr << "Error opening file: " << filepath << std::endl;
    
    FindNoSolutions::search_for_no_solution(file, 1, 50, 2, 3); 
    
    if (file.is_open()) file.close();
 
    return 0;
}
