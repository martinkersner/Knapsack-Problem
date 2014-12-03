/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 12/03/2014
 *
 * @section DESCRIPTION
 *
 * KNAPSACK PROBLEM 
 *
 * Tabu Search.
 */

#include "tabu_search.h"

int main(int argc, char **argv) {
    if (argc >= MIN_PARAM) {
        char * file_name = argv[1];
        int neighborhood_size = atoi(argv[2]);
        //int tabu_memory = atoi(argv[3]);

        auto inst = new Instances(file_name);

        // set seed
        srand(time(NULL));

        SolveTabuSearch(inst);
        //inst->PrintSolutions();


        delete inst;
    }


    return EXIT_SUCCESS;
}

/**
 * Solves given instances by tabu search method.
 *
 * @param   inst       different instances of knapsack problem
 */
void SolveTabuSearch(Instances * inst) {
    //int capacity = inst->GetCapacity();
    int max_volume = inst->GetVolume();
    //int max_items = inst->GetMaxItems();

    std::vector<Instance *> all_instances =  inst->GetAllInstances();

    // generate initial solutions
    auto population = InitializePopulation(20, 12); 
    PrintVectorBinaryVector(population);

    std::vector<bool> solution;

    for (auto inst_it = all_instances.begin(); inst_it != all_instances.end(); ++inst_it) {
      //for (auto inst_it : all_instances)
        solution = Evaluate(*inst_it, population, max_volume);
        //(*inst_it)->solution = solution;
    }
}

/**
 * Function operates the main parts of algorithm.
 */
std::vector<bool> Evaluate(Instance * inst, 
                           std::vector<std::vector<bool>> population, 
                           int max_volume) {
    std::vector<bool> solution;

    return solution;
}

/**
 * Generates randomly population consisting from potential solutions.
 */
Population InitializePopulation(size_t length, int size) {
    Population population;

    for (int i = 0; i < size; ++i) {
        std::vector<bool> tmp_bit_vector;

        // random generate potential solution
        tmp_bit_vector = GenerateBinaryVector(length);

        population.push_back(tmp_bit_vector);
    }

    return population;
}

/**
 * Generates all adajcent neighbors to given solution.
 *
 * @param  solution  examined solution
 * @return           all neighbors
 */
std::vector<std::vector<bool>> 
GetNeighbors(std::vector<bool> solution, std::vector<std::vector<bool>> tabu) {
    std::vector<std::vector<bool>> neighborhood;
    std::vector<bool> tmp_neighbor;
    int vector_size = solution.size();

    for (int i = 0; i < vector_size; ++i) {
        tmp_neighbor = FlipBit(solution, i);

        // compare with tabu list
        if (!IsTabu(tmp_neighbor, tabu))
            neighborhood.push_back(tmp_neighbor);
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
bool IsTabu(std::vector<bool> solution, std::vector<std::vector<bool>> tabu) {
    for (auto t : tabu)
        if (CompareBits(solution, t))
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
    if (vb1.size() == vb2.size()) {
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
void PrintVectorBinaryVector(std::vector<std::vector<bool>> bvv) {
    for (auto bv : bvv)
        PrintBinaryVector(bv);
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
 *
 */
std::vector<bool> BestNeighbor(Neighbors neighbors) {
}

/**
 *
 */
int EvaluateSolution(std::vector<bool> solution) {
    int ranking = 0;

    return ranking;
}
