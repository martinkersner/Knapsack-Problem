/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 10/18/2014
 */

#ifndef DYNAMIC_H
#define DYNAMIC_H

#include "instances.h"

#define MIN_PARAM   2
#define BEGIN       0

struct Cell {
    int cost;
    int weight;
    int weight_index;
    Cell * forward_first;
    Cell * forward_second;

    // 0 left; first
    // 1 bottom left; second
    int direction;
};

struct Solution {
    std::vector<bool> solution_vector;
    int cost;
};

void SolveDynamic(Instances * inst);
std::vector<bool> Evaluate(Instance * inst, int capacity, int max_items);
Cell * CreateCell(int wi);
Cell * ExistCell(std::vector<Cell *> * column, int weight);
void PrintBinaryVector(std::vector<bool> * bv);
std::vector<std::vector<Cell *>> CreateNetwork(Instance * inst, int capacity);
Solution SolveNetwork(Instance * inst, std::vector<std::vector<Cell *>> * table);
std::vector<bool> FindPath(Cell * cell);
void DeleteTable(std::vector<std::vector<Cell*>> * table);

#endif //DYNAMIC_H
