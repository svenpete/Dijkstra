#include <iostream>
#include <thread>
#include "DijkstraPar.h"
#include "DijkstraSeq.h"
using namespace std;
int main(int argc, char *argv[])
{
    cout << "Hello Dijkstra's World!" << endl;

    auto filename = "USA-road-d.NY.gr";
    //auto filename = "rom.gr";

    auto par = DijkstraPar(filename);
    auto seq = DijkstraSeq(filename);

    par.getGraph();

    par.initDijkstra();

    par.doDijkstra();

    seq.getGraph();

    seq.initDijkstra();

    seq.doDijkstra();


    double speedUp = (seq.tsum / par.tsum);
    const auto coreCnt = std::thread::hardware_concurrency();
    double efficiency = speedUp / (coreCnt > 0 ? coreCnt : 1) * 100;

    cout << "\t\tSpeed-Up \t" << std::setprecision(3) << setw(5) << speedUp << "\n";
    cout << "\t\tEfficiency \t" << std::setprecision(3) << setw(5) << efficiency << " %\n";

    string input;
    do {
        cout << "Print hops to destination: ";
        getline(cin, input);
        cout << endl;
        int dest = input == "max" ? par.graph.size() - 1 : atoi(input.c_str());

    } while(input != "exit");

    return 0;
}