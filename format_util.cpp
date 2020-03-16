#include "format_util.h"
#include <cassert>
#include <map>
namespace {
    struct context{
			  typedef std::pair<int,int> key;
				typedef std::pair<double,std::vector<int>> value;
        //Data
        const std::vector<int>& word_lengths;
        int                     line_length;
				mutable std::map<key,value> cache;
        format_util::cost_function cost;
        //ctor
        context(const std::vector<int>& word_lengths_,
                int                     line_length_,
                format_util::cost_function cost_
                ):
        word_lengths(word_lengths_),line_length(line_length_), cost(cost_)
        {}
    };
}
static double min_cost(
    std::vector<int>* result,
    int               a,
    int               b,
		const context&    context
){
    
		assert(result);
    assert(0 <= a);
    assert(a <= b);
    double retval=0.0;
    int i=0;
		int sum = 0;
		auto val_ptr = context.cache.find({a,b});
		if (val_ptr != context.cache.end()){
			*result = val_ptr->second.second;
			retval = val_ptr->second.first;
			return retval;
		}
		for(i=a; i<=b;++i){
			sum += context.word_lengths[i];
			if (sum>context.line_length) break;
		}
		if(b == context.word_lengths.size() -1 && sum <= context.line_length){ // last line
			retval = 0.0;
			context.cache.insert({{a,b},{retval,std::vector<int>()}});
		}else if(sum <= context.line_length)	{// a to b fits line
			retval = context.cost(context.line_length - sum);
			context.cache.insert({{a,b},{retval,std::vector<int>()}});
		}else{ // need to break into several lines
			std::vector<int> lowest_left;
			std::vector<int> lowest_right;
			int lowest_k=0;
			double lowest_cost=0.0;
			const int first = a+1;
			
			for(int k = first; k<=b; ++k){
				std::vector<int> left;
				std::vector<int> right;
				double cost = min_cost(&left,a,k-1,context)+
				              min_cost(&right, k,b,context);
				if(k==first || cost<lowest_cost){
					lowest_cost = cost;
					lowest_k = k;
					lowest_left.swap(left);
					lowest_right.swap(right);
				}
			}
			result->swap(lowest_left);
			result->push_back(lowest_k);
			result->insert(result->end(),lowest_right.begin(),lowest_right.end());
			retval=lowest_cost;
			context.cache.insert({{a,b},{retval,*result}});
		}
		
    return retval;

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
	min_cost(result,0,word_lengths.size()-1,context);
}