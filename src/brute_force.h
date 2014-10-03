/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 09/30/2014
 */

#ifndef BRUTEFORCE_HH
#define BRUTEFORCE_HH

#include <iostream>
#include <string>

#include "instances.h"

#define MIN_PARAM 2

struct Evaluation {
    int cost;
    int weight;
};

void SolveBruteForce(Instances *);
std::vector<std::vector<bool>> * GenerateCombinations(int, int);
std::vector<bool> Evaluate(Instance *, std::vector<std::vector<bool>> *, int);
Evaluation EvaluateCombination(Instance *, std::vector<bool> *);
void PrintVector(std::vector<bool> *);
//void StoreSolution();


#endif //BRUTEFORCE_HH
