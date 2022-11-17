#include <iostream>
#include <thread>
#include "DijkstraParrallel.h"

using namespace std;
int main(int argc, char *argv[])
{
    cout << "Hello Dijkstra's World!" << endl;

    auto filename = "/home/svenpete/CLionProjects/Dijkstra/USA-road-d.NY.gr";


    auto par = DijkstraParrallel(filename);


    par.GraphHolen();

    par.erstellDijk();

    par.execDijk();
    

    string input;
    do {
        cout << "Print hops to destination: ";
        getline(cin, input);
        cout << endl;
        int dest = input == "max" ? par.graph.size() - 1 : atoi(input.c_str());
        if(dest > 0)
            par.printHops(dest);
    } while(input != "exit");

    return 0;
}