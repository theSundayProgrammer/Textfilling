#include "format_util.h"
#include <cassert>
#include <map>
#include <iostream>
namespace {
	struct solution {
		int start;
		int fin;
		solution* left;
		solution* right;
		double cost;
		solution():start(-1){}
	};
    struct context{
		typedef std::pair<int,int> key;
		typedef solution* value;
        //Data
        const std::vector<int>& word_lengths;
        int                     line_length;
		mutable solution *cache;
        format_util::cost_function cost;
        //ctor
        context(const std::vector<int>& word_lengths_,
                int                     line_length_,
                format_util::cost_function cost_
                ):
        word_lengths(word_lengths_),line_length(line_length_), cost(cost_)
        {
			cache = new solution[word_lengths.size()*(word_lengths.size() + 1)/2];
		}
		
		~context(){ delete [] cache;}
		int get_index(int start, int fin) const {
			return start*(2*word_lengths.size() -start + 1) /2 + (fin - start);
		}

		solution* find(int start, int fin) const {
			int index = get_index(start,fin);
			if(cache[index].start == -1) return nullptr;
			else return cache+index;
		}
		solution *add(solution const& sol) const {
			int index = get_index(sol.start,sol.fin);
			*(cache + index) = sol;
			return cache+index;
		}
    };
}
static solution* min_cost(
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
	if (val_ptr != nullptr){
		return val_ptr;
	}
	for(i=a; i<=b;++i){
		sum += context.word_lengths[i];
		if (sum>context.line_length) break;
	}
	if(b == context.word_lengths.size() -1 && sum <= context.line_length){ // last line
		solution retval;
		retval.cost = 0.0;
		retval.left = nullptr;
		retval.right = nullptr;
		retval.start = a;
		retval.fin = b;
		return context.add(retval);
	
	}else if(sum <= context.line_length)	{// a to b fits line
		solution retval;
		retval.cost = context.cost(context.line_length - sum);
		retval.left = nullptr;
		retval.right = nullptr;
		retval.start = a;
		retval.fin = b;
		return context.add(retval);
	}else if(a==b){
		solution retval;
		retval.cost = 0.0;
		retval.left = nullptr;
		retval.right = nullptr;
		retval.start = a;
		retval.fin = b;
		return context.add(retval);


	}else{ // need to break into several lines
		solution* lowest_left=nullptr;
		solution* lowest_right=nullptr;
		double lowest_cost=0.0;
		const int first = a+1;
		
		for(int k = first; k<=b; ++k){
			solution* left=min_cost(a,k-1,context);
			solution* right = min_cost(k,b,context);
			double cost = left->cost + right->cost;
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
		
    return nullptr;

}
void traverse_tree(solution *sol, std::vector<int>* result)
{
	if (sol->left){
		traverse_tree(sol->left,result);
		traverse_tree(sol->right,result);
	} else {
		result->push_back(sol->fin);
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
	solution *sol = min_cost(0,word_lengths.size()-1,context);
	traverse_tree(sol,result);

}