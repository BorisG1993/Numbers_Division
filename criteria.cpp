#include "criteria.h"




inline int sum_arithmetic(int first_element, int step, int num_elements) {
int sum=0;
for(int i=0, elem = first_element; i<num_elements; i++, elem += step)
    sum += elem;
int result = num_elements*(2*first_element+step*(num_elements-1))/2;
assert(result == sum);
return result;
}


// True means there is no solution. False means we don't know.
// Criterion 2.1  - extension to criterion 2 from the paper
// Works only for next couple of ps' after 2
bool Criteria::is_unsolvable_criteria_2_1(const Partition &part) {
    part.assert_valid();
    int n = part.n, k = part.k, S = part.S;
    const std::vector<int>& p = part.p;
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
    int numX = 0;
    while (num2 + numX < k && p[num2 + numX] == X) numX++;
    int nextX = p[num2 + numX];
    int numNextX = 0;
    while (num2 + numX + numNextX < k && p[num2 + numX + numNextX] == nextX) numNextX++;
    
    // the main difference from criterion 2.0 - cuts lb if nessesary by *next* X
    int lb_after_cut = cut_lb(nextX, numNextX, lb, part);
    if (lb_after_cut > 0) lb = lb_after_cut;
    else numNextX = 0;

    if (midU + sum_arithmetic(lb-1,-1,X-1) >= S) return false; // argument not going to work
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
    int current_index = num2 + numX + numNextX + num_U_elements_gt_mid + num_U_elements_leq_mid; // at this index U is exhausted
    if (current_index >= k) return false;
    int Y = p[current_index]; // width of first part after U is exhausted
    return (sum_arithmetic(lb-1,-1,Y) < S);
}


// returns new lb if next X forces use of lower nums and pow of next X is not exhausted, else returns -1
int Criteria::cut_lb(const int& nextX, int& numNextX, const int& lb, const Partition& part) {
    int iter = 1;
    int acc = 0;
    for (int i = lb-1; i > 0 && iter < numNextX; i-=nextX) {
        acc += sum_arithmetic(i, -1, nextX);
        if (acc == (part.S)*iter) {
            numNextX -= (numNextX-iter);
            return i-nextX;
        }
        iter++;
    }
    return -1;
}


