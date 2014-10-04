/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 09/30/2014
 *
 * @section DESCRIPTION
 *
 * KNAPSACK PROBLEM 
 *
 * Brute force solution.
 */

#include "brute_force.h"

int main(int argc, char** argv) {
    
    if (argc >= MIN_PARAM) {
        char * file_name = argv[1];
        auto inst = new Instances(file_name);
        SolveBruteForce(inst);
        inst->PrintSolutions();

        delete inst;
    }

    return EXIT_SUCCESS;
}

/**
 * Solves knapsack problem using brute force.
 * @todo   add return value for printing solutions
 *
 * @param  inst  all loaded instances
 */
void SolveBruteForce(Instances * inst) {
    int capacity = inst->GetCapacity();
    int volume = inst->GetVolume();
    int max_items = inst->GetMaxItems();
    std::vector<Instance *> all_instances =  inst->GetAllInstances();

    std::vector<std::vector<bool>> * combinations = GenerateCombinations(max_items, volume);
    std::vector<bool> solution;

    for (auto inst_it = all_instances.begin(); inst_it != all_instances.end(); ++inst_it) {
        solution = Evaluate(*inst_it, combinations, capacity);
        (*inst_it)->solution = solution;
    }

    delete combinations;
}

/**
 * Generate all possible combinations.
 *
 * @param len       length of vector
 * @param max_item  maximum number of 1's
 * @return          all_combinations
 */
std::vector<std::vector<bool>> * GenerateCombinations(int len, int max_item) {
    auto combinations = new std::vector<std::vector<bool>>;

    for (int i = 1; i <= max_item; ++i) {
        std::vector<bool> v(len);
        std::fill(v.begin() + len - i, v.end(), true);

        do {
            combinations->push_back(v);
        } while (std::next_permutation(v.begin(), v.end()));
    }

    return combinations;
}


std::vector<bool> Evaluate(Instance * inst, std::vector<std::vector<bool>> * comb, int knapsack_capacity) {
    std::vector<bool> solution;
    int min_cost = 0;
    Evaluation ev;

    for (auto comb_it = comb->begin() ; comb_it != comb->end(); ++comb_it) {
        ev = EvaluateCombination(inst, &(*comb_it));

        if (ev.weight <= knapsack_capacity) 
           if (ev.cost >= min_cost) {
               solution = *comb_it;
               min_cost = ev.cost;
           }
    }

    // stores sum of all weight
    inst->sum_cost = min_cost;

    return solution;
}

Evaluation EvaluateCombination(Instance * inst, std::vector<bool> * comb) {
    Evaluation ev;
    ev.cost = 0;
    ev.weight = 0;

    auto comb_it = comb->begin();
    auto w_it = inst->weight.begin();
    for (auto c_it = inst->cost.begin() ; c_it != inst->cost.end(); ++c_it, ++w_it, ++comb_it) {
        if (*comb_it == true) {
           ev.cost += *c_it; 
           ev.weight += *w_it; 
        }
    }

    return ev;
}

void PrintVector(std::vector<bool> * vec) {
    for (std::vector<bool>::iterator it=vec->begin(); it!=vec->end(); ++it)
        std::cout << *it << " ";

    std::cout << std::endl;
}
