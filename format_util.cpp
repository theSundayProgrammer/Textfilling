#include "format_util.h"
#include <cassert>
#include <map>
#include <iostream>
namespace {
	typedef std::pair<int,int> index_type;
	struct solution {
		int start;
		int fin;
		index_type left;
		index_type right;
		double cost;
		solution():start(-1){}
	};
    struct context{
        //Data
        const std::vector<int>& word_lengths;
        int                     line_length;
		typedef std::vector<solution> Solution;
		mutable std::vector<Solution> cache;
        format_util::cost_function cost;
        //ctor
        context(const std::vector<int>& word_lengths_,
                int                     line_length_,
                format_util::cost_function cost_
                ):
        word_lengths(word_lengths_),line_length(line_length_), cost(cost_),
		cache(word_lengths_.size())
        {
			int N=word_lengths.size();
			for(auto& arr: cache) arr.resize(N--);
		}
		 double get_cost(index_type const& index) const
		 {
			 return get_val(index).cost;
		 }
		const solution& get_val(index_type const& index) const
		{
			int start_, fin_;
			std::tie(start_,fin_) = index;
			return	cache[start_][fin_-start_];

		}
		index_type find(int start_, int fin_) const {
			if(cache[start_][fin_-start_].start == -1) 
				return std::make_pair(-1,-1);
			else 
				return std::make_pair(start_,fin_);
		}
		index_type add(solution const& sol) const {
			int start_ = sol.start;
			int fin_   = sol.fin;
			cache[start_][fin_-start_] = sol;
			return std::make_pair(start_,fin_);
		}
    };
}
index_type  min_cost(
    int               a,
    int               b,
	const context&    context
){
    assert(0 <= a);
    assert(a <= b);
	//if(a==0)	std::cerr << a << "," << b << std::endl;

    int i=0;
	int sum = 0;
	auto val_ptr = context.find(a,b);
	if (val_ptr.first != -1){
		return val_ptr;
	}
	for(i=a; i<=b;++i){
		sum += context.word_lengths[i];
		if (sum>context.line_length) break;
	}
	if(b == context.word_lengths.size() -1 && sum <= context.line_length){ // last line
		solution retval;
		retval.cost = 0.0;
		retval.left = std::make_pair(-1,-1);
		retval.right = std::make_pair(-1,-1);
		retval.start = a;
		retval.fin = b;
		return context.add(retval);
	
	}else if(sum <= context.line_length)	{// a to b fits line
		solution retval;
		retval.cost = context.cost(context.line_length - sum);
		retval.left = std::make_pair(-1,-1);
		retval.right = std::make_pair(-1,-1);
		retval.start = a;
		retval.fin = b;
		return context.add(retval);
	}else if(a==b){
		solution retval;
		retval.cost = 0.0;
		retval.left = std::make_pair(-1,-1);
		retval.right = std::make_pair(-1,-1);
		retval.start = a;
		retval.fin = b;
		return context.add(retval);


	}else{ // need to break into several lines
		index_type lowest_left{-1,-1};
		index_type lowest_right{-1,-1};
		double lowest_cost=0.0;
		const int first = a+1;
		
		for(int k = first; k<=b; ++k){
			index_type left=min_cost(a,k-1,context);
			index_type right = min_cost(k,b,context);
			double cost = context.get_cost(left)+ context.get_cost(right);
			if(k==first || cost<lowest_cost){
				lowest_cost = cost;
				lowest_left = left;
				lowest_right= right;
			}
		}
		solution retval;
		retval.cost = lowest_cost;
		retval.left = lowest_left;
		retval.right = lowest_right;
		retval.start = a;
		retval.fin = b;
		return context.add(retval);
		
	}
		
    return std::make_pair(-1,-1);

}
void traverse_tree(context const& context_, solution const& sol, std::vector<int>* result)
{
	if (sol.left.first!=-1){
		traverse_tree(context_, context_.get_val(sol.left),result);
		traverse_tree(context_, context_.get_val(sol.right),result);
	} else {
		result->push_back(sol.fin);
	}

}
void format_util::calculate_optimal_partition(
	std::vector<int>* result,
	const std::vector<int>& word_lengths,
	int line_length,
	format_util::cost_function cost 
){
	context context(word_lengths,line_length,cost);
	assert(result);
	assert(line_length > 0);
	auto index = min_cost(0,word_lengths.size()-1,context);
	traverse_tree(context, context.get_val(index),result);

}