//
// Created by svenpete@id.hsos.de on 17.11.22.
//
#ifndef __DIJKSTRA_SEQ_H
#define __DIJKSTRA_SEQ_H

#include "Dijkstra.h"
#include <fstream>
#include <sstream>
#include <tbb/tbb.h>

using namespace std;
class DijkstraSeq : public Dijkstra
{
public:

    DijkstraSeq(std::string filename) : Dijkstra(filename, "Sequential") {}

    void getGraph() override
    {
        std::ifstream f(filename);
        std::string line;
        unsigned nodeCount = 0;
        while(std::getline(f, line)) {
            std::stringstream ss(line);
            std::string id;
            ss >> id;
            if(id == "a") {
                unsigned srcs;
                unsigned dest;
                unsigned arc;
                ss >> srcs >> dest >> arc;
                nodeCount = std::max(srcs, nodeCount);
                srcs--;
                dest--;
                if(nodeCount > graph.size()) {
                    graph.resize(nodeCount);
                }
                graph[srcs].push_back({
                                              .dest = dest,
                                              .dist = arc
                                      });
            }
        }
        cout << "Extracted nodes: " <<  to_string(graph.size()) << endl;
    }

    void initDijkstra() override
    {
        dist.resize(graph.size());
        prev.resize(graph.size());
        mrkd.resize(graph.size());
        for(size_t i = 0; i < graph.size(); i++) {
            dist[i] = UINT_MAX;
            prev[i] = 0;
            mrkd[i] = false;
        }
        if(dist.size() > 0)
            dist[0] = 0;
    }

    void doDijkstra() override
    {
        for(unsigned i = 0; i < graph.size(); i++) {
            int minDistIdx = getMinDist();
            std::vector<Arc> targetNode = graph[minDistIdx];
            mrkd[minDistIdx] = true;
            for(size_t j = 0; j < targetNode.size(); j++) {
                unsigned destIdx = targetNode[j].dest;
                unsigned relDist = targetNode[j].dist;
                if(!mrkd[destIdx]) {
                    unsigned absDist = dist[minDistIdx] + relDist;
                    if(absDist < dist[destIdx]) {
                        dist[destIdx] = absDist;
                        prev[destIdx] = minDistIdx;
                    }
                }
            }
        }
    }

    int getMinDist() override
    {
        unsigned min = UINT_MAX;
        int idx = -1;
        for(size_t i = 0; i < dist.size(); i++) {
            if(!mrkd[i] && dist[i] < min) {
                min = dist[i];
                idx = i;
            }
        }
        return idx;
    }
};

#endif // __DIJKSTRA_SEQ_H