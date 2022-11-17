//
// Created by svenpete@id.hsos.de on 17.11.22.
//
#ifndef __DIJKSTRA_H
#define __DIJKSTRA_H

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <chrono>
#include <iterator>
#include <vector>
#include <algorithm>

using namespace std::chrono;
using namespace std;
typedef struct
{
    unsigned dest;
    unsigned dist;
} Arc;

class Dijkstra
{
public:
    string filename;
    vector<std::vector<Arc>> graph;
    vector<unsigned> dist;
    vector<unsigned> prev;
    vector<bool> mrkd;
    string tag;
    double tsum;
    std::chrono::time_point<std::chrono::steady_clock> tstart;
    std::chrono::time_point<std::chrono::steady_clock> tend;

    Dijkstra(std::string filename, std::string tag)
            : filename(filename), tag(tag), tsum(0) {}

    virtual void getGraph() = 0;
    virtual void initDijkstra() = 0;
    virtual void doDijkstra() = 0;
    virtual int getMinDist() = 0;



};

#endif // __DIJKSTRA_H#ifndef __DIJKSTRA_H
