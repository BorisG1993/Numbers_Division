#include "trim_no_solutions.h"
#include "shlomo_src/Partition.h"
#include "shlomo_src/utils.h"
#include "utils.h"

#include <iostream>
#include <vector>
#include <string> 
#include <fstream>
#include <sstream>




int TrimNoSolutions::read_file(const std::string& filepath) {
    
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(file, line)) {
        this->to_trim.push_back(line);
    }

    file.close();

    return 0;
}


int TrimNoSolutions::write_to_file(const std::string& filepath) {

    std::ofstream file(filepath);
    if (!file) {
        std::cerr << "Error opening file: " << filepath << std::endl;
        return 1;
    }

    for (const std::string& prefix : this->trimmed) {
        file << prefix << std::endl;
    }
        
    std::cout << "Writing done" << std::endl;
    
    file.close();
    return 0;
}


Partition TrimNoSolutions::line_to_partition(const std::string& line) {
    Prefix prefix = build_p_from_line(line);
    return Partition(prefix.n, prefix.k, prefix.p);
}


std::string TrimNoSolutions::trimmed_partition_to_line(const std::vector<int>& partition_p, const int& criterion, const int& n = 0, const int& k = 0, const int& snk = 0) {

    std::stringstream ss;
    ss << "n=" << n << ", k=" << k << ", S=" << snk <<", p=[";

    int p_val_counter = 1;
    int current_p_val;

    for (size_t i = 0; i <partition_p.size();) {

        current_p_val = partition_p[i++];
        ss << current_p_val;
        
        while (i < partition_p.size() && partition_p[i] == current_p_val) {
            p_val_counter++;
            i++;
        }
    
        if (p_val_counter > 1) {
            ss << "^" << p_val_counter;
            p_val_counter = 1;
        }
            
        if (i == partition_p.size()) {
            ss << "], Criterion " << criterion;
        }

        else {
                ss << " ";
            }
        }

    return ss.str();
}


int TrimNoSolutions::trim_no_solutions(const std::string& src_filepath, const std::string& dst_filepath) {

    if (read_file(src_filepath) == 1) return 1;

    std::vector<int> current_prefix={}; 
    int trim_index;
    int criterion;

    for (auto it = to_trim.begin(); it != to_trim.end(); ++it) {

       trim_index = -1;
       criterion = 0;
//
//        if (!current_prefix.empty()) {
//            for (auto erase_it = it; erase_it != to_trim.end(); ) {
//                if (is_prefix(current_prefix,line_to_partition(*erase_it).p)) { 
//                    erase_it = to_trim.erase(erase_it);
//                }
//                else ++erase_it;
//            }
//        }
//        if (it == to_trim.end()) break;

        while (it != to_trim.end() && !current_prefix.empty() && is_prefix(current_prefix, line_to_partition(*it).p)) it++;
        if (it == to_trim.end()) break;

        Partition partition = line_to_partition(*it);

        trim_index = get_index_from_criterion_1(partition);
        if (trim_index != -1) criterion = 1;

        int trim_index_crit_2 = get_index_from_criterion_2(partition);
        if (trim_index_crit_2 != -1) {
            (trim_index  != -1 && trim_index < trim_index_crit_2) ? trim_index = trim_index : trim_index = trim_index_crit_2;
            if (criterion == 1) criterion = 12;
            else criterion = 2;
        }

        if (trim_index == -1) {
            trimmed.push_back("NAN");
            continue;
        }
        
        current_prefix = partition.p;
        current_prefix.erase(current_prefix.begin() + trim_index + 1, current_prefix.end());
        
        trimmed.push_back(trimmed_partition_to_line(current_prefix, criterion, partition.n, partition.k, partition.S));
    }

    return (write_to_file(dst_filepath) == 1);
}


// Shlomo src_code adopted
int TrimNoSolutions::get_index_from_criterion_1(const Partition& partition) {
    
    int default_index = - 1;
    partition.assert_valid();
    int n = partition.n, k = partition.k, S = partition.S;
    const std::vector<int>& p = partition.p;
    if (p[0] != 2 || p[k-1] == 2) return default_index; // Must be 2^num2 ... for num2 < k for the argument to work
    int num2 = 0;
    while (num2 < k && p[num2] == 2) num2++;
    assert(num2<k);
    int lb = S-n; // set U = {lb, ..., n} where each pair needs two U elements
    if (lb < 1) return default_index;
    int left_after_2 = (n-lb+1) - 2*num2;  // U elements left after populating the pairs
    assert(left_after_2 >= 0); // otherwise slack is violated

    int slack = 0; // slack using only lb-1, lb-2, ... on p[num2+left_after_2] ... p[k-1] must be >= 0
    int current_element = lb-1;
    for(int i=num2+left_after_2; i<k; i++) {
        slack += p[i]*(current_element + current_element-p[i]+1)/2 - S;
        if (slack < 0) return i;
        current_element -= p[i];
    }
    return default_index;
}


// Shlomo src_code adopted
int TrimNoSolutions::get_index_from_criterion_2(const Partition& partition) {
    
    int default_index = -1;
    partition.assert_valid();
    int n = partition.n, k = partition.k, S = partition.S;
    const std::vector<int>& p = partition.p;
    if (p[0] != 2 || p[k-1] == 2) return default_index; // Must be 2^num2 ... for num2 < k for the argument to work
    int num2 = 0;
    while (num2 < k && p[num2] == 2) num2++;
    assert(num2<k);
    int lb = S-n; // set U = {lb, ..., n} where each pair needs two U elements
    if (lb < 1) return default_index;
    int left_after_2 = (n-lb+1) - 2*num2;  // U elements left after populating the pairs
    assert(left_after_2 >= 0); // otherwise slack is violated
    if (left_after_2%2 == 0) return default_index; // is this condition actually needed?
    int midU = (lb+n)/2;
    int X = p[num2]; // the next element
    if (midU + sum_arithmetic(lb-1,-1,X-1) >= S) return default_index; // argument not going to work
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
            return num2 + numX - i;
    }
    int current_index = num2 + numX + num_U_elements_gt_mid + num_U_elements_leq_mid; // at this index U is exhausted
    if (current_index >= k) return default_index;
    int Y = p[current_index]; // width of first part after U is exhausted
    if (sum_arithmetic(lb-1,-1,Y) < S) return current_index;
    return default_index;
}


bool TrimNoSolutions::is_prefix(const std::vector<int>& vec1, const std::vector<int>& vec2) {

    if (vec1.size() > vec2.size()) return false;
    
    for (size_t i = 0; i < vec1.size(); i++) {
        if (vec1[i] != vec2[i]) return false;
    }
    return true;
}
