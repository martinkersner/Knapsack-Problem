#include "instances.h"

Instances::Instances(const char * name) {
    std::vector<std::string> * params = NULL;
    Instance * inst = NULL;

    if (ExistFile(name)) {
        std::ifstream file(name);
        std::string line; 

        while (std::getline(file, line))
        {
            params = ParseLine(line);
            inst = InitializeInstance(params);
            PushInstance(inst);
        }
    }
}

inline bool Instances::ExistFile(const char * name) {
    std::ifstream ifs(name);

    if (ifs.good()) {
        ifs.close();
        return true;
    } else {
        ifs.close();
        return false;
    }   
}

std::vector<std::string> * Instances::ParseLine(std::string line) {
    auto parsed_line = new std::vector<std::string>;
    std::string delimiter = " ";
    std::string token;
    size_t pos = line.find(delimiter);

    // TODO better way?
    line = line + delimiter;

    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        parsed_line->push_back(token);
        line.erase(0, pos + delimiter.length());
    } 

    return parsed_line;
}

Instance * Instances::InitializeInstance(std::vector<std::string> * params) {
    auto instance = new Instance;

    // ID, max number of items, size of knapsack
    instance->id = TopPopFirst(params);
    this->n = TopPopFirst(params);
    this->M = TopPopFirst(params);

    // weight. cost
    int w;
    int c;
    for (std::vector<std::string>::iterator it = params->begin() ; it != params->end(); it+=2) {
       w = atoi((*it).c_str());
       c = atoi((*(it+1)).c_str());

       instance->weight.push_back(w);
       instance->cost.push_back(c);
    }

    // count of items
    this->number_items = instance->weight.size();

    return instance;
}

int Instances::TopPopFirst(std::vector<std::string> * vector) {
    std::string element = vector->front();
    vector->erase(vector->begin());

    return atoi(element.c_str());
}

void Instances::PrintInstance(Instance * inst) {
    std::string space = " ";

    std::cout << inst->id << space;
    std::cout << this->n << space;
    std::cout << this->M;

    std::vector<int>::iterator w_it = inst->weight.begin();
    for (std::vector<int>::iterator c_it = inst->cost.begin() ; c_it != inst->cost.end(); ++c_it, ++w_it) {
        std::cout << space << *w_it << space << *c_it; 
    }

    std::cout << std::endl;
}

void Instances::PushInstance(Instance * inst) {
    this->all_instances.push_back(inst);
}

int Instances::GetNumberItems() {
    return this->number_items;
}

int Instances::GetMaxItem() {
    return this->n;
}

// capacity of knapsack
int Instances::GetMaxWeight() {
    return this->M;
}

std::vector<Instance *> Instances::GetAllInstances() {
    return this->all_instances;
}
