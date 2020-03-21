/// application.cpp
#include "format_util.h"
#include <vector>
#include <iostream>
#include <cctype>
#include <cstddef>
#include <cassert>
#include <iterator>
double cost(int lagging_spaces){
    assert(lagging_spaces  >=0);
    double val = lagging_spaces;
    return val*val*val;
}

int main(int argc, char* argv[]){
    const int max_line_length = argc > 1 ? std::atoi(argv[1]): 79;
    std::vector<std::string>    words;
    std::vector<int>            word_lengths;
    std::istream_iterator<std::string> eos;
    std::istream_iterator<std::string> iit(std::cin);
    for(;iit!=eos;++iit){
        word_lengths.push_back(iit->length() +1);
        words.push_back(*iit);
    }
    std::vector<int>            result;
    format_util::calculate_optimal_partition(
        &result,
        word_lengths,
        max_line_length,
        [](int x){return double(x*x*x);}
        );
    int i =0;
    result.push_back(words.size());
    for(auto fin:result){
        for(; i<fin; ++i){
            std::cout << words[i] << ' ';
        }
        std::cout << "\n";
    }
    return 0;
    
}
