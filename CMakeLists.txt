cmake_minimum_required(VERSION 3.23)
project(Dijkstra)

set(CMAKE_CXX_STANDARD 17)

add_executable(Dijkstra main.cpp Dijkstra.h DijkstraPar.h DijkstraSeq.h)
find_package("TBB")
target_link_libraries(Dijkstra TBB::tbb)