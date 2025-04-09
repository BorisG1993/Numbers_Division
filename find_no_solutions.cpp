#include "find_no_solutions.h"
#include <cmath>
#include "shlomo_src/PartitionGenerator.h"
#include "shlomo_src/InitAssignmentStrategy.h"

const int MAX_TWO_POWS = 15;
const std::string INIT_ASSIGNMENT_STRATEGIES_STR = "GRD,RND,WS,RND/10,GRD";




FindNoSolutions& FindNoSolutions::getInstance() {
    static FindNoSolutions instance;
    return instance;
}


void FindNoSolutions::search_for_potential_new_no_solution(std::ofstream& output_file, const int& n_begin, const int& n_end, const int& min_p, const double& nk_ratio) {
   
    int twos_pow_counter = 0;
    int current_twos_pow = 0;

    int total_counter = 0;
    int potential_new_no_solution_counter = 0;
    int no_solution_slack_counter = 0;
    int no_solution_strategies_counter = 0;
    int solution_found_counter = 0;

    int n = n_begin;
    while (std::fmod(n, nk_ratio) != 0 && n < n_end) n++;

    while (n < n_end) {


        int k = n/nk_ratio;

        if (! Partition::is_valid(n,k)) {
            n += nk_ratio;
            continue;
        }

        auto print = [&](const Partition& partition) {
            std::cout << "\33[2K\r" << "total: " << total_counter << " | " << 
                " no sol slack: " << no_solution_slack_counter << " | " <<
                " no sol strat: " << no_solution_strategies_counter << " | " <<
                " sol found: " << solution_found_counter << " | " <<
                " unknown: " << potential_new_no_solution_counter << " | " <<
                partition_to_string(partition) << std::flush;
        };

        PartitionGenerator partition_generator(n,k,min_p);
        PartitionGeneratorIterator pgi = partition_generator.begin();
        current_twos_pow = count_occurences_in_vec(2,*pgi);

        while (pgi != partition_generator.end() && twos_pow_counter < MAX_TWO_POWS) {
            
            int next_twos_pow = count_occurences_in_vec(2, *pgi);
            if (current_twos_pow != next_twos_pow) {
                current_twos_pow = next_twos_pow;
                twos_pow_counter++;
            }

            if (! Partition::is_valid(n,k,*pgi)) {
                pgi.next();
                continue;
            }

            Partition partition(n,k,*pgi); 
            SolutionType determined_with_known_ways = determine_with_known_ways(partition);  
            
            switch (determined_with_known_ways) {

                case SolutionType::Unknown:
                    potential_new_no_solution_counter ++;
                    output_file << "Potential New No Solution: " <<  partition_to_string(partition) << std::endl;
                    break;
                
                case SolutionType::SolutionFound:
                    solution_found_counter ++;
                    break;
                
                case SolutionType::NoSolutionSlack:
                    no_solution_slack_counter ++;
                    break;

                default:
                    no_solution_strategies_counter ++;
                    break;
            }

            total_counter++;
            print(partition);
            pgi.next();
        }

        twos_pow_counter = 0;
        n += nk_ratio;
    }   

    std::cout << std::endl << "done" << std::endl;
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


SolutionType FindNoSolutions::determine_with_known_ways(Partition partition) {
    
    std::string init_assignment_strategies_str = INIT_ASSIGNMENT_STRATEGIES_STR;
    InitAssignmentStrategy init_assignment_strategies(init_assignment_strategies_str);

    Stats stats;
    SolutionType solution_type = handle_partition(partition, stats, init_assignment_strategies);
    return solution_type;
}


inline int FindNoSolutions::count_occurences_in_vec(const int& num, const std::vector<int> nums) {
    auto range = std::equal_range(nums.begin(), nums.end(), num);
    return range.second - range.first;
}
