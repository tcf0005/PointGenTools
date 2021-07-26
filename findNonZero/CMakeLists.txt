cmake_minimum_required(VERSION 2.8)
project( fNZ )                                 #replace test
find_package( OpenCV REQUIRED )
add_executable( fNZ fNZ.cpp )                 #replace test and test.cpp
target_link_libraries( fNZ ${OpenCV_LIBS} )    #replace test