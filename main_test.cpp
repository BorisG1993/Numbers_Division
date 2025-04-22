#include <iostream>
#include <ostream>

#include "find_no_solutions.h"

int main() {
    
    std::string filepath = "/home/borisg/Projects/NumbersDivision/search_no_solution.txt";   
    std::ofstream file(filepath, std::ios::app);
    if (!file) std::cerr << "Error opening file: " << filepath << std::endl;
    
    FindNoSolutions::search_for_potential_new_no_solution(file, 207, 208); 
    
    if (file.is_open()) file.close();

//     Partition partition (20, 6, {2,2,2,3,4,7});
//     Partition partition (39,13, {2,2,2,2,2,2,2,2,2,3,3,5,10});   
//     std::vector<int> p (68,2);
//     std::vector<int> p2 {3,3,5,5,5,5,5,9,9,22};
//     p.insert(p.end(), p2.begin(), p2.end());
//     Partition partition (207, 78, p);
//     std::map<std::pair<int,int>,std::set<int>> sol = FindNoSolutions::find_solution(partition);
//     std::cout << std::endl <<  sol.empty() << std::endl;

    return 0;
}
