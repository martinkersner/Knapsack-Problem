/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 10/03/2014
 */

#ifndef HEURISTIC_HH
#define HEURISTIC_HH

#include <algorithm>

#include "instances.h"

#define MIN_PARAM 2

/**
 * Solves knapsack problem using heuristic. For all items is computed ratio 
 * of cost/weight, then this values are sorted and chosen first X values which still
 * fits to knapsack.
 *
 * @param  inst  pointer to Instances object
 */
void SolveHeuristic(Instances * inst);

/**
 * Prints vector of float values on line with space between numbers.
 *
 * @param  vec  pointer to vector of float values
 */
void PrintVector(std::vector<float> * vec);


#endif //HEURISTIC_HH
