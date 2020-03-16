/// format_util.h
#pragma once
#include <vector>
struct format_util
{
    /* data */
    /*types*/
    typedef double (*cost_function)(int);
    static void calculate_optimal_partition(
        std::vector<int>*       result,
        const std::vector<int>& word_lengths,
        int                     line_length,
        cost_function           cost
    );

};
