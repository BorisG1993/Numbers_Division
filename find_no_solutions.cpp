#include "find_no_solutions.h"

#include <cmath>
#include <algorithm>

#include "shlomo_src/PartitionGenerator.h"
#include "utils.h"

const int MAX_COMBINATIONS = 100000;




FindNoSolutions& FindNoSolutions::getInstance() {
    static FindNoSolutions instance;
    return instance;
}


std::string FindNoSolutions::solution_to_string (const std::map<std::pair<int, int>, std::set<int>>& solution) {
        
    std::stringstream ss;
    
    for (const auto& entry : solution) {
        ss << "(" << entry.first.first << ", " << entry.first.second << ") -> ";
        ss << "{ ";
        for (const int& val : entry.second) {
            ss << val << " ";
        }
        ss << "}" << " | ";
    }
    
    return ss.str();
}


std::string FindNoSolutions::partition_to_string(const Partition& partition) {
    
    std::stringstream ss;
    ss << "n=" << partition.n << ", k=" << partition.k << ", S=" << partition.S <<", p=[ ";

    int p_val_counter = 1;
    int current_p_val;

    for (size_t i = 0; i <partition.p.size();) {

        current_p_val = partition.p[i++];
        ss << current_p_val;
        while (i < partition.p.size() && partition.p[i] == current_p_val) {
            p_val_counter++;
            i++;
        }
    
        if (p_val_counter > 1) {
            ss << "^" << p_val_counter << " ";
            p_val_counter = 1;
        }
        else ss << " ";
    }
    ss << "]";
            
    return ss.str();
}


void FindNoSolutions::search_for_no_solution(std::ofstream& output_file, const int& n_begin, const int& n_end, const int& min_p, const double& nk_ratio) {
    
    long long counter_1 = 0;
    long long counter_2 = 0;

    int n = n_begin;
    while (std::fmod(n, nk_ratio) != 0 && n < n_end) n++;
    
    while (n < n_end) {
        
          
        int k = n/nk_ratio;

        if (! Partition::is_valid(n,k)) {
            n += nk_ratio;
            continue;
        }
        
        PartitionGenerator partition_generator(n,k,min_p);
        PartitionGeneratorIterator pgi = partition_generator.begin();
        while (pgi != partition_generator.end()) {
            
            if (! Partition::is_valid(n,k,*pgi)) {
                pgi.next();
                continue;
            }

            Partition partition(n,k,*pgi); 
            
            std::cout << "\33[2K\r" << counter_2 << " | " << ++counter_1 << " - " <<  partition_to_string(partition) << std::flush;
                
            bool no_solution_from_criterions = (no_solution_from_criterion_1(partition) || no_solution_from_criterion_2(partition));
            if (! no_solution_from_criterions ) {
                
                    std::cout << "33[2K\r" <<  ++counter_2 << " | " << counter_1 << " - meet criteria " << partition_to_string(partition) << std::flush; 
                    std::map<std::pair<int,int>,std::set<int>> solution = FindNoSolutions::find_solution(partition);

                    if (! solution.empty()) {
                        output_file << "Solution: " << partition_to_string(partition) << " ---> " << solution_to_string(solution) << std::endl;
                    }

                    else {
                        std::cout << "No solution was found!" << std::endl;
                        std::cout << "No Solution: " <<  partition_to_string(partition) << std::endl;
                        output_file << "No Solution: " <<  partition_to_string(partition) << std::endl;
                        return;
                    }
            }
            pgi.next();
        }
        n += nk_ratio;
    }   
    std::cout << "done" << std::endl;
}


std::map<std::pair<int,int>,std::set<int>> FindNoSolutions::find_solution(const Partition& partition) {
    
    // <p,pow>
    std::map<int,int> p_map;
    for (const int& num : partition.p) {
        p_map[num]++; 
    }

    std::vector<int> nums;
    for (int i=1; i <= partition.n; i++) nums.push_back(i);
    
    auto iter = p_map.begin();
    std::map<std::pair<int,int>,std::set<int>> solution;
    
    return solve(p_map, iter, nums, partition.S, solution);
}


 std::map<std::pair<int,int>,std::set<int>> FindNoSolutions::solve
 (const std::map<int,int>& p_map, 
  std::map<int,int>::const_iterator next_p, 
  const std::vector<int>& nums, 
  const int& S,
  std::map<std::pair<int,int>,std::set<int>> solution) {
    
    if (next_p == p_map.end()) {
        if (nums.empty()) return solution;
        else return {};
    }

    std::vector<std::set<int>> combinations_to_build_p = find_size_p_combinations_to_build_s(nums, next_p->first, S);

    std::vector<std::set<int>> combinations_to_build_p_mult_pow_of_p = 
        find_pow_of_p_independant_combinations_from_vec_of_sets(combinations_to_build_p, next_p->first, next_p->second);

    for (const auto& set : combinations_to_build_p_mult_pow_of_p) {

        std::vector<int> next_nums = nums;
        next_nums.erase(std::remove_if(next_nums.begin(), next_nums.end(), [&](int num){return set.count(num) > 0;}), next_nums.end());
        next_p++;
        std::map<std::pair<int,int>,std::set<int>> next_solution = solution;
        next_solution[{next_p->first, next_p->second}].insert(set.begin(), set.end());
        solution = solve(p_map, next_p, next_nums, S, next_solution); 
        if (!solution.empty()) return solution; 
    }

    return {};
}


std::vector<std::set<int>> FindNoSolutions::find_size_p_combinations_to_build_s (const std::vector<int>& nums, const int& p, const int& s) {
    
    std::vector<std::set<int>> size_p_combinations_to_build_s;
    
    int n = nums.size();
    std::vector<int> bitmask (n,0);
    std::fill(bitmask.end() - p, bitmask.end(), 1);
    
    do {

        std::set<int> combination;
        
        int sum = 0;
        for (int i=0; i < n; ++i)  {
            if (bitmask[i]) {
                sum += nums[i];
                if (sum > s) break;
                combination.insert(nums[i]);
            }
        }
        
        if (sum == s) size_p_combinations_to_build_s.push_back(combination);

    } while (std::next_permutation(bitmask.begin(), bitmask.end()));

    return size_p_combinations_to_build_s;
}


std::vector<std::set<int>> FindNoSolutions::find_pow_of_p_independant_combinations_from_vec_of_sets 
    (const std::vector<std::set<int>>& vec_of_sets, const int& p, const int& pow) {
    
    std::vector<std::set<int>> pow_of_p_independant_combinations_from_vec_of_sets;

    int n = vec_of_sets.size();
    if (n == 0) return pow_of_p_independant_combinations_from_vec_of_sets;
    size_t p_mult_pow = p * pow;

    std::vector<int> bitmask (n,0);
    
    if (pow > n) return pow_of_p_independant_combinations_from_vec_of_sets;
    std::fill(bitmask.end() - pow, bitmask.end(), 1);
    
    do {
        
        std::set<int> combinations_union;

        for (int i=0; i < n; ++i) {
            if (bitmask[i]) {
                for (const auto& val : vec_of_sets[i]) {
                    combinations_union.insert(val);
                }
            }
        }
        
        if (combinations_union.size() == p_mult_pow) pow_of_p_independant_combinations_from_vec_of_sets.push_back(combinations_union);

    } while (std::next_permutation(bitmask.begin(), bitmask.end()));
    
    return pow_of_p_independant_combinations_from_vec_of_sets;
}


// from Shlomo src code
bool FindNoSolutions::no_solution_from_criterion_1(const Partition &part) {

    part.assert_valid();
    int n = part.n, k = part.k, S = part.S;
    const std::vector<int>& p = part.p;
    // instead of flase
    if (p[0] != 2 || p[k-1] == 2) return true; // Must be 2^num2 ... for num2 < k for the argument to work
    int num2 = 0;
    while (num2 < k && p[num2] == 2) num2++;
    // assert(num2<k)
    if (! (num2<k))return true;
    int lb = S-n; // set U = {lb, ..., n} where each pair needs two U elements
    // instead of false 
    if (lb < 1) return true;
    int left_after_2 = (n-lb+1) - 2*num2;  // U elements left after populating the pairs
    //assert(left_after_2 >= 0); // otherwise slack is violated
    if (! (left_after_2 >= 0)) return true;
    int slack = 0; // slack using only lb-1, lb-2, ... on p[num2+left_after_2] ... p[k-1] must be >= 0
    int current_element = lb-1;
    for(int i=num2+left_after_2; i<k; i++) {
        slack += p[i]*(current_element + current_element-p[i]+1)/2 - S;
        if (slack < 0) return true;
        current_element -= p[i];
    }
    return false;
}


// from Shlomo src code
bool FindNoSolutions::no_solution_from_criterion_2(const Partition &part) {
    part.assert_valid();
    int n = part.n, k = part.k, S = part.S;
    const std::vector<int>& p = part.p;
    // instead of false
    if (p[0] != 2 || p[k-1] == 2) return true; // Must be 2^num2 ... for num2 < k for the argument to work
    int num2 = 0;
    while (num2 < k && p[num2] == 2) num2++;
    // assert(num2<k)
    if (! (num2<k)) return true;
    int lb = S-n; // set U = {lb, ..., n} where each pair needs two U elements
    // instead of false
    if (lb < 1) return true;
    int left_after_2 = (n-lb+1) - 2*num2;  // U elements left after populating the pairs
    //assert(left_after_2 >= 0); // otherwise slack is violated
    if (! (left_after_2 >= 0)) return true;
    // instead of false
    if (left_after_2%2 == 0) return true; // is this condition actually needed?
    int midU = (lb+n)/2;
    int X = p[num2]; // the next element
    if (midU + sum_arithmetic(lb-1,-1,X-1) >= S) return false; // argument not going to work
    int numX = 0;
    while (num2 + numX < k && p[num2 + numX] == X) numX++;
    int num_U_elements_gt_mid = left_after_2/2;
    int num_U_elements_leq_mid = (left_after_2+1)/2;
    for(int i=numX; i>0; i--) {
        if (num_U_elements_gt_mid > 0)
            num_U_elements_gt_mid--;
        else if (num_U_elements_leq_mid >= 2)
            num_U_elements_leq_mid -= 2;
        else
            return true;
    }
    int current_index = num2 + numX + num_U_elements_gt_mid + num_U_elements_leq_mid; // at this index U is exhausted
    if (current_index >= k) return false;
    int Y = p[current_index]; // width of first part after U is exhausted
    return (sum_arithmetic(lb-1,-1,Y) < S);
}
