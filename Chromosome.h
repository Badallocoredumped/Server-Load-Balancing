#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

struct Chromosome
{
    vector<vector<double>> ServerAllocations; //{Server1 -> Client1,Client2..., 
                                           //Server2 -> Client1, Client2};

    double latencyScore;

    double penaltyCapacity;
    double penaltyBandwith;
    double connectionPen;

    bool isFeas;

    double fitness;
};