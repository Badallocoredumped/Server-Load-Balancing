#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

const int POPULATION_SIZE = 200;
const int KEY_LENGTH = 32;
const int GENERATIONS = 100;
const double MUTATION_RATE = 0.2;
const int penaltyMult = 5;
const int crossoverMethod = 1;

struct chrom{
    string key;

    double uniqScore;
    double ShannonScore;

    double PatternPenalty;

    double fitness;
};


bool compareByFitness(const chrom& a, const chrom& b) {
    return a.fitness < b.fitness;
}
char randomChar();
string generatePassword();
vector<chrom> generateRandomPopulation();
void evaluate(vector<chrom>&);
vector<chrom> selection(vector<chrom>&);
void variation(vector<chrom>&);
void crossover(chrom&, chrom&);
void mutation(chrom&);
vector<chrom> survivor(vector<chrom>&, vector<chrom>&);


void uniqTest(chrom&);
void freqTest(chrom&);
void penalize(chrom&);


vector<chrom> generateRandomPopulation(){

    vector<chrom> ans;

    for(int i = 0; i<POPULATION_SIZE; i++){
        chrom indi;
        indi.key = generatePassword();
        ans.push_back(indi);
    }
    return ans;
}

string generatePassword() {
    string password;
    for (int i = 0; i < KEY_LENGTH; ++i) {
        password += randomChar();
    }
    return password;
}

char randomChar() {
    const string CHAR_SET = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+={}[]:;'<>,.?/";
    return CHAR_SET[rand() % CHAR_SET.size()];
}

void evaluate(vector<chrom>& pop){
    for(int i = 0; i<POPULATION_SIZE; i++){
        uniqTest(pop[i]);
        freqTest(pop[i]);
        penalize(pop[i]);
        pop[i].fitness = pop[i].uniqScore + pop[i].PatternPenalty;
    }
}

void uniqTest(chrom& a){
    bool hasUpper = true, hasLower = true, hasDigit = true, hasSpecial = true;
    for (char c : a.key) {
        if (isupper(c)) hasUpper = false;
        else if (islower(c)) hasLower = false;
        else if (isdigit(c)) hasDigit = false;
        else hasSpecial = false;
    }

    a.uniqScore = (hasUpper + hasLower + hasDigit + hasSpecial) * 2.5;
}

void freqTest(chrom& a){
    short freq[256] = {0};
    for(auto c: a.key){
        freq[c]++;
    }

    double entropy = 0.0;
    for (int i = 0; i<256; i++) {
        if (freq[i] > 0) {
            double p = freq[i] / (double) KEY_LENGTH;
            entropy -= p * log2(p);
        }
    }
    double maxEntropy = log2(89);
    a.ShannonScore = maxEntropy - entropy;

}

void penalize(chrom& a){
    int patternPenalty = 0;
    for (int i = 0; i < KEY_LENGTH - 1; ++i) {
        if ((a.key[i + 1] == a.key[i] + 1) || (a.key[i + 1] == a.key[i]-1)) {
            patternPenalty++; 
        }
    }

    a.PatternPenalty = patternPenalty * penaltyMult;
}

vector<chrom> selection(vector<chrom>& pop){
    vector<chrom> mating;
    for(int i = 0; i<pop.size(); i++){
        int index1 = rand() % pop.size();
        int index2 = rand() % pop.size();
        pop[index1].fitness < pop[index2].fitness ? mating.push_back(pop[index1]) : mating.push_back(pop[index2]);
    }
    return mating;
}

void variation(vector<chrom>& offspring){
    for(int i = 0; i<offspring.size(); i+=2){
        crossover(offspring[i], offspring[i+1]);
        mutation(offspring[i]);
        mutation(offspring[i+1]);

    }
}

void crossover(chrom& p1, chrom& p2){
 
    if(crossoverMethod == 1){  
        int fixedPoint = rand() % KEY_LENGTH;
        for(int i = fixedPoint; i<KEY_LENGTH; i++){
            char temp = p1.key[i];
            p1.key[i] = p2.key[i];
            p2.key[i] = temp;
        }
    }
    else{
        //Random Crossover
        for (int i = 0; i < KEY_LENGTH; ++i) {
            if (rand() % 2 == 0) {
                char temp = p1.key[i];
                p1.key[i] = p2.key[i];
                p2.key[i] = temp;
            }
        }
    }
}

void mutation(chrom& indi){
    int i = rand() % 10;
    if(i< MUTATION_RATE*10){
        for(int i = 0; i<KEY_LENGTH; i++){
            if(rand() % 2 == 1){
                indi.key[i] = randomChar();
            }
        }
    }
}

vector<chrom> survivor(vector<chrom>& offspring, vector<chrom>& parentPop){
    vector<chrom> newParent;
    sort(offspring.begin(), offspring.end(), compareByFitness);
    sort(parentPop.begin(), parentPop.end(), compareByFitness);

    int offspringIndx = 0;
    int ParentIndx = 0;
    for(int i = 0; i<POPULATION_SIZE; i++){
        if(offspring[offspringIndx].fitness < parentPop[ParentIndx].fitness){
            newParent.push_back(offspring[offspringIndx++]);
        }
        else{
            newParent.push_back(parentPop[ParentIndx++]);
        }
    }
    return newParent;
}

int main(){
    srand(time(NULL));

    vector<chrom> parentPop = generateRandomPopulation();

    evaluate(parentPop);
    for(int i = 0; i<GENERATIONS; i++){
        vector<chrom> offspring = selection(parentPop);
        variation(offspring);
        evaluate(offspring);
        parentPop = survivor(offspring, parentPop);
        cout << "Generation " << i+1 << " Current Best: " << parentPop[0].key << " fitness = " <<parentPop[0].fitness <<endl;
    }
    cout << "Best: " << parentPop[0].key << " fitness = " <<parentPop[0].fitness <<endl;
}