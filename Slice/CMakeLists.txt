cmake_minimum_required(VERSION 2.8)
project( Slice )                                 #replace test
find_package( OpenCV REQUIRED )
add_executable( Slice Slice.cpp )                 #replace test and test.cpp
target_link_libraries( Slice ${OpenCV_LIBS} )    #replace test