#include "InitAssignmentStrategy.h"
#include "PartitionGenerator.h"
#include "PartitionGenerator_test.h"
#include "Partition_test.h"
#include "utils.h"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>

using namespace std;

// Returns i for an (i,i+1) swap and *change which is <0, ==0, >0 for improving, neutral and bad swaps.
int get_transposition(const Partition &part, int *change) {
    assert(part.is_complete());
    int neutral_i_vec[part.n], bad_i_vec[part.n];
    int num_neutral_i = 0, num_bad_i = 0;
    const int offset = random_uniform(0,part.n-1);
    for(int index=0; index < part.n - 1; index++) {
        int i = (index + offset) % (part.n - 1) + 1;
        int j1 = part.assignment(i);
        int j2 = part.assignment(i+1);
        if (j1 == j2) continue;
        int s1 = part.part_sum(j1);
        int s2 = part.part_sum(j2);
        // change = (s1-s+1)^2 + (s2-s-1)^2 - (s1-s)^2 - (s2-s)^2
        //        = 2*(s1-s) + 1 - 2*(s2-s) + 1
        //        = 2*(1 + s1 - s2)
        int ch = 2 * (1 + s1 - s2);
        if (ch < 0) {
            *change = ch;
            return i;
        }
        else if (ch == 0)
            neutral_i_vec[num_neutral_i++] = i;
        else
            bad_i_vec[num_bad_i++] = i;
    }
    if (num_neutral_i > 0) {
        *change = 0;
        return neutral_i_vec[random_uniform(0, num_neutral_i - 1)];
    }
    assert(num_bad_i > 0);
    *change = 1;
    return bad_i_vec[random_uniform(0,num_bad_i-1)];
}

int transpositions(Partition &part) {
    assert(part.is_complete());
    const int MAX_PERTURBATIONS = 100;
    const int MAX_NEUTRAL = 1000;
    int swap_count = 0;
    int NeutralSteps = 0, PerturbationSteps = 0;
    while (part.diff() > 0 && PerturbationSteps < MAX_PERTURBATIONS) {
        if (NeutralSteps++ < MAX_NEUTRAL) {
            int change;
            int chosen_i = get_transposition(part, &change);
            if (change < 0)
                NeutralSteps = 0;
            part.swap(chosen_i, chosen_i + 1);
            swap_count++;
        }
        else { // perturbation
            const int PERTURBATION_LENGTH = static_cast<int>(sqrt(part.k)+1);
            for (int c = 0; c < PERTURBATION_LENGTH; c++) {
                int chosen_i = random_uniform(1, part.n - 1);
                part.swap(chosen_i, chosen_i + 1);
                swap_count++;
            }
            PerturbationSteps++;
            NeutralSteps = 0;
        }
    }
    return swap_count;
}

#if 0
// True means there is no solution. False means we don't know.
// 1st strategy - too many X-tuples whose sum is smaller that S using only elements not in U
// Subset of is_unsolvable_strategy2
bool is_unsolvable_strategy1(const Partition &part) {
    part.assert_valid();
    int n = part.n, k = part.k, S = part.S;
    const vector<int>& p = part.p;
    if (p[0] != 2 || p[k-1] == 2) return false; // Must be 2^num2 ... for num2 < k for the argument to work
    int num2 = 0;
    while (num2 < k && p[num2] == 2) num2++;
    int lb = S-n; // set U = {lb, ..., n} where each pair needs two U elements
    if (lb < 1) return false;
    int left_after_2 = (n-lb+1) - 2*num2;  // U elements left after populating the pairs
    assert(left_after_2 >= 0); // otherwise slack is violated
    assert(num2<k);
    // 1st strategy - too many X-tuples whose sum is smaller that S using only elements not in U
    int X = 0; // maximal tuple width that cannot sum up to S using only element < lb
    while (X<lb && (lb*X - (1+X)*X/2) < S) X++;
    X--;
    int num_X = 0; // number of elements after 2^num2 that are <= X
    while (num2+num_X < k && p[num2+num_X] <= X) num_X++;
    int small_Xlets = num_X - left_after_2; // number of Xlets guarantied not to have an element of {lb,...,n}
    return (small_Xlets > 0);
}
#endif

// True means there is no solution. False means we don't know.
// slack not using U
// Criterion 1 from the paper
bool is_unsolvable_strategy2(const Partition &part) {
    part.assert_valid();
    int n = part.n, k = part.k, S = part.S;
    const vector<int>& p = part.p;
    if (p[0] != 2 || p[k-1] == 2) return false; // Must be 2^num2 ... for num2 < k for the argument to work
    int num2 = 0;
    while (num2 < k && p[num2] == 2) num2++;
    assert(num2<k);
    int lb = S-n; // set U = {lb, ..., n} where each pair needs two U elements
    if (lb < 1) return false;
    int left_after_2 = (n-lb+1) - 2*num2;  // U elements left after populating the pairs
    assert(left_after_2 >= 0); // otherwise slack is violated

    int slack = 0; // slack using only lb-1, lb-2, ... on p[num2+left_after_2] ... p[k-1] must be >= 0
    int current_element = lb-1;
    for(int i=num2+left_after_2; i<k; i++) {
        slack += p[i]*(current_element + current_element-p[i]+1)/2 - S;
        if (slack < 0) return true;
        current_element -= p[i];
    }
    return false;
}

#if 0
// True means there is no solution. False means we don't know.
// 3rd strategy - 3^num3 where one element <= midU
// Subset of is_unsolvable_strategy2 + 4
bool is_unsolvable_strategy3(const Partition &part) {
    part.assert_valid();
    int n = part.n, k = part.k, S = part.S;
    const vector<int>& p = part.p;
    if (p[0] != 2 || p[k-1] == 2) return false; // Must be 2^num2 ... for num2 < k for the argument to work
    int num2 = 0;
    while (num2 < k && p[num2] == 2) num2++;
    assert(num2<k);
    int lb = S-n; // set U = {lb, ..., n} where each pair needs two U elements
    if (lb < 1) return false;
    int left_after_2 = (n-lb+1) - 2*num2;  // U elements left after populating the pairs
    assert(left_after_2 >= 0); // otherwise slack is violated
    if (left_after_2%2 == 0) return false; // is this condition actually needed?
    int midU = (lb+n)/2;
    if (midU+(lb-1)+(lb-2) >= S) return false; // argument not going to work
    int num3 = 0;
    while (num2 + num3 <k && p[num2 + num3] == 3) num3++;
    int num_U_elements_gt_mid = left_after_2/2;
    int num_U_elements_leq_mid = (left_after_2+1)/2;
    for(int i=num3; i>0; i--) {
        if (num_U_elements_gt_mid > 0)
            num_U_elements_gt_mid--;
        else if (num_U_elements_leq_mid >= 2)
            num_U_elements_leq_mid -= 2;
        else
            return true;
    }
    int current_index = num2 + num3 + num_U_elements_gt_mid + num_U_elements_leq_mid; // at this index U is exhausted
    if (current_index >= k) return false;
    int X = p[current_index]; // width of first part after U is exhausted
    return (((lb-1)+(lb-X))*X/2 < S);
}
#endif

inline int sum_arithmetic(int first_element, int step, int num_elements) {
    int sum=0;
    for(int i=0, elem = first_element; i<num_elements; i++, elem += step)
        sum += elem;
    int result = num_elements*(2*first_element+step*(num_elements-1))/2;
    assert(result == sum);
    return result;
}

// True means there is no solution. False means we don't know.
// Criterion 2 from the paper
bool is_unsolvable_strategy4(const Partition &part) {
    part.assert_valid();
    int n = part.n, k = part.k, S = part.S;
    const vector<int>& p = part.p;
    if (p[0] != 2 || p[k-1] == 2) return false; // Must be 2^num2 ... for num2 < k for the argument to work
    int num2 = 0;
    while (num2 < k && p[num2] == 2) num2++;
    assert(num2<k);
    int lb = S-n; // set U = {lb, ..., n} where each pair needs two U elements
    if (lb < 1) return false;
    int left_after_2 = (n-lb+1) - 2*num2;  // U elements left after populating the pairs
    assert(left_after_2 >= 0); // otherwise slack is violated
    if (left_after_2%2 == 0) return false; // is this condition actually needed?
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

Partition partition_from_vec(const vector<int>& p) {
    int n = 0;
    for(int i:p) { n += i; assert(i>=1); }
    int k = p.size();
    assert(k>=1);
    return Partition(n,k,p);
}

struct Stats {
    long non_slack_unsolvable = 0, solved_by_greedy = 0, unsolved_by_greedy = 0, negative_slack = 0;
    double total_D_after_init = 0; // total part.D after solve
    double total_swaps = 0;
    int solution_count = 0;        // number of times init_assignment_strategy was called
};

ostream& operator<<(ostream& os, const Stats& stats) {
    os << "unsolved_by_greedy=" << stats.unsolved_by_greedy
       << ", solved_by_greedy=" << stats.solved_by_greedy
       << ",  non_slack_unsolvable=" << stats.non_slack_unsolvable
       << ", negative_slack=" <<  stats.negative_slack;
    os << ", avg_diff_after_init=" << stats.total_D_after_init/stats.solution_count
       << ", avg_swaps=" << stats.total_swaps/stats.solution_count;
    return os;
}

enum class SolutionType {
    SolutionFound=0, NoSolutionTrivial=1, NoSolutionSlack=2,
    NoSolutionStrategy0=10,
    Unknown=100
};

static SolutionType SolutionTypeFromStrategies(int num) {
    return static_cast<SolutionType>(static_cast<int>(SolutionType::NoSolutionStrategy0) + num);
}

ostream& operator<<(ostream& os, SolutionType solution_type) {
    switch(solution_type) {
        case SolutionType::SolutionFound: return os << "SolutionFound";
        case SolutionType::NoSolutionTrivial: return os << "NoSolutionTrivial";
        case SolutionType::NoSolutionSlack: return os << "NoSolutionSlack";
        case SolutionType::Unknown: return os << "Unknown";
        default:
            assert(solution_type >= SolutionType::NoSolutionStrategy0 && solution_type < SolutionType::Unknown);
            return os << "NoSolutionStrategy"
                      << (static_cast<int>(solution_type) - static_cast<int>(SolutionType::NoSolutionStrategy0));
    }
}

SolutionType handle_partition(Partition &part, Stats &stats, InitAssignmentStrategy& init_assignment_strategy) {
//    if (part.k >= 2 && part.p[1] == 1) return SolutionType::NoSolutionTrivial;
    if (part.slack() < 0) { stats.negative_slack++; return SolutionType::NoSolutionSlack; }
    int un = (is_unsolvable_strategy2(part) ? 2 : 0)
           + (is_unsolvable_strategy4(part) ? 8 : 0);
    if (un > 0) {
        stats.non_slack_unsolvable++;
        return SolutionTypeFromStrategies(un);
    }
    for(init_assignment_strategy.reset(); init_assignment_strategy; ++init_assignment_strategy) {
        part.clear_assignment();
        init_assignment_strategy(part);
        stats.total_D_after_init += part.diff();
        stats.solution_count++;
        stats.total_swaps += transpositions(part);
        if (part.diff() == 0) {
            stats.solved_by_greedy++;
            part.assert_valid();
            assert(part.diff_brute_force() == 0);
            for(int j=0; j < part.k; j++) {
                assert(part.part_size(j) == part.p[j]);
                assert(part.part_sum(j) == part.S );
            }
            return SolutionType::SolutionFound;
        }
    }
    stats.unsolved_by_greedy++;
    return SolutionType::Unknown;
}

// Partitions specified by a string array - array[0], ..., array[len-1]
// where each is either the part size or part size "^" multiplicity
// e.g. {"2^25", "3^2", "4", "6", "14"}  will return the corresponding length 30 partition.
static vector<int> PartitionFromStringArray(int len, char **array) {
    vector<int> p;
    while (len-- > 0) {
        char c;
        int i = -1, count = 1;
        istringstream(*array++) >> i >> c >> count;
        assert(i > 0);
        p.insert(p.end(), count, i);
    }
    return p;
}

// Get one line from stream and parse in a a partition. Return true if line read successfully.
// e.g. line = 2^25 3^3 4 6 14 set p to the corresponding length 30 partition and returns true.
static bool PartitionFromStream(istream& is, vector<int>& p) {
    string line;
    if (!getline(is, line)) return false;
    p.clear();
    auto line_is = istringstream(line);
    string word;
    while (line_is >> word) {
        char c;
        int i = -1, count = 1;
        istringstream(word) >> i >> c >> count;
        assert(i > 0);
        p.insert(p.end(), count, i);
    }
    return true;
}

void print_result_line(const Partition& part, const Stats& stats, SolutionType solution_type) {
    cout << "n=" << part.n << ", k=" << part.k << ", S=" << part.S
         << ", p=[" << compress_run_length(part.p) << "]"
         << ", " << solution_type << endl;
}

// Examples with is_valid(n, k) and slack() >= 0
//    n=80, k=30, p=[2^26 4^2 6 14] -- SolutionType::NoSolution
//    n=80, k=30, p=[2^24, 3^4, 6, 14] -- SolutionType::Unknown currently but should be SolutionType::NoSolution
//    n=80, k=30, p=[2^19 3^5 4^2 5^2 8 16] -- SolutionType::NoSolution
//    n=80, k=30, p=[2^25 3^2 5^2 14] -- SolutionType::Unknown currently but should be SolutionType::SolutionFound
//    n=39, k=13, p=[2^8 3^3 5 9 ] -- SolutionType::SolutionFound
int main(int argc, char **argv) {
//    test_partition();
//    test_partition_generator();
    string init_assignment_strategies_str = "GRD,RND,WS,RND/10,GRD";
    unsigned seed = time(nullptr);

    while (argc > 2) {
        if (argv[1] == string("-s"))
            istringstream(argv[2]) >> seed;
        else if (argv[1] == string("-i"))
            init_assignment_strategies_str = argv[2];
        else
            break;
        argc -= 2;
        argv += 2;
    }
    srand(seed);
    InitAssignmentStrategy init_assignment_strategies(init_assignment_strategies_str);
    cout << "init_assignment_strategies = " << init_assignment_strategies << endl;
    if (argc > 1 && argv[1] == string("-p")) {
        vector<int> p = PartitionFromStringArray(argc-2, argv+2);
        Partition part = partition_from_vec(p);
        Stats stats;
        SolutionType solution_type = handle_partition(part, stats, init_assignment_strategies);
        print_result_line(part, stats, solution_type);
        part.print_assignment(cout);
        return 0;
    } else if (argc == 3 && argv[1] == string("-f")) {
        ifstream is(argv[2]);
        vector<int> p;
        Stats stats;
        clock_t start_time = clock();
        while (PartitionFromStream(is, p)) {
            Partition part = partition_from_vec(p);
            SolutionType solution_type = handle_partition(part, stats, init_assignment_strategies);
            if (false /* solution_type > SolutionType::NoSolutionSlack */)
                print_result_line(part, stats, solution_type);
        }
        cout << double(clock() - start_time) / CLOCKS_PER_SEC << " sec" << ", " << stats << endl;
        return 0;
    }
    if (argc < 3 || argc > 4) {
        cerr << "USAGE: partition [-s seed] [-i init_assignment_strategies] min_n max_n [n/k lb]\n"
                "or     partition [-s seed] [-i init_assignment_strategies] -p 2^26 4^2 6 14\n"
                "or     partition [-s seed] [-i init_assignment_strategies] -f partitions_file.txt" << endl;
        exit(1);
    }
    int min_n=1, max_n = 75; // default
    istringstream(argv[1]) >> min_n;
    istringstream(argv[2]) >> max_n;
    double a_lb = 0;
    if (argc > 3) istringstream(argv[3]) >> a_lb;
    for (int n = min_n; n <= max_n; n++) {
        Stats stats;
        clock_t start_time = clock();
        for (int k = 2; k < n; k++) {
	    if ((double)n/k < a_lb) continue;
            if (Partition::is_valid(n, k)) {
//                int S = n * (n + 1) / (2 * k);
                int min_p = 2; // 1 + (S > n);
                for (const auto& p : PartitionGenerator(n,k,min_p)) {
                    Partition part(n, k, p);
                    SolutionType solution_type = handle_partition(part, stats, init_assignment_strategies);
                    if (solution_type > SolutionType::NoSolutionSlack)
                        print_result_line(part, stats, solution_type);
                }
            }
        }
        cout << "n=" << n << ": " << double(clock() - start_time) / CLOCKS_PER_SEC << " sec" << ", " << stats << endl;
        cerr << "n=" << n << ": " << double(clock() - start_time) / CLOCKS_PER_SEC << " sec" << ", " << stats << endl;
    }
    return 0;
}
