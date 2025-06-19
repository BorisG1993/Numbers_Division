#include <iostream>

#include "find_no_solutions.h"

int main() {
    
    std::string filepath = "/home/borisg/Projects/NumbersDivision/search_no_solution.txt";   
    std::ofstream file(filepath, std::ios::app);
    if (!file) std::cerr << "Error opening file: " << filepath << std::endl;
    
    FindNoSolutions::search_for_potential_new_no_solution(file, 299, 325); 
    
    if (file.is_open()) file.close();
 
// //     Partition partition (20, 6, {2,2,2,3,4,7});
// //     Partition partition (39,13, {2,2,2,2,2,2,2,2,2,3,3,5,10});   
//     std::vector<int> p (103,2);
//     std::vector<int> p2 {4,4,5,5,5,5,5,5,8,8,12,27};
//     p.insert(p.end(), p2.begin(), p2.end());
//     Partition partition (299, 115, p);
//     std::cout << (FindNoSolutions::determine_with_known_ways(partition) == SolutionType::Criterion_2_1) << std::endl;

    return 0;
}
