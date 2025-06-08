#include "find_no_solutions.h"
#include <cmath>

//std::mutex out_mutex;
// std::mutex pool_mutex;
// std::condition_variable cv;




FindNoSolutions& FindNoSolutions::getInstance() {
    static FindNoSolutions instance;
    return instance;
}


void FindNoSolutions::search_for_potential_new_no_solution(std::ofstream& output_file, const int& n_begin, const int& n_end) {
    
    output_file << std::endl <<  "Test run for n: "
        << n_begin <<  " - " <<
        n_end << " with n to k ratio of: " <<
        NK_RATIO << " +- " <<
        NK_RATIO_OFFSET <<
        " with min_p after 2 = " << MIN_P <<
        std::endl;
    
    int i = INIT_POW_OF_TWO;
//     int i_increment_swap = INIT_POW_OF_TWO_INCREMENT_ONE;

    long long total_counter = 0;
    int potential_new_no_solution_counter = 0;
    long long no_solution_slack_counter = 0;
    long long no_solution_strategies_counter = 0;
    long long solution_found_counter = 0;
    long long criterion_2_1_counter = 0;

    auto capture = [&]() {
        std::stringstream ss;
        ss << "total: " << total_counter << " | " << 
            " no sol slack: " << no_solution_slack_counter << " | " <<
            " no sol criteria: " << no_solution_strategies_counter << " | " <<
            " criterion 2.1: " << criterion_2_1_counter <<
            " sol found: " << solution_found_counter << " | " <<
            " unknown: " << potential_new_no_solution_counter;
        return ss.str();
    };
    
auto capture_partition = [&](const Partition& partition) {
        std::stringstream ss;
        ss << capture() << " | " <<  partition_to_string(partition);
        return ss.str();
    };

    auto print = [&](const Partition& partition) {
        std::cout << "\33[2K\r" << capture_partition(partition) << std::flush;
    };

    
    int n = n_begin;
    while (n < n_end) {
        
        std::vector<int> ks = get_ks_around_ratio(n, NK_RATIO, NK_RATIO_OFFSET);

        for (const int& k : ks) {

//             bool skip = false;
            
            std::chrono::seconds duration;
        
            if (! Partition::is_valid(n,k)) continue;

            long S = get_sum(n,k);
            int offset = MAX_POW_OF_TWOS_OFFSET;
//             int max_pow_of_twos = get_max_pow_of_twos_to_build_sum(n, S) - offset;
            
//                 for (int i = max_pow_of_twos; i > 0 &&  i > (max_pow_of_twos - MAX_TWO_POWS); --i) {

                std::vector<int> prefix(i, FIRST_ITEM_OF_P);
                prefix.insert(prefix.end(), NEXT_ITEMS_OF_P.begin(), NEXT_ITEMS_OF_P.end());
//                 PartitionGenerator partition_generator(n-FIRST_ITEM_OF_P*i,k-i,MIN_P);
                int prefix_n = std::accumulate(prefix.begin(),prefix.end(), 0);

                PartitionGenerator partition_generator(prefix_n,k-prefix.size(),MIN_P); 
                PartitionGeneratorIterator pgi = partition_generator.begin();
                PartitionGeneratorWrapper<int,PartitionGeneratorIterator> pgw(prefix, pgi);
                    
                while (pgi != partition_generator.end()) {
                    
                    if (! Partition::is_valid(n,k,*pgw)) {
                        pgi.next();
                        ++pgw;
                        continue;
                    }

                    Partition partition(n,k,*pgw); 
                    auto start = std::chrono::high_resolution_clock::now();
                    SolutionType determined_with_known_ways = determine_with_known_ways(partition);  
                    auto end = std::chrono::high_resolution_clock::now();
                    duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
                    
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

                        case SolutionType::Criterion_2_1:
                            criterion_2_1_counter ++;
                            break;

                        default:
                            no_solution_strategies_counter ++;
                    }

                    total_counter++;
                    if (PRINT) print(partition);

//                     if (duration > std::chrono::seconds(SECONDS_LIMIT) && determined_with_known_ways != SolutionType::Unknown) {
//                        output_file << "Brute force struggle: " << partition_to_string(partition) << std::endl;
//                        skip = true;
//                        break;
//                     }

                    pgi.next();
                    ++pgw;
                }
 
//                 if (skip) {
//                     i += i_increment_swap;
//                     if (i_increment_swap == POW_OF_TWO_INCREMENT_ONE) i_increment_swap = POW_OF_TWO_INCREMENT_TWO;
//                     else i_increment_swap = POW_OF_TWO_INCREMENT_ONE;
//                     break;
//                 }
            }
//         }   
        n ++;
    }

    output_file << std::endl << capture() << std::endl;
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

    std::set<int> path;
    std::vector<std::set<int>> combinations_to_build_p;
    find_size_p_combinations_to_build_s(nums, next_p->first, S, path, combinations_to_build_p);

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


SolutionType FindNoSolutions::determine_with_known_ways(Partition partition) {
    
    std::string init_assignment_strategies_str = INIT_ASSIGNMENT_STRATEGIES_STR;
    InitAssignmentStrategy init_assignment_strategies(init_assignment_strategies_str);

    Stats stats;
    SolutionType solution_type;
    for (int i = 0; i < BRUTE_FORCE_TRIALS; ++i) {
        solution_type = handle_partition(partition, stats, init_assignment_strategies);
        if (solution_type != SolutionType::Unknown) break;
        if (Criteria::is_unsolvable_criteria_2_1(partition)) solution_type = SolutionType::Criterion_2_1;
    }
    return solution_type;
}


inline int FindNoSolutions::count_occurences_in_vec(const int& num, const std::vector<int> nums) {
    auto range = std::equal_range(nums.begin(), nums.end(), num);
    return range.second - range.first;
}


std::vector<int> FindNoSolutions::get_ks_around_ratio(const int& n, const double& ratio, const double& epsilon) {
    
    double dn = static_cast<double>(n);

    double lower = dn / (ratio + epsilon);
    double upper = dn / (ratio - epsilon);
    
    std::vector<int> ks;
    for (int k = ceil(lower); k <= floor(upper); k++) ks.push_back(k);

    return ks;
}


void FindNoSolutions::find_size_p_combinations_to_build_s  
        (const std::vector<int>& nums, 
        int rem_p, 
        int rem_s, 
        std::set<int>& path, 
        std::vector<std::set<int>>& res,
        int start) {

    int n = nums.size();
    long long coeff = binom_coeff(n,rem_p);
    if (coeff >= MAX_COMBINATIONS)  {
        std::cout << std::endl << coeff << std::endl;
        throw TooManyCombinations();
    }
        
    if (rem_p == 0 && rem_s == 0) {
        res.push_back(path);
        return;
    }
    
    if (rem_p == 0 || rem_s < 0) return;

    for (int i = start; i < (int)nums.size(); ++i) {
        if (nums[i] > rem_s) break;
        if ((int)nums.size() - i < rem_p) break;

        path.insert(nums[i]);
        find_size_p_combinations_to_build_s(nums, rem_p - 1, rem_s - nums[i], path, res, i+1);
        path.erase(nums[i]);

    }

    return;
}


std::vector<std::set<int>> FindNoSolutions::find_pow_of_p_independant_combinations_from_vec_of_sets 
    (const std::vector<std::set<int>>& vec_of_sets, const int& p, const int& pow) {
    
    std::vector<std::set<int>> pow_of_p_independant_combinations_from_vec_of_sets;


    int n = vec_of_sets.size();
    if (n == 0) return pow_of_p_independant_combinations_from_vec_of_sets;

    size_t p_mult_pow = p * pow;
    long long coeff = (binom_coeff(n,pow));
    if (coeff >= MAX_COMBINATIONS) {
        std::cout << std::endl << coeff << std::endl;
        throw TooManyCombinations();
    }


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

