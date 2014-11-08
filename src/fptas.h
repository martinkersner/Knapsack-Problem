/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 11/03/2014
 */

#ifndef FPTAS_H 
#define FPTAS_H 

#include <bitset>
#include <stdlib.h>

#include "instances.h"

#define MIN_PARAM    3
#define BITS        20

typedef std::bitset<BITS> bit;

struct Cell {
    int cost;
    int weight;
    Cell * forward;
};

struct Solution {
    std::vector<bool> solution_vector;
    int cost;
};

void SolveFptas(Instances * inst, int precision);
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
void  PrintColumn(std::vector<Cell *> & column);
bool ExistCell(std::vector<Cell *> & column, int cost, int weight);
bool EqualCell(Cell * cell, int cost, int weight);
void DeleteTable(std::vector<std::vector<Cell *>> & table);
void ReducePrecisionInstances(std::vector<Instance *> & instances, int precision);

bit CreateMask(int l);
bit ReducePrecision(int n, int p);
int bit2int(bit b);

#endif //FPTAS_H 
