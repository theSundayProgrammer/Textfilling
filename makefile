nwc_format : application.o format_util.o
	g++ -std=c++17 -o $@ -g -pthread  $^ -lstdc++fs

application.o : application.cpp
	g++ -std=c++17 -c  -g -std=c++17 -pthread -I /home/chakra/projects/CPP/boost_1_70_0/ $< 

format_util.o : format_util.cpp
	g++ -std=c++17 -c -g  -std=c++17 -pthread -I /home/chakra/projects/CPP/boost_1_70_0/ $< 

clean : 
	rm nwc_format application.o format_util.o