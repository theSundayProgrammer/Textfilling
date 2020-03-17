/// application.cpp
#include "format_util.h"
#include <vector>
#include <iostream>
#include <cctype>
#include <cstddef>
#include <cassert>
double cost(int lagging_spaces){
    assert(lagging_spaces  >=0);
    double val = lagging_spaces;
    return val*val*val;
}

int load_word(std::string* result){
    assert(result);
    char c = ' ';
    while(std::cin && isspace(c)){
        std::cin.get(c);
    }
    if (!std::cin){
        return -1;
    }
    result->clear();
    while(std::cin && !isspace(c)){
        result->push_back(c);
        std::cin.get(c);
    }
    return 0;

}

int main(int argc, char* argv[]){
    const int max_line_length = argc > 1 ? std::atoi(argv[1]): 79;
    std::vector<std::string>    words;
    std::vector<int>            word_lengths;
    std::string word;
    while(load_word(&word)==0){
        words.push_back(word);
        word_lengths.push_back(word.length()+1);
    }
    std::vector<int>            result;
    format_util::calculate_optimal_partition(&result,word_lengths,max_line_length,cost);
    int start =0;
    
    for(auto fin:result){
        for(int i=start; i<fin; ++i){
            std::cout << words[i] << ' ';
        }
        std::cout << "\n";
        start = fin;
    }
    for (auto it= words.begin()+start; it!=words.end();++it){
        std::cout << *it << ' ';
    }
    std::cout<<std::endl;
    return 0;
    
}
