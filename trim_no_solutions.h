#ifndef TRIM_NO_SOLUTIONS_H
#define TRIM_NO_SOLUTIONS_H

#include <vector>
#include <string>

#include "utils.h"
class Partition;


class TrimNoSolutions {

private:
    
    std::vector<std::string> to_trim;
    std::vector<std::string> trimmed;
    
    int read_file(const std::string& filepath);
    int write_to_file(const std::string& filepath);
    Partition line_to_partition(const std::string& line);
    bool is_prefix(const std::vector<int>& vec1, const std::vector<int>& vec2);

    // utils
    inline int sum_arithmetic(int first_element, int step, int num_elements);

    int get_index_from_criterion_1(const Partition& partition);
    int get_index_from_criterion_2(const Partition& partition);


public:

    TrimNoSolutions(){}
    int trim_no_solutions(const std::string& src_filepath, const std::string& dst_filepath);

};

#endif //TRIM__NO_SOLUTIONS_H
