#include <iostream>
#include <ostream>

#include "find_no_solutions.h"

int main() {
    
    std::string filepath = "/home/borisg/Projects/NumbersDivision/search_no_solution.txt";   
    std::ofstream file(filepath);
    if (!file) std::cerr << "Error opening file: " << filepath << std::endl;
    
    FindNoSolutions::search_for_potential_new_no_solution(file, 201, 300, 2, 3); 
    
    if (file.is_open()) file.close();
 
    return 0;
}
