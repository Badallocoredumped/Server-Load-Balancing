/* 
Initialize Population: Create an initial population of chromosomes.

Evaluate Fitness: Calculate the fitness of each chromosome in the population.

Selection: Select parent chromosomes based on their fitness.

Crossover: Combine parent chromosomes to produce offspring.

Mutation: Introduce random changes to some offspring.

Replacement: Replace the old population with the new one.

Termination: Define a termination condition (e.g., a maximum number of generations or a satisfactory fitness level). 

*/
#include "Task.h"
#include "Chromosome.h"
#include <algorithm>
#include <cmath>
#define POPULATION 500
#define GENERATIONS 350
#define crossoverProbability 0.9
#define mutationProbability 0.2


unsigned int upperBound = 0;

Task current1;

bool compareByFitness(const Chromosome& a, const Chromosome& b) 
{
    return a.fitness < b.fitness;
}


void findUpperBound();
Chromosome generateIndividual();
vector<Chromosome> generateRandomPopulation();
void evaluate(vector<Chromosome>&);
vector<Chromosome> selection(vector<Chromosome>&);
void variation(vector<Chromosome>&);
void crossover(Chromosome&, Chromosome&);
void mutation(Chromosome&);
vector<Chromosome> survivor(vector<Chromosome>&, vector<Chromosome>&);
void printPopulation(vector<Chromosome>&);
void calculatePenalty(Chromosome&);
void printIndividual(Chromosome&);


vector<Chromosome> survivor(vector<Chromosome>& offspring, vector<Chromosome>& parentPop)
{
    vector<Chromosome> newParent;
    sort(offspring.begin(), offspring.end(), compareByFitness);
    sort(parentPop.begin(), parentPop.end(), compareByFitness);

    int offspringIndx = 0;
    int ParentIndx = 0;
    for(int i = 0; i<POPULATION; i++)
    {
        if(offspring[offspringIndx].fitness < parentPop[ParentIndx].fitness)
        {
            newParent.push_back(offspring[offspringIndx++]);
        }
        else
        {
            newParent.push_back(parentPop[ParentIndx++]);
        }
    }
    return newParent;
}

void crossover(Chromosome& off1, Chromosome& off2)
{
    int crossoverPoint = rand() % (current1.getNumServers() + current1.getNumClients());
    for(int i = 0; i<current1.getNumServers(); i++)
    {
        for(int j = 0; j<current1.getNumClients(); j++)
        {
            if(crossoverPoint < i+j){
                int temp = off1.ServerAllocations[i][j];
                off1.ServerAllocations[i][j] = off2.ServerAllocations[i][j];
                off2.ServerAllocations[i][j] = temp;
            }
        }
    }
    
}

void mutation(Chromosome& off1){
    //Random mutation
    for(int i = 0; i<off1.ServerAllocations.size(); i++){
        for(int j = 0; j<off1.ServerAllocations[0].size(); j++){ 
            if(rand() % 2 == 1){
                off1.ServerAllocations[i][j] = rand() % upperBound;
            }
        }
    }
}

vector<Chromosome> selection(vector<Chromosome>& pop)
{
    vector<Chromosome> mating;
    for(int i = 0; i<pop.size(); i++)
    {
        int index1 = rand() % pop.size();
        int index2 = rand() % pop.size();
        pop[index1].fitness < pop[index2].fitness ? mating.push_back(pop[index1]) : mating.push_back(pop[index2]);
    }
    return mating;
}

void variation(vector<Chromosome>& offspring)
{
    for(int i = 0; i<POPULATION; i+=2)
    {
        if(rand() % 100 < crossoverProbability * 100)
        {  
            crossover(offspring[i], offspring[i+1]);
        }
        if(rand() % 100 < mutationProbability * 100)
        {
            mutation(offspring[i]);
            
        }
        if(rand() % 100 < mutationProbability * 100)
        {
            mutation(offspring[i+1]);
            
        }
    }
}


Chromosome generateIndividual()
{
    Chromosome individual;
    individual.ServerAllocations = vector<vector<int>>(current1.getNumServers());
    for(int i=0; i<current1.getNumServers(); i++)
    {
        individual.ServerAllocations[i] = vector<int>(current1.getNumClients());
        for(int j=0; j<current1.getNumClients(); j++)
        {
            individual.ServerAllocations[i][j] = rand() % upperBound;
        }
    }
    return individual;
    
}

vector<Chromosome> generateRandomPopulation(){
    vector<Chromosome> ans;
    for(int i = 0; i<POPULATION; i++){
        ans.push_back(generateIndividual());
    }
    return ans;
}

void printIndividual(Chromosome& individual)
{
    if(individual.isFeas){
        cout << "Feasible Solution" << endl;
    }
    else{
        cout << "Infeasible Solution" << endl;
    }
    for(int i=0; i<current1.getNumServers(); i++)
    {
        for(int j=0; j<current1.getNumClients(); j++)
        {
            cout << individual.ServerAllocations[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


void geneticAlgorithm(Task task1)
{
    srand(time(NULL));
    current1 = task1;
    findUpperBound();
    vector<Chromosome> parentPop = generateRandomPopulation();
    
    evaluate(parentPop);
    for(int i = 0; i<GENERATIONS; i++){
        vector<Chromosome> offspring = selection(parentPop);
        variation(offspring);
        evaluate(offspring);
        parentPop = survivor(offspring, parentPop);

        cout<<endl;
        cout << "Generation " << i+1 << " Current Best: " << endl;
        printIndividual(parentPop[0]);
        cout << "Fitness = " <<parentPop[0].fitness <<endl;
        cout << "BandwithScore = " << parentPop[0].bandwithFitness << endl;

    }
    cout << "Best: " <<endl;
    printIndividual(parentPop[0]);
    cout << "Fitness = " <<parentPop[0].fitness <<endl;
    
}

void printPopulation(vector<Chromosome>& population)
{
    for(int i=0; i<population.size(); i++)
    {
        cout << "Chromosome " << i << endl;
        for(int j=0; j<current1.getNumServers(); j++)
        {
            for(int k=0; k<current1.getNumClients(); k++)
            {
                cout << population[i].ServerAllocations[j][k]  << " ";
            }
            cout << endl;
        }
        cout << "Latency Score: " << population[i].latencyScore << endl;
        cout << "Penalty Capacity: " << population[i].penaltyCapacity << endl;
        cout << "Penalty Bandwith: " << population[i].penaltyBandwith << endl;
    }
}

void calculateLatencyScore(Chromosome& individual)
{
    double sum = 0;

    vector<int> bandwithAlloc;

    for(int i = 0; i<current1.getNumClients(); i++)
    {
        unsigned int sum = 0;
        for(int j = 0; j<current1.getNumServers(); j++)
        {
            sum += individual.ServerAllocations[j][i];
        }
        bandwithAlloc.push_back(sum);
    }

    /*sum = 0;
    for(int i=0; i<current1.getNumClients(); i++)
    {
        for(int j=0; j<current1.getNumServers(); j++)
        {
            double dummyScore =  individual.ServerAllocations[j][i] / bandwithAlloc[i];

            sum +=   individual.ServerAllocations[j][i] / dummyScore;
        }
    }
    individual.latencyScore = sum; */

    sum = 0;
    for(int i=0; i<current1.getNumServers(); i++)
    {
        for(int j=0; j<current1.getNumClients(); j++)
        {
            sum +=   individual.ServerAllocations[i][j] / current1.getLatency(i, j);
        }
    }
    individual.latencyScore = sum;
}

void bandwithFitness(Chromosome& indi){

    vector<int> bandwithAlloc;

    for(int i = 0; i<current1.getNumClients(); i++){

        unsigned int sum = 0;
        for(int j = 0; j<current1.getNumServers(); j++){
            sum += indi.ServerAllocations[j][i];
        }
        bandwithAlloc.push_back(sum);
    }

    double summation = 0.0;
    for(int i = 0; i<current1.getNumClients(); i++){
        summation += (current1.getBandwith(i) - bandwithAlloc[i]);
    }
   
    indi.bandwithFitness = summation;
}

void calculatePenalty(Chromosome& indi){

    unsigned int capacityPen = 0;
    unsigned int bandwithPen = 0;
    unsigned int connectionPen = 0;

    for(int i = 0; i<current1.getNumServers(); i++){
        unsigned int sum = 0;
        for(int j = 0; j<current1.getNumClients(); j++){
            if(indi.ServerAllocations[i][j] == 0){
                connectionPen += current1.getBandwith(i)*2.5;
            }
            sum += indi.ServerAllocations[i][j];
        }
        if(sum > current1.getCapacity(i)){
            capacityPen += (sum-current1.getCapacity(i));
        }
    }
    
    

    indi.penaltyCapacity = capacityPen;
    indi.penaltyBandwith = bandwithPen;
    indi.connectionPen = connectionPen;
}

void evaluate(vector<Chromosome>& pop)
{
    for(int i = 0; i<POPULATION; i++)
    {
        calculateLatencyScore(pop[i]);
        bandwithFitness(pop[i]);
        calculatePenalty(pop[i]);
        pop[i].fitness = pop[i].bandwithFitness + pop[i].latencyScore + 2.5 * pop[i].penaltyCapacity + 2.5 * pop[i].penaltyBandwith + pop[i].connectionPen;
        if(pop[i].penaltyCapacity+pop[i].penaltyBandwith+pop[i].connectionPen == 0){
            pop[i].isFeas =true;
        }
        else{
            pop[i].isFeas =false;
        }

    }
}

void findUpperBound(){
    for(int i = 0; i<current1.getNumServers(); i++){
        if(current1.getCapacity(i) > upperBound){
            upperBound = current1.getCapacity(i);
        }
    }
    for(int i = 0; i<current1.getNumClients(); i++){
        if(current1.getBandwith(i) > upperBound){
            upperBound = current1.getBandwith(i);
        }
    }
}