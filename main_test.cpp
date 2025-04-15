#include <iostream>
#include <ostream>

#include "find_no_solutions.h"

int main() {
    
//     std::string filepath = "/home/borisg/Projects/NumbersDivision/search_no_solution.txt";   
//     std::ofstream file(filepath);
//     if (!file) std::cerr << "Error opening file: " << filepath << std::endl;
//     
//     FindNoSolutions::search_for_potential_new_no_solution(file, 201, 250); 
//     
//     if (file.is_open()) file.close();

    Partition partition (207, 78, {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,3,3,3,3,5,5,7,7,10,10,16});
    std::map<std::pair<int,int>,std::set<int>> sol = FindNoSolutions::find_solution(partition);
    std::cout << sol.empty() << std::endl;

    return 0;
}
