/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 12/28/2014
 *
 * @section DESCRIPTION
 *
 * SAT PROBLEM
 *
 * Tabu Search.
 */

#include "tabu_search_sat.h"

int main(int argc, char **argv) {
    if (argc > MIN_PARAM) {
        char * file_name = argv[1];

        Settings settings;
        settings.neighborhoodSize = 1;
        settings.populationLength = 1;
        settings.numberIterations = 5;
        settings.duration = 10;

        SatInstance inst = SatInstance(file_name);

        settings.solutionSize = inst.GetLength();

        // set seed
        srand(1);
        //srand(time(NULL));

        SolveTabuSearch(inst, settings);

        //inst->PrintSolutions();
    }

    return EXIT_SUCCESS;
}

/**
 * Solves given instances by tabu search method.
 *
 * @param   inst                different instances of knapsack problem
 * @param   neighborhood_size
 */
void SolveTabuSearch(SatInstance & inst, 
                     Settings & settings) {

    int solutionLength = inst.GetLength();
    std::vector<bool> solution;

    // generate initial solutions
    auto population = InitializePopulation(settings); 
    auto tabu = InitializeTabu();

    solution = Evaluate(inst, population, tabu, settings);
//    (*inst_it)->solution = solution;
}

/**
 * Function operates the main parts of algorithm.
 */
std::vector<bool> 
Evaluate(SatInstance & inst, 
         Population population, 
         std::vector<Tabu> tabu,
         Settings & settings) {

    // create binary solution
    int solutionLength = inst.GetLength();
    std::vector<bool> solution = ZeroSolution(solutionLength);

    // create temporary best state 
    State bestState = CreateState(solution);

    State tmp_solution;
    std::vector<State> neighbors;

    Population tmp_population = population;
    Population tmp_next_population;

    int numberIterations = settings.numberIterations;
    int duration = settings.duration;

     for (int i = 0; i < numberIterations; ++i ) {
         // add it to tabu list
         // search neighborhood
         // select best solution
         // update population

         AddTabu(tmp_population, tabu, duration);
         tmp_next_population.clear();

         for (auto popu_it : tmp_population) {
             neighbors = GetNeighbors(popu_it, tabu, inst, settings);
             tmp_solution = BestNeighbor(inst, neighbors);
             tmp_next_population.push_back(tmp_solution.solution);

             // find the best temporary solution
             bestState = CompareStates(tmp_solution, bestState);
         }

         tmp_population = tmp_next_population;

         // clear tabu
         ClearTabu(tabu);
     }

//     // set the best cost of particular knapsack configuration
//     inst->sum_cost = GetSumCost(inst, solution);

     return solution;
}

/**
 * Generates randomly population consisting from potential solutions.
 */
Population InitializePopulation(Settings & settings) {
    Population population;
    int populationLength = settings.populationLength;
    int solutionSize = settings.solutionSize;

    for (int i = 0; i < populationLength; ++i) {
        std::vector<bool> tmp_bit_vector;

        // random generate potential solution
        tmp_bit_vector = GenerateBinaryVector(solutionSize);

        population.push_back(tmp_bit_vector);
    }

    return population;
}

/**
 * Generates all adjacent neighbors to given solution.
 *
 * @param  solution  examined solution
 * @return           all neighbors
 */
std::vector<State> 
GetNeighbors(std::vector<bool> & solution, 
             std::vector<Tabu> & tabu,
             SatInstance & inst,
             Settings & settings) {

    std::vector<State> neighborhood;

    std::vector<bool> neighbor;
    State state;
    int vector_size = solution.size();

    for (int i = 0; i < vector_size; ++i) {
        neighbor = FlipBit(solution, i);

        // compare with tabu list
        if (!IsTabu(neighbor, tabu)) {
            AddOneTabu(neighbor, tabu, settings.duration);
            state = SolveBooleanFormula(inst, neighbor);

            // TODO review
            neighborhood.push_back(state);
        }
    }

    return neighborhood;
}

/**
 * Controls if given potential solution is in tabu list.
 *
 * @param  tabu      tabu list
 * @param  solution  examined potential solution
 * @return           boolean variable answering the question
 */
bool IsTabu(std::vector<bool> solution, std::vector<Tabu> tabu) {
    for (auto t : tabu)
        if (CompareBits(solution, t.solution))
            return true;

    return false;
}

/**
 * Compares two bit vectors and determines if they are identical.
 *
 * @param  vb1  the first vector of bool values
 * @param  vb2  the second vector of bool values
 * @return      
 */
bool CompareBits(std::vector<bool> vb1, std::vector<bool> vb2) {
    // check the size of vectors
    if (vb1.size() != vb2.size()) {
        std::cerr << "The size of vector is not equal!" << std::endl;
        exit(NOT_EQUAL);
    }

    // gradual controling bits in vector
    auto vb2_it = vb2.begin();
    for (auto vb1_it = vb1.begin(); vb1_it != vb1.end(); ++vb1_it, ++vb2_it) {
        if (*vb1_it != *vb2_it)
            return false;
    }

    return true;
}

/**
 * Flips binary value of binary vector at particular position.
 *
 * @param  vb        binary vector
 * @param  position  position of bit, which changes
 * @return           changed binary vector
 */
std::vector<bool> FlipBit(std::vector<bool> vb, int position) {
    if (vb.at(position) == 1)
        vb.at(position) = 0;
    else
        vb.at(position) = 1;

    return vb;
}

/**
 * Prints binary vector on standard output.
 *
 * @param  bv  binary vector
 */
void PrintBinaryVector(std::vector<bool> bv) {
    for (auto it = bv.begin(); it != bv.end(); ++it)
        std::cout << *it;

    std::cout << std::endl;
}

/**
 * Print vector of binary vectors.
 */
//void PrintVectorBinaryVector(std::vector<std::vector<bool>> bvv) {
//    for (auto bv : bvv)
//        PrintBinaryVector(bv);
//}

/**
 * Generates random binary vector.
 */
std::vector<bool> GenerateBinaryVector(int length) {
    std::vector<bool> bv;

    for (int i = 0; i < length; ++i)
        bv.push_back(rand()%2);

    return bv;
}

/**
 * Selects the best neighbor from given neighborhood.
 */
State
BestNeighbor(SatInstance & inst, std::vector<State> & neighbors) {

    State bestSolution;

    // keep eye on
    // set best solution as the empty binary vector
    if (neighbors.size() == 0) {
        std::vector<bool> tmpSolution = ZeroSolution(inst.GetLength()); 
        bestSolution = CreateState(tmpSolution); 
    }
    else
        bestSolution = neighbors.at(0);

    // searching for the best neighbor among the rest of unreviewed members
    int bestWeight = 0;
    for (auto neighbor : neighbors) {
        if (bestWeight < neighbor.weightSum) {
            bestWeight = neighbor.weightSum;
            bestSolution = neighbor;
        }
    }

    return bestSolution;
}

State SolveBooleanFormula(SatInstance & inst, std::vector<bool> & solution) {
    State state = CreateState(solution);
    bool result;
    bool violated = false;

    for (auto bf_it : inst.formula) {
        result = SolveClause(bf_it, solution);

        // TODO different strategy?
        if (!result) {
            state.numberViolated++;
            violated = true;
        }
    }

    if (!violated)
        state.weightSum = CountWeights(solution, inst.weights);
    else
        state.weightSum = INVALID_SOLUTION;

    return state;
}

bool SolveClause(Clause & c, std::vector<bool> & solution) {
    bool result = false;
    bool tmp_bool;

    for (auto c_it : c) {
        tmp_bool = EvaluateBool(c_it, solution);
        result = Or(result, tmp_bool);
    }

    return result;
}

bool Or(bool b1, bool b2) {
    if (b1 == true || b2 == true)
        return true;
    else
        return false;
}

bool EvaluateBool(int p, std::vector<bool> & solution) {
    bool b = solution.at(std::abs(p)-1);

    if (p < 0)
        return FlipBool(b);
    else
        return b;
}

bool FlipBool(bool b) {
    if (b == true)
        return false;
    else
        return true;
}

///**
// * Computes cost of given solution.
// */
//int EvaluateSolution2(Instance & inst, std::vector<bool> & solution) {
//    int cost = 0;
//
//    auto sol_it = solution.begin();
//    for (auto c_it = inst.cost.begin() ; c_it != inst.cost.end(); ++c_it, ++sol_it)
//        if (*sol_it == true)
//           cost += *c_it;
//
//    return cost;
//}

/**
 * Clears states from tabu list after tabu interval expires.
 */
void ClearTabu(std::vector<Tabu> & tabu) {
    std::vector<std::vector<Tabu>::iterator> tabu_delete;

    for (auto tabu_it = tabu.begin(); tabu_it != tabu.end(); ++tabu_it) {

        tabu_it->duration--;
        if (tabu_it->duration <= 0)
            tabu_delete.push_back(tabu_it);

    }

    // delete tabu
    for (auto tabu_del_it : tabu_delete)
        tabu.erase(tabu_del_it);
}

/**
 * Creates tabu list.
 */
std::vector<Tabu> InitializeTabu() {
    std::vector<Tabu> tabu;
    return tabu;
}

/**
 * Adds all population (all best solutions) to tabu list.
 */
void AddTabu(Population & population, 
             std::vector<Tabu> & tabu, 
             int duration) {

    Tabu tmp_tabu;

    for (auto pop_it : population) {
        if (!IsTabu(pop_it, tabu)) {
            tmp_tabu = CreateTabu(pop_it, duration);
            tabu.push_back(tmp_tabu);
        }
    }
}

/**
 * Adds one solution to tabu list.
 */
void AddOneTabu(std::vector<bool> solution, 
                std::vector<Tabu> & tabu,
                int duration) {

    Tabu tmp_tabu;
    tmp_tabu = CreateTabu(solution, duration);
    tabu.push_back(tmp_tabu);
}

/**
 * Creates and fills Tabu struct.
 */
Tabu CreateTabu(std::vector<bool> solution, int duration) {
    Tabu tabu;
    tabu.solution = solution;
    tabu.duration = duration;
    return tabu;
}

/**
 * Finds the best solution in given vector of solutions.
 */
//void BestSolution(Population & solutions, 
//                  Instance & inst, 
//                  std::vector<bool> & best_solution) {
//
//    int best_cost = EvaluateSolution2(inst, best_solution);
//    int tmp_cost;
//
//    for (auto best_it : solutions) {
//        tmp_cost= EvaluateSolution2(inst, best_it);
//
//        if (tmp_cost > best_cost) {
//            best_cost = tmp_cost;
//            best_solution = best_it;
//        }
//    }
//}

///**
// * Sums real costs of output configuration.
// *
// * @param   inst
// * @param   solution  
// * @return            sum of costs from final configuration
// */
//int GetSumCost(Instance * inst, std::vector<bool> solution) {
//    int sum_cost = 0;
//
//    auto b_it = solution.begin();
//    for (auto it = inst->cost.begin(); it != inst->cost.end(); ++it, ++b_it) {
//        if (*b_it)
//            sum_cost += *it;
//    }
//
//    return sum_cost;
//}

/**
 *  Creates vector of zeros of given length.
 */
std::vector<bool> ZeroSolution(int length) {
    std::vector<bool> zeros;

    for (int i = 0; i < length; ++i ) {
        zeros.push_back(0);
    }

    return zeros;
}

///**
// * Prints sum of weights from items in knapsack.
// */
//void PrintWeights(Instance * inst, std::vector<bool> & solution) {
//    int weight = 0;
//
//    auto sol_it = solution.begin();
//    for (auto w_it = inst->weight.begin() ; w_it != inst->weight.end(); ++w_it, ++sol_it) {
//        if (*sol_it == 1)
//            weight += *w_it;
//    }
//
//    std::cout << weight << std::endl;
//}

State CompareStates(State & s1, State & s2) {
    if (s2.weightSum == INVALID_SOLUTION && s1.weightSum == INVALID_SOLUTION)
        return LowerViolated(s1, s2);
    else if (s1.weightSum == INVALID_SOLUTION && s2.weightSum > 0)
        return s2;
    else if (s2.weightSum == INVALID_SOLUTION && s1.weightSum > 0)
        return s1;
    else if (s2.numberViolated < s1.numberViolated)
        return s2;
    else
        return s1;
}

State LowerViolated(State & s1, State & s2) {
    if (s1.numberViolated < s2.numberViolated)
        return s1;
    else
        return s2;
}

State CreateState(std::vector<bool> & solution) {
    State s;
    s.solution = solution;
    s.numberViolated = 0;
    s.weightSum = INVALID_SOLUTION;

    return s;
}

int CountWeights(std::vector<bool> & solution, std::vector<int> & weights) {
    int weightSum;

    auto w_it = weights.begin();
    for (auto s_it : solution) {
        if (s_it)
            weightSum += *w_it;

        w_it++;    
    }

    return weightSum;
}
