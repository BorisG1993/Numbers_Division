#include <limits>
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "InitAssignmentStrategy.h"
#include "utils.h"

using namespace std;

InitAssignmentStrategy::InitAssignmentStrategy(const std::string& strategies) {
   size_t pos = 0, new_pos = 0;
   while ((new_pos = strategies.find(',',pos)) != string::npos) {
       append_strategy(strategies.substr(pos, new_pos-pos));
       pos = new_pos + 1;
   }
   append_strategy(strategies.substr(pos, string::npos));
   _it = _strategies.begin();
}

void InitAssignmentStrategy::append_strategy(const std::string &strategy) {
    string s = strategy;
    int repeat = 1;
    auto pos = strategy.find('/');
    if (pos != string::npos) {
        s = strategy.substr(0,pos);
        istringstream(strategy.substr(pos+1)) >> repeat;
    }
    while (repeat--) {
        if (s == "WS")        _strategies.push_back(whole_set_populate_assignment);
        else if ( s == "RR")  _strategies.push_back(round_robin_populate_assignment);
        else if ( s == "RND") _strategies.push_back(random_populate_assignment);
        else if ( s == "GRD") _strategies.push_back(adaptive_populate_assignment);
        else {
            cerr << "InitAssignmentStrategy: unknown strategy " << s << endl;
            exit(1);
        }
    }
}

void InitAssignmentStrategy::Print(std::ostream& os) const {
    vector<string> v;
    for(InitStrategy s : _strategies) {
        if (s == whole_set_populate_assignment) v.push_back("WS");
        else if (s == round_robin_populate_assignment) v.push_back("RR");
        else if (s == random_populate_assignment) v.push_back("RND");
        else if (s == adaptive_populate_assignment) v.push_back("GRD");
        else v.push_back("UNK");
    }
    os << '"' << compress_run_length(v,'/',',') << '"';
}

void InitAssignmentStrategy::whole_set_populate_assignment(Partition &part) {
   int i = part.n, j = 0;
    while (i > 0) {
        assert(j < part.k);
        if (part.part_size(j) < part.p[j])
            part.assign(i--, j);
        else
            j++;
    }
    assert(part.is_complete());
    part.assert_valid();
}

void InitAssignmentStrategy::round_robin_populate_assignment(Partition &part) {
    int j = 0;
    for(int i=part.n; i > 0; i--) {
        assert(j < part.k);
        while (part.part_size(j) == part.p[j])
            j = (j+1) % part.k;
        part.assign(i, j);
        j = (j+1) % part.k;
    }
    assert(part.is_complete());
    part.assert_valid();
}

void InitAssignmentStrategy::random_populate_assignment(Partition &part) {
    whole_set_populate_assignment(part);
    for(int i=1; i<part.n; i++)
        part.swap(i,random_uniform(i,part.n));
    part.assert_valid();
}

void InitAssignmentStrategy::adaptive_populate_assignment(Partition &part) {
    for(int i=part.n; i>=1; i--) {
        int best_j = -1;
        float max_average = numeric_limits<float>::lowest();
        for(int j=0; j<part.k; j++) {
            if (part.part_size(j) == part.p[j]) continue;
            if (part.part_size(j) == part.p[j]-1 && i <= part.S-part.part_sum(j)) {
                best_j = j;
                break;
            }
            float average = (part.S-part.part_sum(j)-i)/max(1.0f, part.p[j] - part.part_size(j) - 1.0f);
            if (average > max_average) {
                max_average = average;
                best_j = j;
            }
        }
        assert(best_j != -1);
        part.assign(i,best_j);
    }
    assert(part.is_complete());
}



    


