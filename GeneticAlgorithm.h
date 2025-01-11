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
#define POPULATION 20
#define GENERATIONS 1000
#define crossoverProbability 0.9
#define mutationProbability 0.2

vector<unsigned int> upperBounds;

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
double blxacrossover(double, double, double, int);
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

void crossover(Chromosome& off1, Chromosome& off2){
    int crossoverPoint = rand() % current1.getNumServers();
    for(int i = crossoverPoint; i<current1.getNumServers(); i++){
        for(int j = 0; j<current1.getNumClients(); j++){

            double newVal1 = blxacrossover(off1.ServerAllocations[i][j], off2.ServerAllocations[i][j], 0.5, j);
            double newVal2 = blxacrossover(off1.ServerAllocations[i][j], off2.ServerAllocations[i][j], 0.5, j);
            off1.ServerAllocations[i][j] = newVal1;
            off2.ServerAllocations[i][j] = newVal2;
        }
    }
}

double blxacrossover(double p1, double p2, double alpha, int clientNum)
{

    double minVal = min(p1, p2);
    double maxVal = max(p1, p2);

    double u = (rand() % 100) /100;
    double gamma = ((1.0 + 2.0 * alpha) * u) - alpha;
    double off = ((1.0 - gamma) * minVal) + (gamma * maxVal);
    off = max(min(off, (double) upperBounds[clientNum]), 0.0);
    
    return off;
}

void mutation(Chromosome& off1){
    for(int i = 0; i<off1.ServerAllocations.size(); i++){
        for(int j = 0; j<off1.ServerAllocations[0].size(); j++){ 
            if(rand() % 2 == 1){
                off1.ServerAllocations[i][j] = rand() % upperBounds[j];
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
    individual.ServerAllocations = vector<vector<double>>(current1.getNumServers());
    for(int i=0; i<current1.getNumServers(); i++)
    {
        individual.ServerAllocations[i] = vector<double>(current1.getNumClients());
        for(int j=0; j<current1.getNumClients(); j++)
        {
            individual.ServerAllocations[i][j] = rand() % upperBounds[j];
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
        cout << "LatencyScore = " << parentPop[0].latencyScore<< endl;
        cout << "Penalty Capacity: " << parentPop[0].penaltyCapacity << endl;
        cout << "Penalty Bandwith: " << parentPop[0].penaltyBandwith << endl;

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

    double latencyScore = 0.0;

    for(int i=0; i<current1.getNumClients(); i++)
    {
        double sum = 0.0;
        double latencyTotal = 0.0;
        for(int j=0; j<current1.getNumServers(); j++)
        {
            latencyTotal += (1.0 / current1.getLatency(j, i));
        }

        for(int j=0; j<current1.getNumServers(); j++){

            double expected =  current1.getBandwith(i) * (1.0 / current1.getLatency(j, i))/latencyTotal;

            latencyScore += abs(individual.ServerAllocations[j][i] - expected); 
        }
        
    }
    individual.latencyScore = latencyScore;
}

void calculatePenalty(Chromosome& indi){

    double capacityPen = 0;
    double bandwithPen = 0;
    double connectionPen = 0;

    for(int i = 0; i<current1.getNumServers(); i++){
        unsigned int sum = 0;
        for(int j = 0; j<current1.getNumClients(); j++){
            if(indi.ServerAllocations[i][j] == 0){
                connectionPen += 10000;
            }
            sum += indi.ServerAllocations[i][j];
        }
        if(sum > current1.getCapacity(i)){
            capacityPen += (sum-current1.getCapacity(i))*1000;
        }
    }
    
    for(int i = 0; i<current1.getNumClients(); i++){

        unsigned int sum = 0;
        for(int j = 0; j<current1.getNumServers(); j++){
            sum += indi.ServerAllocations[j][i];
        }
        if(sum > current1.getBandwith(i)){
            bandwithPen += (sum-current1.getBandwith(i));
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
        calculatePenalty(pop[i]);
        //double penaltyWeight = 25.0 * (1.0 - (double)generation / GENERATIONS);
        pop[i].fitness =  pop[i].latencyScore + 25 * pop[i].penaltyCapacity + 25 * pop[i].penaltyBandwith + 25*pop[i].connectionPen;
        if(pop[i].penaltyCapacity+pop[i].penaltyBandwith+pop[i].connectionPen == 0){
            pop[i].isFeas =true;
        }
        else{
            pop[i].isFeas =false;
        }

    }
}

void findUpperBound(){
    for(int i = 0; i<current1.getNumClients(); i++){
        upperBounds.push_back(0);
        if(current1.getBandwith(i) > upperBounds[i]){
            upperBounds[i] = current1.getBandwith(i);
        }
    }
}