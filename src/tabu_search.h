/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 12/03/2014
 */

#ifndef TABU_SEARCH_H 
#define TABU_SEARCH_H 

#include "instances.h"

#include <assert.h> 
#include <bitset>
#include <random>

#define MIN_PARAM    3
#define NOT_EQUAL    1

typedef std::vector<std::vector<bool>> Population;
typedef std::vector<std::vector<bool>> Neighbors;

void SolveTabuSearch(Instances * inst);
Population InitializePopulation(size_t length, int size);

bool IsTabu(std::vector<bool> solution, std::vector<std::vector<bool>> tabu);
bool CompareBits(std::vector<bool> vb1, std::vector<bool> vb2);
std::vector<bool> FlipBit(std::vector<bool> vb, int position);
std::vector<std::vector<bool>> 
GetNeighbors(std::vector<bool> solution, std::vector<std::vector<bool>> tabu);
void PrintBinaryVector(std::vector<bool> * bv);
void PrintVectorBinaryVector(std::vector<std::vector<bool>> bvv);
std::vector<bool> GenerateBinaryVector(int length);
std::vector<bool> Evaluate(Instance * inst, 
                           std::vector<std::vector<bool>> population, 
                           int max_volume);


#endif //TABU_SEARCH
