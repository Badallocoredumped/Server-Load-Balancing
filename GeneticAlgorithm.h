// Genetic Algorithm Implementation
// Constants and Macros:
// - POPULATION: Size of the population.
// - GENERATIONS: Number of generations.
// - PENALTY_METHOD: Type of penalty method used (1 -> static).
// - PENALTY_CONSTANT: Penalty constant for constraint violations.
// - ELITISM: Boolean to determine whether elitism is used.
// - SELECTION_METHOD: Selection method (1 -> binary tournament).
// - CROSSOVER_METHOD: Crossover method (1 -> BLX-alpha, 2 -> SBX).
// - CROSSOVER_ALPHA: Alpha value for BLX-alpha crossover (range: 0 to 1).
// - CROSSOVER_ETA: Eta value for SBX crossover (higher values -> more exploitation).
// - crossoverProbability: Probability of crossover.
// - MUTATION_METHOD: Mutation method (1 -> random mutation).
// - mutationProbability: Probability of mutation.

#include "Task.h"
#include "Chromosome.h"
#include <algorithm>
#include <random>
#include <cmath>
#include <iomanip> 

#define POPULATION 1000
#define GENERATIONS 2000
#define PENALTY_METHOD 1 // 1 -> static
#define PENALTY_CONSTANT 100 // The initial penalty constant for constraints
#define ELITISM true // true -> elitist_full, false -> non_elitist
#define SELECTION_METHOD 1 // 1 -> binary tournament
#define CROSSOVER_METHOD 2 // 1 -> BLX-alpha, 2 -> SBX
#define CROSSOVER_ALPHA 0.5 // BLX-alpha crossover parameter
#define CROSSOVER_ETA 5 // SBX crossover parameter
#define crossoverProbability 0.9 // Probability of crossover
#define MUTATION_METHOD 1 // 1 -> random
#define mutationProbability 0.2 // Probability of mutation
#define Verbose true // set true to see all the logs

using namespace std;

vector<unsigned int> upperBounds; // Upper bounds for allocation constraints
Task current1; // Current task being solved

// Compare function for sorting chromosomes based on fitness
// Time Comp: O(1)
// Space Comp: O(1)
bool compareByFitness(const Chromosome& a, const Chromosome& b) 
{
    return a.fitness < b.fitness;
}

// Function prototypes
void findUpperBound();
Chromosome generateIndividual();
vector<Chromosome> generateRandomPopulation();
void evaluate(vector<Chromosome>&);
vector<Chromosome> selection(vector<Chromosome>&);
void variation(vector<Chromosome>&);
void crossover(Chromosome&, Chromosome&);
double blxaCrossover(double, double, int);
void sbxCrossover(Chromosome&, Chromosome&, int, int);
void mutation(Chromosome&);
vector<Chromosome> survivor(vector<Chromosome>&, vector<Chromosome>&);
void printPopulation(vector<Chromosome>&);
void calculatePenalty(Chromosome&);
void printIndividual(Chromosome&);

// Survivor selection based on elitism -> elitist_full and non-elitist
// Time Comp: O(POP*logn(POP)) 
// Space Comp: O(1)
vector<Chromosome> survivor(vector<Chromosome>& offspring, vector<Chromosome>& parentPop)
{
    vector<Chromosome> newParent;
    if (ELITISM) {

        // Sort both offspring and parent population by fitness so then we can combine them into one
        sort(offspring.begin(), offspring.end(), compareByFitness); // O(POP * log(POP)) sorting algorithmn
        sort(parentPop.begin(), parentPop.end(), compareByFitness); // O(POP * log(POP)) sorting algorithmn

        int offspringIndx = 0;
        int ParentIndx = 0;
        for (int i = 0; i < POPULATION; i++) { //TC: O(POP) SC: O(POP) 
            if (offspring[offspringIndx].fitness < parentPop[ParentIndx].fitness) {
                newParent.push_back(offspring[offspringIndx++]);
            } else {
                newParent.push_back(parentPop[ParentIndx++]);
            }
        }
    } else {
        newParent = offspring; // Non-elitist approach
    }
    return newParent;
}

// Performs crossover between two chromosomes (off1 and off2).
// Supports two crossover methods:
// CROSSOVER_METHOD == 1 BLX-alpha (Blend Crossover Alpha): Generates offspring by blending values with a factor determined by alpha.
// CROSSOVER_METHOD == 2 SBX (Simulated Binary Crossover): Mimics binary crossover in continuous search spaces.
// Time Comp: O(S*C) S stands for server number, C stands for client number
// Space Comp: O(1) all the things passed by reference
void crossover(Chromosome& off1, Chromosome& off2) {
    if (CROSSOVER_METHOD == 1) {
        // BLX-alpha crossover
        // Iterate each server and client combination
        for (int i = 0; i < current1.getNumServers(); i++) {
            for (int j = 0; j < current1.getNumClients(); j++) {

                // Generate two new values for offspring using the BLX-alpha method
                double newVal1 = blxaCrossover(off1.ServerAllocations[i][j], off2.ServerAllocations[i][j], j);
                double newVal2 = blxaCrossover(off1.ServerAllocations[i][j], off2.ServerAllocations[i][j], j);
                // Assign the new values to the offspring
                off1.ServerAllocations[i][j] = newVal1;
                off2.ServerAllocations[i][j] = newVal2;
            }
        }
    } else if (CROSSOVER_METHOD == 2) {

        // SBX (Simulated Binary Crossover)
        // Iterate each server and client combination
        for (int i = 0; i < current1.getNumServers(); i++) {
            for (int j = 0; j < current1.getNumClients(); j++) {

                // Apply SBX crossover
                sbxCrossover(off1, off2, i, j);
                sbxCrossover(off1, off2, i, j);
            }
        }
    }
}

// SBX Crossover
// Time Complexity: O(1)
// Space Complexity: O(1)
void sbxCrossover(Chromosome& off1, Chromosome& off2, int index1, int index2) {
    double x1 = off1.ServerAllocations[index1][index2];
    double x2 = off2.ServerAllocations[index1][index2];

    double k = ((double)rand() / RAND_MAX); // Generate random value [0, 1]
    double beta;

    if (k <= 0.5) {
        beta = pow(2 * k, 1.0 / (CROSSOVER_ETA + 1));
    } else {
        beta = pow(1.0 / (2 * (1 - k)), 1.0 / (CROSSOVER_ETA + 1));
    }

    //Formulation of SBX crossover
    double y1 = 0.5 * ((1 + beta) * x1 + (1 - beta) * x2); 
    double y2 = 0.5 * ((1 - beta) * x1 + (1 + beta) * x2);

    // Clamp values to valid bounds
    y1 = max(min(y1, (double)upperBounds[index2]), 0.0);
    y2 = max(min(y2, (double)upperBounds[index2]), 0.0);


    //Assigns new values to proper genes
    off1.ServerAllocations[index1][index2] = y1;
    off2.ServerAllocations[index1][index2] = y2;
}

// BLX-alpha Crossover
// Time Complexity: O(1)
// Space Complexity: O(1)
double blxaCrossover(double p1, double p2, int clientNum) {
    double minVal = min(p1, p2); //Find the min one
    double maxVal = max(p1, p2); //Find the max one
    double u = ((double)rand() / RAND_MAX); //generate random number between 0 to 1
    double gamma = ((1.0 + 2.0 * CROSSOVER_ALPHA) * u) - CROSSOVER_ALPHA; //gamma formulation
    double off = ((1.0 - gamma) * minVal) + (gamma * maxVal); //find new offspring values
    off = max(min(off, (double)upperBounds[clientNum]), 0.0); //bound check
    return off;
}


// Function to apply mutation on a chromosome
// Time Complexity: O(S*C) C stands for number of server, C stands for number of clients
// Space Complexity: O(1) constant
void mutation(Chromosome& off1) {
    if (MUTATION_METHOD == 1) {
        for (int i = 0; i < off1.ServerAllocations.size(); i++) {
            for (int j = 0; j < off1.ServerAllocations[0].size(); j++) {

                // Randomly mutate gene based on a 50% probability
                if (rand() % 2 == 1) {
                    off1.ServerAllocations[i][j] = rand() % upperBounds[j];
                }
            }
        }
    }
}

// Function to select parents for mating using tournament selection
// Time Complexity: O(POP) POP stands for size of population
// Space Complexity: O(POP) POP stands for size of population, it creates matingPool
vector<Chromosome> selection(vector<Chromosome>& pop) {
    vector<Chromosome> mating;
    if (SELECTION_METHOD == 1) {
        for (int i = 0; i < pop.size(); i++) {
            // Select two random chromosomes and pick the one with better fitness
            int index1 = rand() % pop.size();
            int index2 = rand() % pop.size();
            pop[index1].fitness < pop[index2].fitness ? mating.push_back(pop[index1]) : mating.push_back(pop[index2]);
        }
    }
    return mating;
}

// Function to apply crossover and mutation on offspring
// Time Complexity: O(POP * (S*C)) S*C is time complexity of mutation, POP stands for population size
// Space complexity: O(1)
void variation(vector<Chromosome>& offspring) {
    for (int i = 0; i < POPULATION; i += 2) {
        // Apply crossover with a given probability
        if (rand() % 100 < crossoverProbability * 100) {
            crossover(offspring[i], offspring[i + 1]); //TC: O(S*C)
        }

        // Apply mutation to the first and second offspring with given probabilities
        if (rand() % 100 < mutationProbability * 100) {
            mutation(offspring[i]); //TC: O(S*C)
        }
        if (rand() % 100 < mutationProbability * 100) {
            mutation(offspring[i + 1]); //TC: O(S*C)
        }
    }
}

// Function to generate a random individual
// Time Complexity: O(S*C) S stands for server number, C stands for client number
// Space Complexity: O(S*C) it creates a matrix respect to to number of server and clients
Chromosome generateIndividual() {
    Chromosome individual;
    //randomly generate individual respect to bounds
    individual.ServerAllocations = vector<vector<double>>(current1.getNumServers());
    for (int i = 0; i < current1.getNumServers(); i++) {
        individual.ServerAllocations[i] = vector<double>(current1.getNumClients());
        for (int j = 0; j < current1.getNumClients(); j++) {
            individual.ServerAllocations[i][j] = rand() % upperBounds[j];
        }
    }
    return individual;
}

// Function to generate a random population
// Time Complexity: O(POP * S*C) POP is population size S stands for server number, C stands for client number
// Time Complexity: O(POP * S*C) POP is population size S stands for server number, C stands for client number
vector<Chromosome> generateRandomPopulation() {
    vector<Chromosome> ans;
    for (int i = 0; i < POPULATION; i++) {
        ans.push_back(generateIndividual()); //TC: O(S*C)
    }
    return ans;
}

// Function to print an individual's server allocations and fitness details
// Time Complexity: O(S * C). S stands for server number, C stands for client number 
// Space Complexity: O(1).
void printIndividual(Chromosome& individual)
 {
    // Check if the solution is feasible
    if (individual.isFeas) {
        cout << "Feasible Solution" << endl;
    } else {
        cout << "Infeasible Solution" << endl;
    }

    cout << "\n";
    cout << setw(15) << left << "Servers";  
    for (int i = 0; i < current1.getNumClients(); i++) 
    {
        
        cout << setw(10) << left << ("C" + to_string(i + 1));
    }
    cout << endl;
    cout << string(15 + current1.getNumClients() * 10, '-') << endl;

    // Print allocations for each server
    for (int i = 0; i < current1.getNumServers(); i++) 
    {
        cout << setw(15) << left << ("Server " + to_string(i + 1));  // Server labels with extra space
        for (int j = 0; j < current1.getNumClients(); j++) 
        {
            // Print the allocation for each server-client pair
            cout << setw(10) << left << individual.ServerAllocations[i][j];
        }
        cout << endl;
    }

    // Add an extra line for better visual separation
    cout << "\n";
}


// Main genetic algorithm function
// Time Complexity: O(G * (P * S * C)), where G is the number of generations, P is population size, S stands for server number, C stands for client number
// Space Complexity: O(P * S * C).  P is population size, S stands for server number, C stands for client number
void geneticAlgorithm(Task task1) {
    srand(time(NULL));
    current1 = task1;
    findUpperBound(); // TC: O(C) C stands for clients
    vector<Chromosome> parentPop = generateRandomPopulation();
    evaluate(parentPop); // TC:O(P * S * C). P is population size, S stands for server number, C stands for client number

    for (int i = 0; i < GENERATIONS; i++) {
        vector<Chromosome> offspring = selection(parentPop); //TC: O(POP) POP stands for size of population
        variation(offspring); //Time Complexity: O(POP * (S*C))
        evaluate(offspring);
        parentPop = survivor(offspring, parentPop); //  TC:O(P * S * C). P is population size, S stands for server number, C stands for client number

        if(Verbose){
            //Print the individual
            cout << endl;
            cout << "Generation " << i + 1 << " Current Best: " << endl;
            printIndividual(parentPop[0]);
            cout << "Fitness = " << parentPop[0].fitness << endl;
            cout << "LatencyScore = " << parentPop[0].latencyScore << endl;
            cout << "Penalty Capacity: " << parentPop[0].penaltyCapacity << endl;
            cout << "Penalty Bandwith: " << parentPop[0].penaltyBandwith << endl;
        }
    }
    // Print the best solution found
    cout << "Best: " << endl;
    printIndividual(parentPop[0]);
    cout << "Fitness = " << parentPop[0].fitness << endl;
}



// Function to calculate the latency score for a chromosome
// Time Complexity: O(S * C). S stands for server number, C stands for client number 
// Space Complexity: O(1) 
void calculateLatencyScore(Chromosome& individual) {
    double latencyScore = 0.0;

    for (int i = 0; i < current1.getNumClients(); i++) {
        double sum = 0.0;
        double latencyTotal = 0.0;
        for (int j = 0; j < current1.getNumServers(); j++) {
            latencyTotal += (1.0 / current1.getLatency(j, i)); //Calculate the total latcen with reverse of respective latency matrix cell
        }

        for (int j = 0; j < current1.getNumServers(); j++) {
            //This is the main objective function to minimize
            latencyScore += abs(current1.getBandwith(i) * (1.0 / current1.getLatency(j, i)) - (individual.ServerAllocations[j][i] * latencyTotal));
        }
    }
    individual.latencyScore = latencyScore; //assign the fitness to individual
}

//Penalty calculation (Constraints)
// Time Complexity: O(S * C). S stands for server number, C stands for client number 
// Space Complexity: O(1) 
void calculatePenalty(Chromosome& indi){

    double capacityPen = 0;
    double bandwithPen = 0;
    double connectionPen = 0;

    for(int i = 0; i<current1.getNumServers(); i++){
        unsigned int sum = 0;
        for(int j = 0; j<current1.getNumClients(); j++){
            if(indi.ServerAllocations[i][j] == 0){ //If serverAlloc is 0 assign connection penalty
                connectionPen += PENALTY_CONSTANT;
            }
            sum += indi.ServerAllocations[i][j];
        }
        if(sum > current1.getCapacity(i)){ //If all the allocation sum greater than server capacity assign capacity penalty
            capacityPen += (sum-current1.getCapacity(i))*PENALTY_CONSTANT;
        }
    }
    
    for(int i = 0; i<current1.getNumClients(); i++){

        unsigned int sum = 0;
        for(int j = 0; j<current1.getNumServers(); j++){
            sum += indi.ServerAllocations[j][i];
        }
        if(sum > current1.getBandwith(i)){ //If all the allocation sum greater than server capacity assign capacity penalty
            bandwithPen += (sum-current1.getBandwith(i)*PENALTY_CONSTANT);
        }
    }

    indi.penaltyCapacity = capacityPen;
    indi.penaltyBandwith = bandwithPen;
    indi.connectionPen = connectionPen;
}

//Evaluation Phase of the genetic algorithm
// Time Comp: O(POP* (S*C)), POP stands for population size, S stands for server number, C stands for client number 
// Space Comp: O(1) every elements are passed by reference
void evaluate(vector<Chromosome>& pop)
{
    // Iterate through the entire population
    for(int i = 0; i < POPULATION; i++) 
    {
        // Calculate the latency score for the current chromosome
        calculateLatencyScore(pop[i]); 

        // Calculate the penalty values
        calculatePenalty(pop[i]); 

        // Compute the fitness score by combining latency score and penalties
        pop[i].fitness = pop[i].latencyScore + PENALTY_CONSTANT * (pop[i].penaltyCapacity + pop[i].penaltyBandwith + pop[i].connectionPen);

        // Check if the chromosome is feasible (if there is no penalties)
        if(pop[i].penaltyCapacity + pop[i].penaltyBandwith + pop[i].connectionPen == 0) {
            pop[i].isFeas = true;
        } else {
            pop[i].isFeas = false;
        }
    }
}


//Finds upper bounds for each clients
//Time Complexity: O(C) C stands for number of clients
//Space Complexity: O(C) it adds c elements to upperBounds vector
void findUpperBound(){
    for(int i = 0; i<current1.getNumClients(); i++){
        upperBounds.push_back(0);
        if(current1.getBandwith(i) > upperBounds[i]){
            upperBounds[i] = current1.getBandwith(i);
        }
    }
}