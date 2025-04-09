#include "Partition.h"
#include "InitAssignmentStrategy.h"




enum class SolutionType {
    SolutionFound=0, NoSolutionTrivial=1, NoSolutionSlack=2,
    NoSolutionStrategy0=10,
    Unknown=100
};


struct Stats {
    long non_slack_unsolvable = 0, solved_by_greedy = 0, unsolved_by_greedy = 0, negative_slack = 0;
    double total_D_after_init = 0; // total part.D after solve
    double total_swaps = 0;
    int solution_count = 0;        // number of times init_assignment_strategy was called
};


SolutionType handle_partition (Partition& partition, Stats& stats, InitAssignmentStrategy& init_assignment_strategies);
