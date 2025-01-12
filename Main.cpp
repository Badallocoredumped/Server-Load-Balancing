#include "GeneticAlgorithm.h"
#include <iostream>
#include <vector>

using namespace std;

int main()
{
    const unsigned int numClients = 6; //Specify client number
    const unsigned int numServers = 4; //Specify server number


    //The structure of the latency matrix is {Server1 -> Client1,Client2...,Clientn}
    //                                        Server2 -> Client1, Client2....,Clientn};
    //Do not enter 0 or negative values! It will cause an error
    vector<vector<double>> desiredLatencyMatrix = {
    {2.3, 4.5, 2.0, 5.0, 15.0, 25.5},
    {5.0, 10.0, 15.0, 20.0, 30.0, 6.0},
    {5.0, 10.0, 10.0, 20.0, 35.0, 42.0},
    {2.3, 4.5, 2.0, 40.0, 15.0, 50.0},
    {2.3, 4.5, 20.0, 5.0, 15.0, 25.5}
                    };

    Task task1(numServers, numClients, desiredLatencyMatrix); //Construct the task

    //Takes input from user for each clients TC: O(n) SC:(n) n number of clients
    for(int i = 0; i<numClients; i++){
        int a;
        cout << "Enter Bandwith of Client "<<i+1<< endl;
        cout << "(Do not enter 0 or negative values!)" << endl;
        cin >> a;
        task1.setBandwith(i, a);    
    }
    
        //Takes input from user for each clients TC: O(n) SC:(n) n number of servers
    for(int i = 0; i<numServers; i++){
        int a;
        cout << "Enter Capacity of Server "<<i+1 << endl;
        cout << "(Do not enter 0 or negative values!)" << endl;
        cin >> a;
        task1.setCapacity(i, a);    
    }

    //Genetic Algorithm main5
    geneticAlgorithm(task1);
    
    return 0;
}