/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 10/18/2014
 *
 * @section DESCRIPTION
 *
 * KNAPSACK PROBLEM 
 *
 * Dynamic programming solution.
 */

#include "dynamic.h"

int main(int argc, char** argv) {
    
    if (argc >= MIN_PARAM) {
        char * file_name = argv[1];
        auto inst = new Instances(file_name);
        SolveDynamic(inst);
        inst->PrintSolutions();

        delete inst;
    }

    return EXIT_SUCCESS;
}

/**
 * Solves given instances by dynamic programming method.
 *
 * @param  inst  different instances of knapsack problem
 */
void SolveDynamic(Instances * inst) {
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
 * Evalueates particular instance of knapsack problem by dynamic programming.
 * Firstly the network is created, then solved and solution returned.
 *
 * @param  inst       particualr instance of knapsack problem
 * @param  capacity   capacity of knapsack
 * @param  max_items  maximum number of items in knapsack
 * @return            binary vector, the best selection of items for knapsack
 */
std::vector<bool> Evaluate(Instance * inst, int capacity, int max_items) {
    std::vector<bool> solution;
    std::vector<int> weights = inst->weight;

    // create network
    std::vector<std::vector<Cell*>> table = CreateNetwork(inst, capacity);

    // compute network values
    Solution S = SolveNetwork(inst, &table);
    inst->sum_cost = S.cost;

    // deallocate all nodes

    return S.solution_vector;
}

/**
 * Creates and initializes cell.
 *
 * @param  wi  weight index in knapsack of created cell
 * @return     pointer to new created cell
 */
Cell * CreateCell(int wi) {
    auto cell = new Cell();

    cell->cost = 0;
    cell->weight = 0;
    cell->weight_index = wi;
    cell->forward_first = NULL;
    cell->forward_second = NULL;

    return cell;
}

/**
 * Controls if particular cell does already exist and returns pointer to that cell,
 * otherwise NULL pointer.
 *
 * @param  column  colummn where the search is happening
 * @param  weight  weight of searched cell
 * @return         pointer to searched cell
 */
Cell * ExistCell(std::vector<Cell *> * column, int weight) {
    for (auto it = column->begin(); it != column->end(); ++it)
        if ((*it)->weight_index == weight) return *it;

    return NULL;
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
 * Creates network for compution best combination of items in knapsack.
 * 
 * @param  inst      particular instance of knapsack problem
 * @param  capacity  capacity of knapsack
 * @return           network used for solving knapsack problem by dynamic programming
 */
std::vector<std::vector<Cell *>> CreateNetwork(Instance * inst, int capacity) {
    std::vector<Cell*> column;
    int tmp_weight;
    std::vector<std::vector<Cell *>> table;

    Cell * tmp_cell = NULL;
    std::vector<int> weights = inst->weight;

    Cell * init_cell = CreateCell(capacity);
    std::vector<Cell *> init_col = {init_cell};
    table.insert(table.begin(), init_col); 

    for (auto w = weights.begin(); w != weights.end(); ++w) {

        std::vector<Cell*> tmp_column;
        column = table.front();

        for (auto c = column.begin(); c != column.end(); ++c) {
            // first arrow; left direction
            tmp_weight = (*c)->weight_index;

            if (ExistCell(&tmp_column, tmp_weight) == NULL) {
                tmp_cell = CreateCell(tmp_weight);
                tmp_column.push_back(tmp_cell);
            }

            (*c)->forward_first = tmp_cell;

            // second arrow; bottom left direction
            if ( (tmp_weight = (*c)->weight_index - (*w)) >= 0) {
                if ((tmp_cell = ExistCell(&tmp_column, tmp_weight)) == NULL) {
                    tmp_cell = CreateCell(tmp_weight);
                    tmp_column.push_back(tmp_cell);
                }

                (*c)->forward_second = tmp_cell;
            }

        }

        table.insert(table.begin(), tmp_column); 

    }

    return table;
}

/**
 * Computes weight and cost of selected items in knapsack continuously from zero items 
 * to maximum allowed number of items.
 *
 * @param  inst   particular instance of knapsack problem
 * @param  table  necessary network used for dynamic programming
 * @return        solution of network, carries binary vector of selected items and total
 *                cost of items
 */
Solution SolveNetwork(Instance * inst, std::vector<std::vector<Cell *>> * table) {
    Cell * first_cell;
    Cell * second_cell;
    int tmp_cost_first;
    int tmp_cost_second;
    int tmp_cost_item;

    std::vector<int> costs = inst->cost;
    std::vector<int> weights = inst->weight;
    int cw_it = inst->cost.size() - 1;

    // solution temporary variables
    Solution S;
    std::vector<bool> solution_vector;
    Cell * solution_cell = NULL;

    // solves network
    // +1 in iterator is due to starting from second column, 
    // first column is already filled
    for (auto t = table->begin()+1; t != table->end(); ++t, --cw_it) {
        for (auto c = (*t).begin(); c != (*t).end(); ++c) {
            first_cell = (*c)->forward_first;
            second_cell = (*c)->forward_second;

            // just arrow from left direction
            if (second_cell == NULL) {
                (*c)->cost = (*c)->forward_first->cost;
                (*c)->weight += (*c)->forward_first->weight;
                (*c)->direction = 0;
            }
            else {
            // both arrows
                tmp_cost_first = (*c)->forward_first->cost;
                tmp_cost_second = (*c)->forward_second->cost + costs.at(cw_it);

                // arrow from left direction
                if (tmp_cost_first > tmp_cost_second) {
                    (*c)->cost = tmp_cost_first;
                    (*c)->weight += (*c)->forward_first->weight;
                    (*c)->direction = 0;
                }
                else {
                //arrow from bottom left direction
                    (*c)->cost = tmp_cost_second;
                    (*c)->weight += (*c)->forward_second->weight + weights.at(cw_it);
                    (*c)->direction = 1;
                }

                // store temporary best solution
                solution_cell = (*c);
                S.cost = (*c)->cost;
            }
        }
    }

    S.solution_vector = FindPath(solution_cell);

    return S;
}

/**
 * Find a path which denotes selected items of knapsack for particular instance. 
 *
 * @param   cell  cell contataining final solution
 * @return        final combination of items in knapsack
 */
std::vector<bool> FindPath(Cell * cell) {
    std::vector<bool> path;
    Cell * tmp_cell = cell;

    while (tmp_cell != NULL) {
        switch (tmp_cell->direction) {
            // left
            case 0:
                path.push_back(0);
                tmp_cell = tmp_cell->forward_first;
                break;

            // bottom left
            case 1:
                path.push_back(1);
                tmp_cell = tmp_cell->forward_second;
                break;
        }
    }

    // delete last item, exceeds limits of knapsack
    path.erase(path.end());

    return path;
}
