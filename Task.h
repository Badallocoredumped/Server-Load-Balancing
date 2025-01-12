#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <vector>

using namespace std;

class Task
{
    private:
    vector<vector<double>> latencyMatrix;  // Matrix to store latency between servers and clients.
    vector<unsigned int> bandwithClients; // Bandwidth requirement for each client.
    vector<unsigned int> capacityServers; // Capacity of each server.
    unsigned int numServers;  // Number of servers.
    unsigned int numClients;  // Number of clients.

    public:
    Task(int NumServers, int numClients); // Constructor with number of servers and clients.
    Task(int NumServers, int numClients, vector<vector<double>>); // Constructor with a predefined latency matrix.
    Task(); // Default constructor.

    // Methods
    void clear(); // Resets the class attributes.
    void setLatency(int server, int client, double value); // Sets the latency value for a specific server-client pair.
    double getLatency(int server, int client); // Gets the latency value for a specific server-client pair.
    unsigned int getNumServers(); // Returns the number of servers.
    void setNumServers(int num); // Sets the number of servers.
    unsigned int getNumClients(); // Returns the number of clients.
    void setNumClients(int num); // Sets the number of clients.
    unsigned int getBandwith(int client); // Gets the bandwidth for a specific client.
    void setBandwith(int client, unsigned int bw); // Sets the bandwidth for a specific client.
    unsigned int getCapacity(int server); // Gets the capacity of a specific server.
    void setCapacity(int server, unsigned int cap); // Sets the capacity for a specific server.
    void inputMatrix(); // Inputs the latency matrix from the user.
    void printMatrix(); // Prints the latency matrix.
};

Task::Task()
{
    this->numServers = 0;
    this->numClients = 0;
}


Task::Task(int numServers, int numClients)
{
    this->numServers = numServers;
    this->numClients = numClients;
    this->latencyMatrix = vector<vector<double>>(numServers, vector<double>(numClients, 0));
    this->bandwithClients = vector<unsigned int>(this->numClients);
    this->capacityServers = vector<unsigned int>(this->numServers);
}

Task::Task(int numServers, int numClients, vector<vector<double>> matrix)
{
    this->numServers = numServers;
    this->numClients = numClients;
    this->latencyMatrix = matrix;
    this->bandwithClients = vector<unsigned int>(this->numClients);
    this->capacityServers = vector<unsigned int>(this->numServers);
}

void Task::inputMatrix()
{
    cout << "Enter the latency values between clients and servers:"<< endl;
    for (unsigned int i = 0; i < numServers; i++)
    {
        for (unsigned int j = 0; j < numClients; j++)
        {
            cin >> latencyMatrix[i][j];
            while (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a valid floating-point number.\n";
                cin >> latencyMatrix[i][j];
            }
        }
    }
}

void Task::printMatrix()
{
    cout << "The latency matrix is:" << endl;
    for (unsigned int i = 0; i < numServers; i++)
    {
        for (unsigned int j = 0; j < numClients; j++)
        {
            cout << latencyMatrix[i][j] << " ";
        }
        cout << endl;
    }
}
unsigned int Task::getNumServers()
{
    return numServers;
}

unsigned int Task::getNumClients()
{
    return numClients;
}

void Task::setNumServers(int num)
{
    if(num > 0)
    {
        this->numServers = num;
    }
    else
    {
        cout << "Invalid number of servers." << endl;
    }
}

void Task::setNumClients(int num )
{
    if(num > 0)
    {
        this->numClients = num;
    }
    else
    {
        cout << "Invalid number of clients." << endl;
    }
}

void Task::clear()
{
    numServers = 0;
    numClients = 0;
    latencyMatrix.clear();
}

void Task::setLatency(int server, int client, double value)
{
    if (client >= 0 && client < numClients && server >= 0 && server < numServers && value >= 0)
    {
        latencyMatrix[server][client] = value;
    }
    else
    {
        cout << "Invalid server or client number." << endl;
    }
}

double Task::getLatency(int server, int client)
{
    if(server < 0 || client < 0 || client >= this->numClients || server >= this->numServers){
        return -1;
    }
    return latencyMatrix[server][client];
}

unsigned int Task::getBandwith(int client){
    if(client >= 0 && client < this->numClients){
        return this->bandwithClients[client];
    }
    return 0;
}

unsigned int Task::getCapacity(int server){
    if(server >= 0 && server < this->numServers){
        return this->capacityServers[server];
    }
    return 0;
}

void Task::setBandwith(int client, unsigned int bw){
    if(client >= 0 && client < this->numClients){
        this->bandwithClients[client] = bw;
    }
}

void Task::setCapacity(int server, unsigned int cap){
    if(server >= 0 && server < this->numClients){
        this->capacityServers[server] = cap;
    }
}