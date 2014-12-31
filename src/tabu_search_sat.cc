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

// Settings od different strategies
#define EXACT_ITERATION
#define TABU_CLAUSE

int main(int argc, char **argv) {

    if (!UnitTests()) return EXIT_FAILURE;
    
    if (argc > MIN_PARAM) {
        char * fileName = argv[1];

        Settings settings;
        settings.neighborhoodSize = 1;
        settings.populationLength = 2;
        settings.duration = 5; // related to tabu list

        // * Either the definite number of iterations
        // * or if solution does not change for good within this time span, iteration is
        // then terminated
        settings.numberIterations = 30;

        SatInstance inst = SatInstance(fileName);

        settings.solutionSize = inst.GetLength();

        // set seed
        //srand(1);
        srand(time(NULL));

        SolveTabuSearch(inst, settings);
    }

    return EXIT_SUCCESS;
}

/**
 * Solves given instance by tabu search method.
 */
void SolveTabuSearch(SatInstance & inst, 
                     Settings & settings) {

    int solutionLength = inst.GetLength();
    std::vector<bool> solution;

    // generate initial solutions
    auto population = InitializePopulation(settings); 
    auto tabu = InitializeTabu();

    Evaluate(inst, population, tabu, settings);
}

/**
 * Function operates the main parts of algorithm.
 */
void Evaluate(SatInstance & inst, 
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

    // auxiliary variables related to terminating iterations
    int tmpWeightSum = 0;
    int recentChange = 0;

    std::vector<TabuClause> vectorTabuClause;

     int iteration = 0;
#ifdef EXACT_ITERATION
     for (int i = 0; i < numberIterations; ++i ) {
#else
     while (1) {
#endif
         // add it to tabu list
         // search neighborhood
         // select best solution
         // update population

         tmp_next_population.clear();

         for (auto popu_it : tmp_population) {
             neighbors = GetNeighbors(popu_it, tabu, vectorTabuClause, inst, settings);
             tmp_solution = BestNeighbor(inst, neighbors);
             tmp_next_population.push_back(tmp_solution.solution);

             // find the best temporary solution
             bestState = CompareStates(tmp_solution, bestState);
         }

         tmp_population = tmp_next_population;

         // CLEARING TABU LIST
#ifdef TABU_CLAUSE
         ClearTabuClause(vectorTabuClause);
#else         
         ClearTabu(tabu);
#endif

         // terminating of iterations
         if (tmpWeightSum == bestState.weightSum && bestState.weightSum != -1) {
             if (++recentChange > numberIterations)
                 break;
         }
         else {
             tmpWeightSum = bestState.weightSum;
             recentChange = 0;
         }

         ++iteration;
         assert(iteration <= 100);
     }

     // print results
     std::cout << "IT: " << iteration << std::endl;
     PrintBest(bestState);
     PrintSuboptimal(inst);
     PrintMaxWeight(inst);
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
             std::vector<TabuClause> & vectorTabuClause,
             SatInstance & inst,
             Settings & settings) {

    std::vector<State> neighborhood;

    std::vector<bool> neighbor;
    State state;
    int vectorSize = solution.size();

    std::vector<std::vector<int>> 
    combinations = CreateFlipCombinations(vectorSize,
                                          settings.neighborhoodSize);

    for (auto comb_it : combinations) {
        neighbor = FlipPartBits(solution, comb_it);

#ifdef TABU_CLAUSE
        if (!IsSolutionInTabuClause(neighbor, vectorTabuClause)) {
#else
        if (!IsTabu(neighbor, tabu)) {
            AddOneTabu(neighbor, tabu, settings.duration);
#endif
            state = SolveBooleanFormula(inst, 
                                        neighbor, 
                                        vectorTabuClause, 
                                        settings.duration);

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

    for (auto neighbor : neighbors)
        bestSolution = CompareStates(bestSolution, neighbor);

    return bestSolution;
}

State SolveBooleanFormula(SatInstance & inst, 
                          std::vector<bool> & solution,
                          std::vector<TabuClause> & vectorTabuClause,
                          int duration) {

    State state = CreateState(solution);
    bool result;
    bool violated = false;

    // tabu clause
    int size = solution.size();
    TabuClause tabuClause;

    for (auto bf_it : inst.formula) {
        result = SolveClause(bf_it, solution);

        if (!result) {
            state.numberViolated++;
            violated = true;

#ifdef TABU_CLAUSE
            tabuClause = CreateTabuClause(bf_it, size, duration);
            AddTabuClause(vectorTabuClause, tabuClause);
#endif
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
 * Clears tabu clause after tabu interval expires.
 */
void ClearTabuClause(std::vector<TabuClause> & tabu) {
    std::vector<std::vector<TabuClause>::iterator> tabu_delete;

    for (auto tabu_it = tabu.begin(); tabu_it != tabu.end(); ++tabu_it) {

        tabu_it->duration--;
        if (tabu_it->duration <= 0) { tabu_delete.push_back(tabu_it); }
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
 *  Creates vector of zeros of given length.
 */
std::vector<bool> ZeroSolution(int length) {
    std::vector<bool> zeros;

    for (int i = 0; i < length; ++i ) {
        zeros.push_back(0);
    }

    return zeros;
}

State CompareStates(State & s1, State & s2) {
    if (s2.weightSum == INVALID_SOLUTION && s1.weightSum == INVALID_SOLUTION)
        return LowerViolated(s1, s2);
    else if (s1.weightSum == INVALID_SOLUTION && s2.weightSum > 0)
        return s2;
    else if (s2.weightSum == INVALID_SOLUTION && s1.weightSum > 0)
        return s1;
    else if (s2.weightSum > s1.weightSum)
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
    int weightSum = 0;

    auto w_it = weights.begin();
    for (auto s_it : solution) {
        if (s_it)
            weightSum += *w_it;

        w_it++;    
    }

    return weightSum;
}


void PrintBest(State & best) {
    for (auto s : best.solution)
        std::cout << s;

    std::cout << std::endl;

    std::cout << "WS: " << best.weightSum << std::endl;
    std::cout << "NV: " << best.numberViolated << std::endl;
}

void PrintSuboptimal(SatInstance & inst) {
    for (auto s : inst.GetSuboptimalSolution())
        std::cout << s;

    std::cout << std::endl;

    std::cout << "WS: " << inst.GetSuboptimalWeightSum() << std::endl;
}

void PrintMaxWeight(SatInstance & inst) {
    std::cout << "MAX WS: " << inst.GetMaxWeightSum() << std::endl;
}

bool UnitTests() {
    bool result = true;
    std::vector<TabuClause> vectorTabuClause;
    int d = 0;

    // Unit 0
    SatInstance si = SatInstance("../data-sat/sat0.dat");

    std::vector<bool> s1 = {0, 0, 0, 1};
    State u1 = SolveBooleanFormula(si, s1, vectorTabuClause, d);
    result &= UnitTestEvaluate(1, u1, 6, 0);

    std::vector<bool> s2 = {1, 0, 0, 1};
    State u2 = SolveBooleanFormula(si, s2, vectorTabuClause, d);
    result &= UnitTestEvaluate(2, u2, 8, 0);

    std::vector<bool> s3 = {1, 1, 1, 0};
    State u3 = SolveBooleanFormula(si, s3, vectorTabuClause, d);
    result &= UnitTestEvaluate(3, u3, 7, 0);

    std::vector<bool> s4 = {1, 1, 1, 1};
    State u4 = SolveBooleanFormula(si, s4, vectorTabuClause, d);
    result &= UnitTestEvaluate(4, u4, INVALID_SOLUTION, 1);

    // Unit 1
    si = SatInstance("../data-sat/sat1.dat");
    std::vector<bool> s5 = {0, 1, 1, 0, 1, 0};
    State u5 = SolveBooleanFormula(si, s5, vectorTabuClause, d);
    result &= UnitTestEvaluate(5, u5, 11, 0);

    // Unit 2
    si = SatInstance("../data-sat/sat2.dat");
    std::vector<bool> s6 = {0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1};
    State u6 = SolveBooleanFormula(si, s6, vectorTabuClause, d);
    result &= UnitTestEvaluate(6, u6, 43, 0);

    // Unit 3
    si = SatInstance("../data-sat/sat3.dat");
    std::vector<bool> s7 = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};
    State u7 = SolveBooleanFormula(si, s7, vectorTabuClause, d);
    result &= UnitTestEvaluate(7, u7, 77, 0);

    return result;
}

/** 
 * Evaluate success of particular unit test.
 */
bool UnitTestEvaluate(int id, 
                      State & s, 
                      int weightSum, 
                      int numberViolated) {

    if (s.weightSum != weightSum || s.numberViolated != numberViolated) {
        std::cerr << "UNIT TEST " << id << ": FAIL" << std::endl;
        return false;
    }
    else {
        std::cerr << "UNIT TEST " << id << ": OK" << std::endl;
        return true;
    }
}

/**
 * Checks if binaryVector contains pattern at particular positions.
 */
bool EqualTabuClause(TabuClause & input, 
                     TabuClause & pattern) {
    
    if (!CompareBits(input.position, pattern.position))
        return false;

    bool equal = true;
    
    auto in_it = input.clause.begin();
    auto pa_it = pattern.clause.begin();

    for (auto pos_it : input.position) {
       if (pos_it == 1) {
           if (*in_it == *pa_it)
               equal &= true;
           else
               equal &= false;
       }

       in_it++;
       pa_it++;
    }

    return equal;
}

/**
 * Creates binary vector which determines particular clause.
 */
std::vector<bool> 
CreateClauseBinaryVector(std::vector<int> & positions, 
                         int length) {

    std::vector<bool> positionBinaryVector = ZeroSolution(length);

    for (auto p_it : positions)
        if (p_it > 0) { positionBinaryVector.at(std::abs(p_it)-1) = 1; }

    return positionBinaryVector;
}

/**
 * Creates binary vector in which the ones determines positions in binary vector.
 */
std::vector<bool> 
CreatePositionBinaryVector(std::vector<int> & positions, 
                           int length) {

    std::vector<bool> positionBinaryVector = ZeroSolution(length);

    for (auto p_it : positions)
        positionBinaryVector.at(std::abs(p_it)-1) = 1;

    return positionBinaryVector;
}

/**
 * Compares given clause tabu clauses.
 */
bool IsTabuClause(std::vector<TabuClause> & tabu, 
                  TabuClause & clause) {

    for (auto t_it : tabu)
       if (EqualTabuClause(t_it, clause))
           return true;

    return false;
}

/**
 * Controls if tabu contains given clause. If not clause is added to tabu list.
 */
void AddTabuClause(std::vector<TabuClause> & tabu, 
                   TabuClause & clause) {
    
    if (!IsTabuClause(tabu, clause))
        tabu.push_back(clause);
}

/**
 * Creates a tabu clause with all its attributes and intialize them.
 */
TabuClause 
CreateTabuClause(std::vector<int> & clause,
                 int size,
                 int duration) {

    TabuClause tc;
    tc.clause = CreateClauseBinaryVector(clause, size);
    tc.position = CreatePositionBinaryVector(clause, size);
    tc.duration = duration;

    return tc;
}

/**
 * Checks if given solution doesnt consist from tabu clauses.
 */
bool IsSolutionInTabuClause(std::vector<bool> solution,
                            std::vector<TabuClause> tabuClause) {

    TabuClause simpleTc;

    for (auto tc_it : tabuClause) {
        simpleTc = CreateSimpleTabuClause(solution, tc_it.position, 0);

        if (EqualTabuClause(simpleTc, tc_it)) 
            return true;
    }

    return false;
}

TabuClause 
CreateSimpleTabuClause(std::vector<bool> & clause,
                       std::vector<bool> & position, 
                       int duration) {

    TabuClause tc;
    tc.clause = clause;
    tc.position = position;
    tc.duration = duration;

    return tc;
}

/**
 * Creates combinations of given size.
 */
void comb(int n, 
          int r, 
          int *arr, 
          int sz,
          std::vector<std::vector<int>> & positions) {

    std::vector<int> tmpPositions;

    for (int i = n; i >= r; i--) {
        // choose the first element
        arr[r - 1] = i-1;
        if (r > 1) { // if still needs to choose
            // recursive into smaller problem
            comb(i - 1, r - 1, arr, sz, positions);
        } 
        else {
            // print out one solution
            for (int i = 0; i < sz; i ++)
                tmpPositions.push_back(arr[i]);
            
            positions.push_back(tmpPositions);
            tmpPositions.clear();
        }
    }
}

std::vector<std::vector<int>> 
CreateFlipCombinations(int length, 
                       int neighborhoodSize) {
    int N = length;
    int M = neighborhoodSize;
    int *arr = new int[M];
    std::vector<std::vector<int>> positions;

    comb(N, M, arr, M, positions);

    delete[] arr;

    return positions;
}

std::vector<bool> FlipPartBits(std::vector<bool> solution,
                               std::vector<int> combinations) {

    std::vector<bool> flippedSolution = solution;

    for (auto comb_it : combinations)
        flippedSolution = FlipBit(flippedSolution, comb_it);

    return flippedSolution;
}
