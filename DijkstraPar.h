//
// Created by svenpete@id.hsos.de on 17.11.22.
//
#ifndef __DIJKSTRA_PAR_H
#define __DIJKSTRA_PAR_H

#include "Dijkstra.h"
#include <fstream>
#include <sstream>
#include <tbb/tbb.h>

class DijkstraPar : public Dijkstra
{
public:

    DijkstraPar(std::string filename) : Dijkstra(filename, "Parallel") {}

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
                unsigned srcs, dest, arc;
                ss >> srcs >> dest >> arc;
                nodeCount = std::max(srcs, nodeCount);
                srcs--;
                dest--;
                if(nodeCount > graph.size())
                    graph.resize(nodeCount);
                graph[srcs].push_back({
                                              .dest = dest,
                                              .dist = arc
                                      });
            }
        }
        cout << "Extracted nodes: " << std::to_string(graph.size()) << endl;
    }

    void initDijkstra() override
    {
        dist.resize(graph.size());
        prev.resize(graph.size());
        mrkd.resize(graph.size());
        tbb::parallel_for(tbb::blocked_range<size_t>(0, graph.size()),
                          [this](tbb::blocked_range<size_t> &r) {
                              for(size_t i = r.begin(); i != r.end(); i++) {
                                  dist[i] = UINT_MAX;
                                  prev[i] = 0;
                                  mrkd[i] = false;
                              }
                          }
        );
        if(dist.size() > 0)
            dist[0] = 0;
    }

    void doDijkstra() override
    {
        for(unsigned i = 0; i < graph.size(); i++) {
            int minDistIdx = getMinDist();
            std::vector<Arc> targetNode = graph[minDistIdx];
            mrkd[minDistIdx] = true;
            tbb::parallel_for(tbb::blocked_range<size_t>(0, targetNode.size(), 1e4),
                              [this, &targetNode, &minDistIdx](tbb::blocked_range<size_t> &r) {
                                  for(size_t j = r.begin(); j != r.end(); j++) {
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
            );
        }
    }

    int getMinDist() override
    {
        unsigned min = UINT_MAX;
        int idx = -1;
        tbb::concurrent_vector<std::pair<int, unsigned>> localMins;
        tbb::parallel_for(tbb::blocked_range<size_t>(0, dist.size(), 1e3),
                          [this, &localMins](tbb::blocked_range<size_t> &r) {
                              unsigned _min = UINT_MAX;
                              int _idx = -1;
                              for(size_t i = r.begin(); i < r.end(); i++) {
                                  if(!mrkd[i] && dist[i] < _min) {
                                      _min = dist[i];
                                      _idx = i;
                                  }
                              }
                              if(_idx >= 0)
                                  localMins.push_back(std::make_pair(_idx, _min));
                          }
        );
        for(size_t i = 0; i < localMins.size(); i++) {
            if(localMins[i].second < min) {
                min = localMins[i].second;
                idx = localMins[i].first;
            }
        }
        return idx;
    }
};

#endif // __DIJKSTRA_PAR_H