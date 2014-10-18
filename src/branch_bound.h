/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 10/18/2014
 */

#ifndef BRANCHBOUND_HH
#define BRANCHBOUND_HH

#define MIN_PARAM   2
#define EMPTY      -1

#include <queue>

#include "instances.h"

void SolveBranchBound(Instances * all_instances);
std::vector<bool> NextItem(std::vector<bool> * inst_item, int position);
int GetPosition(std::vector<bool> * inst_item);
void PrintBinaryVector(std::vector<bool> * bv);
std::vector<bool> CreateEmptyKnapsack(int size);
std::vector<bool> Evaluate(Instance * inst, int knapsack_capacity, int max_items);
int ComputeCurrentCost(std::vector<bool> * distribution, Instance * inst);
int ComputeRemainingCost(std::vector<bool> * distribution, Instance * inst);
int ComputeCurrentWeight(std::vector<bool> * distribution, Instance * inst);

#endif //BRANCHBOUND_HH
