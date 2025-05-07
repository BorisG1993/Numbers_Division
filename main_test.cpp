#include <iostream>

#include "find_no_solutions.h"

int main() {
    
    std::string filepath = "/home/borisg/Projects/NumbersDivision/search_no_solution.txt";   
    std::ofstream file(filepath, std::ios::app);
    if (!file) std::cerr << "Error opening file: " << filepath << std::endl;
    
    FindNoSolutions::search_for_potential_new_no_solution(file, 201, 300); 
    
    if (file.is_open()) file.close();

//     Partition partition (20, 6, {2,2,2,3,4,7});
//     Partition partition (39,13, {2,2,2,2,2,2,2,2,2,3,3,5,10});   
//     std::vector<int> p (73,2);
//     std::vector<int> p2 {3,3,5,5,5,5,5,6,11,11,19};
//     p.insert(p.end(), p2.begin(), p2.end());
//     Partition partition (224, 84, p);
//     FindNoSolutions::determine_with_known_ways(partition);
    return 0;
}
