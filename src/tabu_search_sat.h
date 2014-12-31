/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 12/28/2014
 */

#ifndef TABU_SEARCH_SAT_H 
#define TABU_SEARCH_SAT_H 

#include "sat.h"

#include <assert.h> 
#include <bitset>
#include <cmath>
#include <random>
#include <stdlib.h>

typedef std::vector<std::vector<bool>> Population;
typedef std::vector<std::vector<bool>> Neighbors;

const int INVALID_SOLUTION = -1;
const int MIN_PARAM        =  1;
const int NOT_EQUAL        =  1;

struct State {
    std::vector<bool> solution;
    int numberViolated;
    int weightSum;
};

struct Settings {
    int neighborhoodSize;
    int populationLength;
    int numberIterations;
    int duration;
    int solutionSize;
};

struct Tabu {
    std::vector<bool> solution;
    int duration;
};

struct TabuClause {
    std::vector<bool> clause;
    std::vector<bool> position;
    int duration;
};

void SolveTabuSearch(SatInstance & inst, Settings & settings);

Population InitializePopulation(Settings & settings);

bool IsTabu(std::vector<bool> solution, std::vector<Tabu> tabu);

bool CompareBits(std::vector<bool> vb1, std::vector<bool> vb2);

std::vector<bool> FlipBit(std::vector<bool> vb, int position);

std::vector<State> 
GetNeighbors(std::vector<bool> & solution, 
             std::vector<Tabu> & tabu,
             std::vector<TabuClause> & vectorTabuClause,
             SatInstance & inst,
             Settings & settings);

void PrintBinaryVector(std::vector<bool>  bv);
void PrintVectorBinaryVector(std::vector<std::vector<bool>> bvv);

std::vector<bool> GenerateBinaryVector(int length);

void Evaluate(SatInstance & inst, 
              Population population, 
              std::vector<Tabu> tabu,
              Settings & settings);

State
BestNeighbor(SatInstance & inst, std::vector<State> & neighbors);

void ClearTabu(std::vector<Tabu> & tabu);
void ClearTabuClause(std::vector<TabuClause> & tabu);

std::vector<Tabu> InitializeTabu();

void AddTabu(Population & population, 
             std::vector<Tabu> & tabu, 
             int duration);

void AddOneTabu(std::vector<bool> solution, 
                std::vector<Tabu> & tabu,
                int duration);

Tabu CreateTabu(std::vector<bool> solution, int duration);

std::vector<bool> ZeroSolution(int length);

State SolveBooleanFormula(SatInstance & inst, 
                          std::vector<bool> & solution,
                          std::vector<TabuClause> & vectorTabuClause,
                          int duration);

bool FlipBool(bool b);

bool EvaluateBool(int p, std::vector<bool> & solution);

bool Or(bool b1, bool b2);

bool SolveClause(Clause & c, std::vector<bool> & solution);

State CreateState(std::vector<bool> & solution);

int CountWeights(std::vector<bool> & solution, std::vector<int> & weights);

State CompareStates(State & s1, State & s2);

State LowerViolated(State & s1, State & s2);

void PrintBest(State & best);

void PrintSuboptimal(SatInstance & inst);

void PrintMaxWeight(SatInstance & inst);

bool UnitTests();

bool UnitTestEvaluate(int id, 
                      State & s, 
                      int weightSum, 
                      int numberViolated);

std::vector<bool> 
CreatePositionBinaryVector(std::vector<int> & positions, 
                           int length);

bool IsTabuClause(std::vector<TabuClause> & tabu, 
                  TabuClause & clause);

void AddTabuClause(std::vector<TabuClause> & tabu, 
                   TabuClause & clause);

bool EqualTabuClause(TabuClause & input, 
                     TabuClause & pattern);

TabuClause 
CreateTabuClause(std::vector<int> & clause,
                 int size,
                 int duration);

std::vector<bool> 
CreateClauseBinaryVector(std::vector<int> & positions, 
                         int length);

TabuClause 
CreateSimpleTabuClause(std::vector<bool> & clause,
                       std::vector<bool> & position, 
                       int duration);

bool IsSolutionInTabuClause(std::vector<bool> solution,
                            std::vector<TabuClause> tabuClause);

#endif //TABU_SEARCH_SAT_H
