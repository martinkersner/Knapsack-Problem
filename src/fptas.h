/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 11/03/2014
 */

#ifndef FPTAS_H 
#define FPTAS_H 

#include <bitset>

#include "instances.h"

#define MIN_PARAM    2
#define BITS        10

typedef std::bitset<BITS> bit10;

struct Cell {
    int cost;
    int weight;
    //int weight_index;
    Cell * forward;
    //Cell * forward_second;

    // 0 left; first
    // 1 bottom left; second
    //int direction;
};

struct Solution {
    std::vector<bool> solution_vector;
    int cost;
};


void SolveFptas(Instances * inst);
std::vector<std::vector<Cell *>> CreateNetwork(Instance * inst, int capacity);
std::vector<bool> Evaluate(Instance * inst, int capacity, int max_items);
std::vector<std::vector<Cell *>> CreateNetwork(Instance * inst, int capacity);
Cell * CreateCell(int c, int w, Cell * p_c);
bool TestCellWeight(Cell * c, int capacity);
Cell * CompareCells(Cell * c1, Cell * c2);
std::vector<bool> GetSolutionPath(std::vector<std::vector<Cell *>> * table, Cell* cell, Instance * inst);
Cell * FindCostWeight(std::vector<std::vector<Cell*>>* table, int capacity);
Cell * FindNextCell(std::vector<Cell *> * column, int weight);
void PrintCell(Cell * cell);
std::vector<bool> RecoverPath(std::vector<bool> * path, int length);
void PrintBinaryVector(std::vector<bool> * bv);

bit10 CreateMask(int l);
bit10 ReducePrecision(bit10 b, int p);
int bit2int(bit10 b);

#endif //FPTAS_H 
