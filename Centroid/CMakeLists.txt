cmake_minimum_required(VERSION 2.8)
project( Centroid )                                 #replace test
find_package( OpenCV REQUIRED )
add_executable( Centroid Centroid.cpp )                 #replace test and test.cpp
target_link_libraries( Centroid ${OpenCV_LIBS} )    #replace test