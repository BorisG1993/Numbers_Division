#ifndef FIND_NO_SOLUTIONS_H
#define FIND_NO_SOLUTIONS_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <exception>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <numeric>

//#include <thread>
//#include <mutex>
//#include <condition_variable>

#include "shlomo_src/shlomo_main.h"
#include "shlomo_src/Partition.h"
#include "shlomo_src/PartitionGenerator.h"
#include "shlomo_src/InitAssignmentStrategy.h"
#include "partition_generator_wrapper.h"
#include "utils.h"

//const int MAX_THREADS = 1000;
const int MAX_TWO_POWS = 2;
const int MAX_POW_OF_TWOS_OFFSET = 0;
const std::string INIT_ASSIGNMENT_STRATEGIES_STR = "RR/10,WS/10,RND/10,GRD/10";
const double NK_RATIO = 2.65;
const double NK_RATIO_OFFSET = 0.06;
const int MIN_P = 4;
const int MAX_COMBINATIONS = 100000000;
const int BRUTE_FORCE_TRIALS = 10;
const int SECONDS_LIMIT = 1;

const int FIRST_ITEM_OF_P = 2; 
const std::vector<int> NEXT_ITEMS_OF_P = {4,4,5,5,5,5,5,5};
const int INIT_POW_OF_TWO = 103;
const int INIT_POW_OF_TWO_INCREMENT_ONE = 5;
const int INIT_POW_OF_TWO_INCREMENT_TWO = 3;
const bool PRINT = true;




// singleton class
// search for new no solutions that were not detected by known practices 
class FindNoSolutions {

private:
    
    FindNoSolutions(){}
    
    // helper functions for a brute force algorithm that searches for solution using hte most brute force technique
    // not effective where combinations number is high
    // ---------------------------------------------------------------------------------------------
    
    static void find_size_p_combinations_to_build_s 
        (const std::vector<int>& nums, 
        int rem_p, 
        int rem_s, 
        std::set<int>& path, 
        std::vector<std::set<int>>& res,
        int start = 0);
    
    static std::vector<std::set<int>> find_pow_of_p_independant_combinations_from_vec_of_sets
        (const std::vector<std::set<int>>& vec_of_sets, const int& p, const int& pow);

    static std::map<std::pair<int,int>,std::set<int>> 
    solve(const std::map<int,int>& p_map, 
          std::map<int,int>::const_iterator next_p, 
          const std::vector<int>& nums, 
          const int& S, 
          std::map<std::pair<int,int>,std::set<int>> solution);

    // --------------------------------------------------------------------------------------------

    static std::string solution_to_string (const std::map<std::pair<int, int>, std::set<int>>& solution);
    static inline int count_occurences_in_vec(const int& num, const std::vector<int> nums);

    // returns all numbers k that correspond to given n to k ratio
    static std::vector<int> get_ks_around_ratio(const int& n, const double& ratio, const double& epsilon);
    
public:
    
    FindNoSolutions(const FindNoSolutions&) = delete;
    FindNoSolutions& operator=(const FindNoSolutions&) = delete;
    
    // returns a map for each item^pow in p with its set of numbers to fill
    static std::map<std::pair<int,int>,std::set<int>> find_solution(const Partition& partition);

    static std::string partition_to_string(const Partition& partition);
    static FindNoSolutions& getInstance();
    
    // looks for a combination not recognized by known practices and outputs results to a file
    static void search_for_potential_new_no_solution
        (std::ofstream& output_file, const int& n_begin, const int& n_end);
    
    // tries to solve a combination with known practices
    static SolutionType determine_with_known_ways(Partition partition);
};


class TooManyCombinations : public std::exception {
public:
    const char* what() const noexcept override {
        return "too many combinations";
    }
};


#endif // FIND_NO_SOLUTIONS_H
