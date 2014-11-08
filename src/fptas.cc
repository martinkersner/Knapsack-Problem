/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 11/03/2014
 *
 * @section DESCRIPTION
 *
 * KNAPSACK PROBLEM 
 *
 * FPTAS solution.
 */

#include "fptas.h"

int main(int argc, char **argv) {
    if (argc >= MIN_PARAM) {
        char * file_name = argv[1];
        auto inst = new Instances(file_name);
        SolveFptas(inst);
        inst->PrintSolutions();

        delete inst;
    }


    return EXIT_SUCCESS;
}

/**
 * Solves given instances by FPTAS method.
 *
 * @param  inst  different instances of knapsack problem
 */
void SolveFptas(Instances * inst) {
    int capacity = inst->GetCapacity();
    int volume = inst->GetVolume();
    int max_items = inst->GetMaxItems();

    std::vector<Instance *> all_instances =  inst->GetAllInstances();
    std::vector<bool> solution;

    for (auto inst_it = all_instances.begin(); inst_it != all_instances.end(); ++inst_it) {
        solution = Evaluate(*inst_it, capacity, max_items);
        (*inst_it)->solution = solution;
    }
}

/**
 * Evaluates particular instance of knapsack problem by FPTAS.
 * Firstly the network is created, then solved and solution returned.
 *
 * @param  inst       particualr instance of knapsack problem
 * @param  capacity   capacity of knapsack
 * @param  max_items  maximum number of items in knapsack
 * @return            binary vector, the best selection of items for knapsack
 */
std::vector<bool> Evaluate(Instance * inst, int capacity, int max_items) {
    std::vector<int> weights = inst->weight;

    // create and compute network
    std::vector<std::vector<Cell*>> table = CreateNetwork(inst, capacity);
    
    // find final weight and cost
    Cell * c_w = FindCostWeight(&table, capacity);

    std::vector<bool> solution = GetSolutionPath(&table, c_w, inst);

//    // compute network values
//    Solution S = SolveNetwork(inst, &table);
    inst->sum_cost = c_w->cost;
//
//    // deallocate all nodes
//    DeleteTable(&table);
//
    return solution;
}

/**
 * Creates network for compution best combination of items in knapsack.
 * 
 * @param  inst      particular instance of knapsack problem
 * @param  capacity  capacity of knapsack
 * @return           network used for solving knapsack problem by dynamic programming
 */
std::vector<std::vector<Cell *>> CreateNetwork(Instance * inst, int capacity) {
    std::vector<Cell*> column;
    int tmp_cost;
    int tmp_weight;
    std::vector<std::vector<Cell *>> table;

    Cell * tmp_cell = NULL;
    std::vector<int> costs = inst->cost;
    std::vector<int> weights = inst->weight;

    Cell * init_cell = CreateCell( costs.front(), 
                                   weights.front(),
                                   NULL );

    std::vector<Cell *> init_col = {init_cell};
    table.insert(table.begin(), init_col); 

    // +1 because we have already used the first item in creating network
    auto c = costs.begin()+1;
    for (auto w = weights.begin()+1; w != weights.end(); ++w, ++c) {

        std::vector<Cell*> tmp_column;
        column = table.front();

        for (auto col = column.begin(); col != column.end(); ++col) {
            tmp_cost = (*col)->cost;
            tmp_weight = (*col)->weight;

            // recreate the same cell
            // TODO exist cell?
            tmp_cell = CreateCell(tmp_cost, tmp_weight, *col);
            tmp_column.push_back(tmp_cell);

            // create new cell
            tmp_cell = CreateCell((*c), (*w), NULL);
            tmp_column.push_back(tmp_cell);

            // summed cell
            // add weight and cost to the current examined item
            // TODO exist cell?
            tmp_cell = CreateCell(tmp_cost+(*c), tmp_weight+(*w), NULL);
            tmp_column.push_back(tmp_cell);

            //std::cout << "REC " << tmp_cost << " "<< tmp_weight << std::endl;
            //std::cout << "NEW " << tmp_cost+(*c) << " "<< tmp_weight+(*w) << std::endl;
        }

        table.insert(table.begin(), tmp_column); 

    }

    return table;
}

/**
 * Creates and initializes cell.
 *
 * @param  c  cost
 * @param  w  weight
 * @return    pointer to new created cell
 */
Cell * CreateCell(int c, int w, Cell * p_c) {
    auto cell = new Cell();

    cell->cost = c;
    cell->weight = w;
    //cell->weight_index = wi;
    cell->forward = p_c;
    //cell->forward_second = NULL;

    return cell;
}

/**
 * Finds the best combination of cost and weight in the last column 
 * of created network/table.
 *
 * @param   table     generated network
 * @param   capacity  maximum capacity fo knapsack
 * @return            cell containin the best combination of weight and cost
 */
Cell * FindCostWeight(std::vector<std::vector<Cell*>>* table, int capacity) {
    std::vector<Cell*> last_column = table->front();

    Cell * tmp_best_cell = NULL;

    for (auto c_it = last_column.begin(); c_it != last_column.end(); ++c_it) {
       if (TestCellWeight(*c_it, capacity)) {

        if (tmp_best_cell == NULL) // the first occasion of solution
            tmp_best_cell = *c_it;
        else // not the first occasion of solution
            tmp_best_cell = CompareCells(tmp_best_cell, *c_it);
       }
    }

    return tmp_best_cell;
}

/**
 * Compares two cells in order to find the best solution.
 * Assumes that weight of both cells does not exceed the maximum weight.
 *
 * @param   c1  the first tested cell
 * @param   c2  the second tested cell
 * @return      the best cell selected from tested cells
 */
Cell * CompareCells(Cell * c1, Cell * c2) {
    if (c1->cost > c2->cost)
        return c1;
    else
        return c2;
}

/**
 * Tests if the weight of given cell does not exceed desired maximum allowed weight.
 *
 * @param   c           tested cell
 * @param   max_weight  maximum weight of knapsack
 * @return              boolean decision about weight of knapsack
 */
bool TestCellWeight(Cell * c, int capacity) {
    return (c->weight <= capacity);
}

/**
 *
 */
std::vector<bool> GetSolutionPath(std::vector<std::vector<Cell *>> * table, Cell * cell, Instance * inst) {
    auto w_it = inst->weight.end()-1;
    auto col_it = table->begin()+1;
    int tmp_weight = cell->weight;

    Cell * forward_cell = cell;

    std::vector<bool> path;

    while (tmp_weight > 0) {
        if (forward_cell->forward == NULL) {
            path.insert(path.begin(), 1);
      //      std::cout << 1 << std::flush;

            tmp_weight -= (*w_it);
            forward_cell = FindNextCell(&(*col_it), tmp_weight);
        }
        else {
            path.insert(path.begin(), 0);
    //        std::cout << 0 << std::flush;
            forward_cell = forward_cell->forward;
        }

        --w_it;
        ++col_it;
    }

    path = RecoverPath(&path, inst->weight.size());

    return path;
}

/**
 * Finds the next cell, which can be connected to current cell with given weight, 
 * in lower level of network.
 *
 * @param   column  column of the network
 * @param   weight  searched weight of cell
 * @return          cell with the highest cost and desired weight
 */
Cell * FindNextCell(std::vector<Cell *> * column, int weight) {
    Cell * tmp_cell = NULL;

    for (auto col_it = column->begin(); col_it != column->end(); ++col_it) {
        if ((*col_it)->weight == weight) {
            if (tmp_cell == NULL) 
                tmp_cell = *col_it;
            else
                tmp_cell = CompareCells(tmp_cell, *col_it);
        }
    }

    return tmp_cell;
}

/**
 * Prints information about cost and weight inside of cell.
 *
 * @param   cell  examined cell
 */
void PrintCell(Cell * cell) {
    std::cout << "(" << cell->cost << ", " << cell->weight << ")" << std::endl << std::flush;
}

/**
 *
 */
std::vector<bool> RecoverPath(std::vector<bool> * path, int length) {
    int d;
    std::vector<bool> full_path = *path;

    if ((d = length - path->size()) != 0)
        for (int i = 0; i < d; ++i) full_path.insert(full_path.begin(), 0);

    return full_path;
}

/**
 * Prints binary vector on standard output.
 *
 * @param  bv  binary vector
 */
void PrintBinaryVector(std::vector<bool> * bv) {
    for (auto it = bv->begin(); it != bv->end(); ++it)
        std::cout << *it;

    std::cout << std::endl;
}


/**
 * Reducts precision of given number. Ommited precision is set by parameter p.
 *
 * @param   b  bits
 * @param   p  desired precision
 * @return     binary number with reducted precision
 */
bit10 ReducePrecision(bit10 b, int p) {
    return CreateMask(p)&=b;
}

/**
 * Creates binary mask. 'l' rightmost bits have number 1.
 *
 * @param  l  lenght of masked bits
 * @return    binary mask
 */
bit10 CreateMask(int l) {
    bit10 mask(0);

    for (int i = 0; i < BITS; ++i)
        if (i >= l) mask.set(i, 1);

    return mask;
}

/**
 * Converts binary number represented by biset to integer representation.
 *
 * @param  b  binary number
 * @return    integer number
 */
int bit2int(bit10 b) {
    return (int)(b.to_ulong());
}
