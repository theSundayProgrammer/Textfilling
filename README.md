# Text-filling
Text-filling problem with different cost functions 
based on on example of the same name in the book
"Large Scale C++- Volume I" John Lakos, Addison Wesley
Boston 2020

The master branch is essentially the same as the source code in pages 65-67
except 
1. using stream iterator rather than the load function
2. using C++11 features such as auto ande lambda
3. the function parameters  for min_cost is 
    * vector<int> *result, // same as LSC - V1
    * start_index, // same as LSC - V1
    * fin_index, // one past the index as in STL iterators
    * connst Context&, // same as LSC V1

The mapped_version branch contains the program that is outlined as fixed size nodes except we use
1. one long fixed length array in tag v1.2
2. vector of vectors that are all pre-allocated as soon as Context is constructed in tag v1.3

