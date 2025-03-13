#ifndef FIND_NO_SOLUTIONS_H
#define FIND_NO_SOLUTIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>

#include "Partition.h"
#include "PartitionGenerator.h"
#include "utils.h"


class FindNoSolutions {

private:
    
    FindNoSolutions(){}

    static std::vector<std::set<int>> find_size_p_combinations_to_build_s (const std::vector<int>& nums, const int& p, const int& S);

    static std::vector<std::set<int>> find_pow_of_p_independant_combinations_from_vec_of_sets
        (const std::vector<std::set<int>>& vec_of_sets, const int& p, const int& pow);

    static std::map<std::pair<int,int>,std::set<int>> 
        solve(const std::map<int,int>& p_map, 
                std::map<int,int>::const_iterator next_p, 
                const std::vector<int>& nums, 
                const int& S, 
                std::map<std::pair<int,int>,std::set<int>> solution);

    static std::string solution_to_string (const std::map<std::pair<int, int>, std::set<int>>& solution);

public:
    
    FindNoSolutions(const FindNoSolutions&) = delete;
    FindNoSolutions& operator=(const FindNoSolutions&) = delete;
    
    static std::map<std::pair<int,int>,std::set<int>> find_solution(const Partition& partition);
    static bool no_solution_from_criterion_1(const Partition& partition);
    static bool no_solution_from_criterion_2(const Partition& partition);

    static std::string partition_to_string(const Partition& partition);

    static FindNoSolutions& getInstance();

    static void search_for_no_solution(std::ofstream& output_file, const int& n_begin, const int& n_end, const int& min_p, const double& nk_ratio);
    
};


#endif // FIND_NO_SOLUTIONS_H
