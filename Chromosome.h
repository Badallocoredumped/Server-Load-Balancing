#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;


// Struct representing a Chromosome in the population
struct Chromosome
{
    // 2D vector to store the server allocations. It represents the allocation of clients
    // Server1 -> Client1, Client2, ... 
    // Server2 -> Client1, Client2, ...
    vector<vector<double>> ServerAllocations;

    // Latency score of the chromosome.
    double latencyScore;

    // Penalty associated with the server's capacity. This is the penalty incurred
    // if the server exceeds its capacity due to client allocations.
    double penaltyCapacity;

    // Penalty associated with bandwidth usage. This penalty is applied when the 
    // total bandwidth usage exceeds the available bandwidth for a client.
    double penaltyBandwith;

    // Connection penalty. If servers gets 0 mbps it adds penalty
    double connectionPen;

    // Boolean indicating whether the chromosome's configuration is feasible.
    // If all constraints (capacity, bandwidth, connection) are satisfied, 
    // this value will be true; otherwise, it will be false.
    bool isFeas;

    // Fitness score of the chromosome. This is the evaluation metric used to determine how good the solution is
    double fitness;
};