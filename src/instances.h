/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 09/30/2014
 */

#ifndef INSTANCE_HH
#define INSTANCE_HH

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <stdlib.h>

/**
 * For branch and bound purposes.
 */
//struct InstanceItem {
//    int position;
//    std::vector<bool> knapsack_distribution;
//};

struct Instance {
    int id;                        // ID of instance
    std::vector<int> weight;       // weight of all items
    std::vector<int> cost;         // cost of all items
    std::vector<int> appr_cost;     // approximated cost of all items

    // auxiliary variables
    int sum_cost;                  // sum of all item costs 
    std::vector<bool> solution;    // solution for current items
    std::vector<float> heuristic;  // computed heuristic value (cost/weight)
};

class Instances {
    public:
        /**
         * Constructor. 
         *
         * Checks if given file name exist, parse lines of that file and creates instances.
         *
         * @param  file_name  name of file with instances
         */
        Instances(const char * file_name);

        /**
         * Destructor.
         *
         * Deallocates members of vector all_instances.
         */
        ~Instances();

        /**
         * Getter of max items which can be selected for knapsack.
         *
         * @return  number of items
         */
        int GetMaxItems();

        /**
         * Getter of knapsack volume.
         *
         * @return  volume
         */
        int GetVolume();

        /**
         * Getter of knapsack capacity.
         *
         * @return  capacity
         */
        int GetCapacity();

        /**
         * Getter of all instances needed to be solved.
         *
         * @return  all instances
         */
        std::vector<Instance *> GetAllInstances();

        /**
         * Prints solutions of all instances.
         */
        void PrintSolutions();

        /**
         * Compute heuristic for all instance values and sort them in decreasing order.
         */
        void ComputeHeuristic();

        /**
         * Sorts vector of float values.
         *
         * @param  values  vector of float values
         * @return         vector of indexes according to sorted values
         */
        std::vector<float> OrderVector(std::vector<float> const& values);

    private:
        /**
         * Checks if given file exists.
         *
         * @param   file_name  name of file
         * @return             bool existence of file
         */
        inline bool ExistFile(const char * file_name);

        /**
         * Parses line with space delimiter.
         *
         * @param  line  string containing words delimited by space character
         * @return       vector of strings
         */
        std::vector<std::string> * ParseLine(std::string line);

        /**
         * Created new instance and initialized its parameters.
         *
         * @param  params  parameters of instance stored in vector of strings 
         * @return         pointer to new instance
         */
        Instance * InitializeInstance(std::vector<std::string> * params);

        /**
         * Takes the first string value from vector of strings and converts it 
         * to integer. The first string value is then deleted from vector.
         *
         * @param  vector  pointer to string
         * @return         integer value
         */
        int TopPopFirst(std::vector<std::string> * vector);

        /**
         * Prints information about given Instance * inst.
         *
         * @param  inst  pointer to instance
         */
        void PrintInstance(Instance * inst);

        /**
         * Pushes Instance * inst to the end of vector all_instances, which stores
         * pointers to all instances.
         *
         * @param  inst  pointer to instance
         */
        inline void PushInstance(Instance * inst);


        std::vector<Instance *> all_instances;
        int max_items;  // number of items which can be chosen
        int volume;     // max volume of knapsack 
        int capacity;   // capacity of knapsack
};

#endif //INSTANCE_HH
