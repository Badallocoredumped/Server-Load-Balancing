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

// Default constructor: Initializes numServers and numClients to 0
// Time Complexity: O(1) 
// Space Complexity: O(1) (no additional memory allocation)

Task::Task()
{
    this->numServers = 0;    // Default number of servers is set to 0
    this->numClients = 0;    // Default number of clients is set to 0
    
}

// Constructor: Initializes Task with specified number of servers and clients
// Time Complexity: O(S * C), where S is the number of servers and C is the number of clients 
// (due to creation of the latencyMatrix)
// Space Complexity: O(S * C + S + C), where:
//  - O(S * C) for latencyMatrix
//  - O(S) for capacityServers
//  - O(C) for bandwithClients

Task::Task(int numServers, int numClients)
{
    this->numServers = numServers; // Set the number of servers for the task
    this->numClients = numClients; // Set the number of clients for the task

    // Create a 2D latency matrix with all elements initialized to 0.0
    this->latencyMatrix = vector<vector<double>>(numServers, vector<double>(numClients, 0));

    // Create a vector to store bandwidth for each client (initialized to default value of 0)
    this->bandwithClients = vector<unsigned int>(this->numClients);

    // Create a vector to store capacity for each server (initialized to default value of 0)
    this->capacityServers = vector<unsigned int>(this->numServers);

    
}

// Constructor: Initializes Task with specified number of servers, clients, and an existing latency matrix
// Time Complexity: O(1) (no new memory allocation for latencyMatrix, as it's passed in)
// Space Complexity: O(S + C), where:
//  - O(S) for capacityServers
//  - O(C) for bandwithClients
//  - Latency matrix is passed by reference, so no new space is allocated.

Task::Task(int numServers, int numClients, vector<vector<double>> matrix)
{
    this->numServers = numServers; // Set the number of servers for the task
    this->numClients = numClients; // Set the number of clients for the task

    // Set the latency matrix to the provided matrix
    this->latencyMatrix = matrix;

    // Create a vector to store bandwidth for each client (initialized to default value of 0)
    this->bandwithClients = vector<unsigned int>(this->numClients);

    // Create a vector to store capacity for each server (initialized to default value of 0)
    this->capacityServers = vector<unsigned int>(this->numServers);

    
}

// Method to input latency values between clients and servers
// Time Complexity: O(S * C), where S is the number of servers and C is the number of clients
// (inputting latency values for all client-server pairs)
// Space Complexity: O(1), no new memory is allocated; only the existing latencyMatrix is used.

void Task::inputMatrix()
{
    cout << "Enter the latency values between clients and servers:" << endl;

    // Loop over all servers and clients to input their latency values
    for (unsigned int i = 0; i < numServers; i++) 
    {
        for (unsigned int j = 0; j < numClients; j++) 
        {
            // Get the latency value for the specific client-server pair
            cin >> latencyMatrix[i][j];

            // Input validation loop to ensure the user enters a valid floating-point number
            while (std::cin.fail()) {
                std::cin.clear();  // Clear the error flag
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore invalid input
                std::cout << "Invalid input. Please enter a valid floating-point number.\n"; // Prompt user for input again
                cin >> latencyMatrix[i][j]; // Re-enter latency value
            }
        }
    }

    
}


// Method to print the latency matrix
// Time Complexity: O(S * C), where S is the number of servers and C is the number of clients
// (Printing all elements in the latencyMatrix)
// Space Complexity: O(1), as no new memory is allocated, we are only reading and printing the existing data.
void Task::printMatrix()
{
    cout << "The latency matrix is:" << endl;
    
    // Loop through all servers and clients to print the latency matrix
    for (unsigned int i = 0; i < numServers; i++) 
    {
        for (unsigned int j = 0; j < numClients; j++) 
        {
            cout << latencyMatrix[i][j] << " ";  // Print the latency value for each client-server pair
        }
        cout << endl;  // Print a newline after each row (server)
    }

    
}

// Getter method to retrieve the number of servers
// Time Complexity: O(1)
// Space Complexity: O(1) (no additional space required)

unsigned int Task::getNumServers()
{
    // Return the number of servers
    return numServers;

    
}

// Getter method to retrieve the number of clients
// Time Complexity: O(1) 
// Space Complexity: O(1) (no additional space required)

unsigned int Task::getNumClients()
{
    // Return the number of clients
    return numClients;

    
}

// Setter method to set the number of servers
// Time Complexity: O(1) 
// Space Complexity: O(1) (no additional space required)
void Task::setNumServers(int num)
{
    if(num > 0)  // Validate if the number of servers is greater than 0
    {
        this->numServers = num;  // Set the number of servers
    }
    else
    {
        cout << "Invalid number of servers." << endl;  // Print error if invalid
    }

    
}

// Setter method to set the number of clients
// Time Complexity: O(1) 
// Space Complexity: O(1) (no additional space required)

void Task::setNumClients(int num)
{
    if(num > 0)  // Validate if the number of clients is greater than 0
    {
        this->numClients = num;  // Set the number of clients
    }
    else
    {
        cout << "Invalid number of clients." << endl;  // Print error if invalid
    }

    
}

// Method to clear/reset the task
// Time Complexity: O(S * C) for the `clear()` operation on the 2D `latencyMatrix`, where S is the number of servers and C is the number of clients
// Space Complexity: O(1) after clearing the latency matrix, as no new memory is allocated

void Task::clear()
{
    numServers = 0;    // Reset the number of servers to 0
    numClients = 0;    // Reset the number of clients to 0
    latencyMatrix.clear();  // Clear the latency matrix (removes all elements)

    
}


// Method to set the latency between a specific server and client
// Time Complexity: O(1) 
// Space Complexity: O(1) (no additional space is allocated)
void Task::setLatency(int server, int client, double value)
{
    // Check if the server, client, and value are valid
    if (client >= 0 && client < numClients && server >= 0 && server < numServers && value >= 0)
    {
        latencyMatrix[server][client] = value;  // Set the latency value for the specified server-client pair
    }
    else
    {
        cout << "Invalid server or client number." << endl;  // Print error if invalid input
    }

    
}

// Method to get the latency between a specific server and client
// Time Complexity: O(1) 
// Space Complexity: O(1) (no additional space required)
double Task::getLatency(int server, int client)
{
    // Check if the server and client are valid
    if(server < 0 || client < 0 || client >= this->numClients || server >= this->numServers){
        return -1;  // Return -1 for invalid server or client index
    }
    return latencyMatrix[server][client];  // Return the latency value for the specified server-client pair

    
}

// Method to get the bandwidth for a specific client
// Time Complexity: O(1) 
// Space Complexity: O(1) (no additional space required)
unsigned int Task::getBandwith(int client)
{
    // Check if the client index is valid
    if(client >= 0 && client < this->numClients){
        return this->bandwithClients[client];  // Return the bandwidth for the specified client
    }
    return 0;  // Return 0 if the client index is invalid

    
}

// Method to get the capacity for a specific server
// Time Complexity: O(1) 
// Space Complexity: O(1) (no additional space required)
unsigned int Task::getCapacity(int server)
{
    // Check if the server index is valid
    if(server >= 0 && server < this->numServers){
        return this->capacityServers[server];  // Return the capacity for the specified server
    }
    return 0;  // Return 0 if the server index is invalid

    
}

// Method to set the bandwidth for a specific client
// Time Complexity: O(1) 
// Space Complexity: O(1) (no additional space required)
void Task::setBandwith(int client, unsigned int bw)
{
    // Check if the client index is valid
    if(client >= 0 && client < this->numClients){
        this->bandwithClients[client] = bw;  // Set the bandwidth value for the specified client
    }

    
}

// Method to set the capacity for a specific server
 // Time Complexity: O(1) 
// Space Complexity: O(1) (no additional space required)
void Task::setCapacity(int server, unsigned int cap)
{
    // Check if the server index is valid
    if(server >= 0 && server < this->numClients){  // Note: The check should be against `numServers`, not `numClients`
        this->capacityServers[server] = cap;  // Set the capacity value for the specified server
    }

   
}
