#ifndef INSTANCE_HH
#define INSTANCE_HH

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <stdlib.h>

struct Instance {
    int id; // ID of instance
    std::vector<int> weight;
    std::vector<int> cost;

    int sum_cost;
    std::vector<bool> solution;
};

class Instances {
    public:
        Instances(const char *);
        int GetNumberItems();
        int GetMaxItem();
        int GetMaxWeight();
        std::vector<Instance *> GetAllInstances();

    private:
        inline bool ExistFile(const char *);
        std::vector<std::string> * ParseLine(std::string);
        Instance * InitializeInstance(std::vector<std::string> *);
        int TopPopFirst(std::vector<std::string> *);
        void PrintInstance(Instance *);
        void PushInstance(Instance *);

        std::vector<Instance *> all_instances;
        int number_items;
        int n;  // maximum number of items
        int M;  // capacity of knapsack
};

#endif //INSTANCE_HH
