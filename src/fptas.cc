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
        int precision = atoi(argv[2]);

        auto inst = new Instances(file_name);
        SolveFptas(inst, precision);
        inst->PrintSolutions();

        delete inst;
    }


    return EXIT_SUCCESS;
}

/**
 * Solves given instances by FPTAS method.
 *
 * @param   inst       different instances of knapsack problem
 * @param   precision  number of LSB which we can ommit
 */
void SolveFptas(Instances * inst, int precision) {
    int capacity = inst->GetCapacity();
    int volume = inst->GetVolume();
    int max_items = inst->GetMaxItems();

    std::vector<Instance *> all_instances =  inst->GetAllInstances();

    // reduction of precision
    ReducePrecisionInstances(all_instances, precision);

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

    // set the best cost of particular knapsack configuration
    inst->sum_cost = c_w->cost;

    // deallocate all nodes
    DeleteTable(table);

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
            if (!ExistCell(tmp_column, tmp_cost, tmp_weight)) {
                tmp_cell = CreateCell(tmp_cost, tmp_weight, *col);
                tmp_column.push_back(tmp_cell);
            }

            // create new cell
            if (!ExistCell(tmp_column, *c, *w)) {
                tmp_cell = CreateCell((*c), (*w), NULL);
                tmp_column.push_back(tmp_cell);
            }

            // summed cell
            // add weight and cost to the current examined item
            if (!ExistCell(tmp_column, tmp_cost+(*c), tmp_weight+(*w))) {
                tmp_cell = CreateCell(tmp_cost+(*c), tmp_weight+(*w), NULL);
                tmp_column.push_back(tmp_cell);
            }
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
    cell->forward = p_c;

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
 * Goes through the network and follows the path from the best solution to the beginning.
 *
 * @param   table
 * @param   cell
 * @param   inst   
 * @return         solution path
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

            tmp_weight -= (*w_it);

            if (tmp_weight <= 0)
                break;

            forward_cell = FindNextCell(&(*col_it), tmp_weight);
        }
        else {
            path.insert(path.begin(), 0);
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
 * Prints all cell stored in given column.
 *
 * @param   column
 */
void  PrintColumn(std::vector<Cell *> & column) {
    for (auto c_it = column.begin(); c_it != column.end(); ++c_it)
        PrintCell(*c_it);
}

/**
 * Searches for a cell with specific parameters in column.
 *
 * @param   column
 * @param   cost
 * @param   weight  
 * @return          true if column contains cell with tested cost and weight, 
 *                  otherwise false
 */
bool ExistCell(std::vector<Cell *> & column, int cost, int weight) {
    for (auto c_it = column.begin(); c_it != column.end(); ++c_it)
        if (EqualCell(*c_it, cost, weight))
            return true;
    
    return false;
}

/**
 * Checks if cell contain given cost and weight.
 *
 * @param   cell
 * @param   cost
 * @param   weight  
 * @return          true if cell contains tested cost and weight, otherwise false
 */
bool EqualCell(Cell * cell, int cost, int weight) {
    if (cell->cost == cost && cell->weight == weight)
        return true;
    else
        return false;
}

/**
 * Adds binary zeros to given path to extend binary solution to desired length.
 *
 * @param   path
 * @param   length  
 * @return          rovered path with right length of binary vector
 */
std::vector<bool> RecoverPath(std::vector<bool> * path, int length) {
    int d;
    std::vector<bool> full_path = *path;

    if ((d = length - path->size()) != 0)
        for (int i = 0; i < d; ++i) full_path.insert(full_path.begin(), 0);

    return full_path;
}

/**
 * Deallocated all cells from given table.
 *
 * @param   table
 */
void DeleteTable(std::vector<std::vector<Cell *>> & table) {
    for (auto t_it = table.begin(); t_it != table.end(); ++t_it)
        for (auto c_it = t_it->begin(); c_it != t_it->end(); ++c_it)
            delete *c_it;
}

/**
 * Reduces precison of cost in all later examined instances.
 *
 * @param   instances
 * @param   precision
 */
void ReducePrecisionInstances(std::vector<Instance *> & instances, int precision) {
    // cost
    for (auto i_it = instances.begin(); i_it != instances.end(); ++i_it)
        for (auto c_it = (*i_it)->cost.begin(); c_it != (*i_it)->cost.end(); ++c_it)
            *c_it = bit2int(ReducePrecision(*c_it, precision));
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
bit ReducePrecision(int n, int p) {
    bit b(n);
    return CreateMask(p)&=b;
}

/**
 * Creates binary mask. 'l' rightmost bits have number 1.
 *
 * @param  l  lenght of masked bits
 * @return    binary mask
 */
bit CreateMask(int l) {
    bit mask(0);

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
int bit2int(bit b) {
    return (int)(b.to_ulong());
}
