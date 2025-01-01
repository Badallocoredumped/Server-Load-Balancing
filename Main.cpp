#include "GeneticAlgorithm.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    //20 15
    //5 7 10 
    //10 20 15 25 10 30
    const unsigned int numClients = 2;
    const unsigned int numServers = 3;
    Task task1(numServers, numClients);
    
    for(int i = 0; i<numServers; i++){
        int a;
        cout << "Enter Capacity:" << endl;
        cin >> a;
        task1.setCapacity(i, a);    
    }

    for(int i = 0; i<numClients; i++){
        int a;
        cout << "Enter Bandwith:" << endl;
        cin >> a;
        task1.setBandwith(i, a);    
    }

    task1.inputMatrix();
    
    geneticAlgorithm(task1);
    
    return 0;
}