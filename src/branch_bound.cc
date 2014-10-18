/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 10/18/2014
 *
 * @section DESCRIPTION
 *
 * KNAPSACK PROBLEM 
 *
 * Branch & Bound solution.
 */

#include "branch_bound.h"

int main(int argc, char** argv) {
    
    if (argc >= MIN_PARAM) {
        char * file_name = argv[1];
        auto inst = new Instances(file_name);
        SolveBranchBound(inst);
        inst->PrintSolutions();

        delete inst;
    }

    return EXIT_SUCCESS;
}

/**
 * Solves knapsack problem using Branch & Bound method.
 *
 * @param  inst  all loaded instances
 */
void SolveBranchBound(Instances * inst) {
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
 * Evaluates particular instances of knapsack problem.
 *
 * @param  inst       instance of knapsack problem
 * @param  capacity   maximum capacity of knapsack
 * @param  max_items  number of allowed items in knapsack
 * @return            solution of particular instances
 */
std::vector<bool> Evaluate(Instance * inst, int capacity, int max_items) {
    // create queue and insert distribution with empty knapsack
    std::queue<std::vector<bool>> queue; 
    queue.push(CreateEmptyKnapsack(max_items));

    // temporary variables
    int tmp_pos;
    std::vector<bool> tmp_item;

    // best solution variables
    int tmp_best_cost = 0;
    std::vector<bool> tmp_best_distribution;

    // current, remaining cost
    int cc;
    int rc;

    // current weight
    int cw;

    // run till queue is not empty
    while (!queue.empty()) {
        tmp_item = queue.front();
        queue.pop();

        cc = ComputeCurrentCost(&tmp_item, inst);

        if (cc >= tmp_best_cost) {
            tmp_best_cost = cc;
            tmp_best_distribution = tmp_item;
        }

        rc = ComputeRemainingCost(&tmp_item, inst);

        // COST bounding
        if ((cc + rc) < tmp_best_cost)
            continue;

        cw = ComputeCurrentWeight(&tmp_item, inst);

        // find position from where new distributions will be generated
        tmp_pos = GetPosition(&tmp_item);

        for (int i = tmp_pos+1; i < max_items; ++i) {
            // WEIGHT bounding
            if ((cw + inst->weight.at(i)) <= capacity)
                queue.push(NextItem(&tmp_item, i));
        }

    }

    // stores sum cost of all used items
    inst->sum_cost = tmp_best_cost;

    return tmp_best_distribution;
}

/**
 * Returns next generated item with defined new position.
 *
 * @param  inst_item  distribution of items in knapsack
 * @param  position   position of new added item
 * @return            new distribution of items in knapsack
 */
std::vector<bool> NextItem(std::vector<bool> * inst_item, int position) {
    std::vector<bool> tmp_inst = *inst_item;
    tmp_inst.at(position) = 1;

    return tmp_inst;
}

/**
 * Returns index of the last position of number one in vector of bools.
 *
 * @param   inst_item  distribution of items in knapsack
 * @return             position of last binary one
 */
int GetPosition(std::vector<bool> * inst_item) {
    int tmp_index = 0;
    int index = EMPTY;

    for (auto it = inst_item->begin(); it != inst_item->end(); ++it) {
        if (*it == 1)
            index = tmp_index;

        ++tmp_index;
    }

    return index;
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
 * Creates empty knapsack, binary vector of zeros.
 *
 * @param   size  size of kanpsack
 * @return        generated empty knapsack
 */
std::vector<bool> CreateEmptyKnapsack(int size) {
    std::vector<bool> knapsack;

    for (int i = 0; i < size; ++i) {
        knapsack.push_back(0);
    }

    return knapsack;
}

/**
 * Compute cost of items added to knapsack.
 *
 * @param   distribution  distribution of items in knapsack
 * @param   inst          holds information about costs and weights of items
 * @return                cost of all items in knapsack
 */
int ComputeCurrentCost(std::vector<bool> * distribution, Instance * inst) {
    int cost = 0;
    int index = 0;

    for (auto it = distribution->begin(); it != distribution->end(); ++it) {
        if (*it == 1) cost += inst->cost.at(index);

        ++index;
    }

    return cost;
}

/**
 * Compute cost of items which still can be added to knapsack.
 *
 * @param   distribution  distribution of items in knapsack
 * @param   inst          holds information about costs and weights of items
 * @return                cost of remaining items
 */
int ComputeRemainingCost(std::vector<bool> * distribution, Instance * inst) {
    int cost = 0;
    int index = 0;
    int offset = GetPosition(distribution) + 1;

    for (auto it = distribution->begin() + offset; it != distribution->end(); ++it) {
            cost += inst->cost.at(offset);

        ++offset;
    }

    return cost;
}

/**
 * Compute weight of items added to knapsack.
 *
 * @param   distribution  distribution of items in knapsack
 * @param   inst          holds information about costs and weights of items
 * @return                weight of all items in knapsack
 */
int ComputeCurrentWeight(std::vector<bool> * distribution, Instance * inst) {
    int weight = 0;
    int index = 0;

    for (auto it = distribution->begin(); it != distribution->end(); ++it) {
        if (*it == 1) weight += inst->weight.at(index);

        ++index;
    }

    return weight;
}
