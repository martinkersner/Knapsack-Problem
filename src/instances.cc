/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 10/01/2014
 *
 * @section DESCRIPTION
 *
 * KNAPSACK PROBLEM 
 *
 * Class for handling with instances. Reading file with instances, initializing, 
 * reading, etc.
 */

#include "instances.h"

/**
 * Constructor.
 *
 * Checks if given file name exist, parse lines of that file and creates instances.
 *
 */
Instances::Instances(const char * file_name) {
    std::vector<std::string> * params = NULL;
    Instance * inst = NULL;

    if (ExistFile(file_name)) {
        // read lines from file
        std::ifstream file(file_name);
        std::string line; 

        // parse line and create instances
        while (std::getline(file, line))
        {
            params = ParseLine(line);
            inst = InitializeInstance(params);
            PushInstance(inst);
            delete params;
        }
    }
}

/**
 * Destructor.
 * 
 * Deallocates members of vector all_instances.
 */
Instances::~Instances() {
    for (auto it = this->all_instances.begin() ; it != this->all_instances.end(); ++it)
        delete *it;
}

/**
 * Getter of max items which can be selected for knapsack.
 */
int Instances::GetMaxItems() {
    return this->max_items;
}

/**
 * Getter of knapsack volume.
 */
int Instances::GetVolume() {
    return this->volume;
}

/**
 * Getter of knapsack capacity.
 */
int Instances::GetCapacity() {
    return this->capacity;
}

/**
 * Getter of all instances needed to be solved.
 */
std::vector<Instance *> Instances::GetAllInstances() {
    return this->all_instances;
}

/**
 * Prints solutions of all instances.
 */
void Instances::PrintSolutions() {
    Instance * inst = NULL;
    std::string space = " ";

    for (auto inst_it = this->all_instances.begin(); inst_it != this->all_instances.end(); ++inst_it) {
        inst = (*inst_it);

        // ID, total number of items, total cost
        std::cout << inst->id << space;
        std::cout << inst->solution.size() << space;
        std::cout << inst->sum_cost << space;

        // binary solution
        for (auto sol_it = inst->solution.begin(); sol_it != inst->solution.end(); ++sol_it)
            std::cout << space << *sol_it;

        // new line
        std::cout << std::endl;
    }
}

/**
 * Compute heuristic for all instance values and sort them in decreasing order.
 */
void Instances::ComputeHeuristic() {
    int len;
    Instance * inst = NULL;
    std::vector<int>::iterator cost_it;
    int h;

    for (auto inst_it = this->all_instances.begin(); inst_it != this->all_instances.end(); ++inst_it) {
        inst = *inst_it;
        len = inst->weight.size();

        cost_it = inst->cost.begin();
        for (auto weight_it= inst->weight.begin(); weight_it < inst->weight.end();  ++weight_it, ++cost_it) {
            h = (*cost_it) / (*weight_it);
            inst->heuristic.push_back(h);
        }
    }
}

/**
 * Sorts vector of float values.
 */
std::vector<float> Instances::OrderVector(std::vector<float> const& values) {
    std::vector<float> indices(values.size());
    std::iota(begin(indices), end(indices), static_cast<float>(0));

    std::sort(
            begin(indices), end(indices),
            [&](float a, float b) { return values[a] < values[b]; }
            );

    return indices;
}

/**
 * Checks if given file exists.
 */
inline bool Instances::ExistFile(const char * file_name) {
    std::ifstream ifs(file_name);

    if (ifs.good()) {
        ifs.close();
        return true;
    } else {
        ifs.close();
        return false;
    }   
}

/**
 * Parses line with space delimiter.
 */
std::vector<std::string> * Instances::ParseLine(std::string line) {
    auto parsed_line = new std::vector<std::string>;
    std::string delimiter = " ";
    std::string token;
    size_t pos = line.find(delimiter);

    // workaround which enables to get all words from whole line
    line = line + delimiter;

    // parsing
    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        parsed_line->push_back(token);
        line.erase(0, pos + delimiter.length());
    } 

    return parsed_line;
}

/**
 * Created new instance and initialized its parameters.
 */
Instance * Instances::InitializeInstance(std::vector<std::string> * params) {
    auto instance = new Instance;

    // initializing ID, max number of items, size of knapsack
    instance->id = TopPopFirst(params);
    this->volume = TopPopFirst(params);
    this->capacity = TopPopFirst(params);

    // initializing weight and cost of items
    int w;
    int c;
    for (auto it = params->begin() ; it != params->end(); it+=2) {
       w = atoi((*it).c_str());
       c = atoi((*(it+1)).c_str());

       instance->weight.push_back(w);
       instance->cost.push_back(c);
    }

    // number of items which can be chosen 
    this->max_items = instance->weight.size();

    return instance;
}


/**
 * Takes the first string value from vector of strings and converts it 
 * to integer. The first string value is then deleted from vector.
 */
int Instances::TopPopFirst(std::vector<std::string> * vector) {
    std::string element = vector->front();
    vector->erase(vector->begin());

    return atoi(element.c_str());
}

/**
 * Prints information about given Instance * inst.
 */
void Instances::PrintInstance(Instance * inst) {
    std::string space = " ";

    // priting ID, volume and capacity of knapsack
    std::cout << inst->id << space;
    std::cout << this->volume << space;
    std::cout << this->capacity;

    // printing weights and costs
    std::vector<int>::iterator w_it = inst->weight.begin();
    for (auto c_it = inst->cost.begin() ; c_it != inst->cost.end(); ++c_it, ++w_it) {
        std::cout << space << *w_it << space << *c_it; 
    }

    // new line
    std::cout << std::endl;
}

/**
 * Pushes Instance * inst to the end of vector all_instances, which stores
 * pointers to all instances.
 */
inline void Instances::PushInstance(Instance * inst) {
    this->all_instances.push_back(inst);
}


