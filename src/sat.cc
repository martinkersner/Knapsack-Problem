/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 12/27/2014
 *
 * @section DESCRIPTION
 *
 * LOADING SAT INSTANCES
 *
 */

#include "sat.h"

SatInstance::SatInstance(const char * file_name) {
    std::vector<std::string> * params = NULL;
    SatInstance * inst = NULL;

    if (ExistFile(file_name)) {
        // read lines from file
        std::ifstream file(file_name);
        std::string line; 

        ReadHead(file);
        this->weights = ReadWeights(file);
        this->formula = ReadBooleanFormula(file);
        this->length = weights.size();
    }
    else {
        std::cerr << "Given file does not exist!" << std::endl;
        exit(FILE_NOT_EXIST);
    }
}

SatInstance::~SatInstance() {

}

inline bool SatInstance::ExistFile(const char * file_name) {
    std::ifstream ifs(file_name);

    if (ifs.good()) {
        ifs.close();
        return true;
    } else {
        ifs.close();
        return false;
    }   
}

// Simply skips the first rows containing head.
void SatInstance::ReadHead(std::ifstream & file) {
    std::string line;
    for (int i = 0; i < 4; ++i)
       std::getline(file, line);
}

std::vector<int> SatInstance::ReadWeights(std::ifstream & file) {
    std::string line;
    std::getline(file, line);

    return ParseWeights(line);
}

std::vector<int> SatInstance::ParseWeights(std::string & line) {
    std::vector<int> parsedWeights;
    std::string delimiter = " ";
    std::string token;

    // delete the name of line
    size_t pos = line.find(delimiter);
    assert(line.substr(0, pos) == "w");
    line.erase(0, pos + delimiter.length());

    // workaround which enables to get all words from whole line
    line = line + delimiter;

    // parsing
    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        parsedWeights.push_back(std::stoi(token));
        line.erase(0, pos + delimiter.length());
    } 

    return parsedWeights;
}

BooleanFormula SatInstance::ReadBooleanFormula(std::ifstream & file) {
   BooleanFormula bf;
   std::string line;
   Clause c;

   while (std::getline(file, line)) {
       c = ParseClause(line);
       bf.push_back(c);
   }

   return bf;
}

Clause SatInstance::ParseClause(std::string &line) {
    std::vector<int> parsedClause;
    std::string delimiter = " ";
    std::string token;
    size_t pos;

    // parsing
    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        parsedClause.push_back(std::stoi(token));
        line.erase(0, pos + delimiter.length());
    } 

    return parsedClause;
}

void SatInstance::PrintFormula() {
    for (auto i : this->formula) {
        for (auto j : i)
            std::cout << j << " ";

        std::cout << std::endl;
    }
}

void SatInstance::PrintWeights() {
    for (auto i : this->weights)
        std::cout << i << " ";

    std::cout << std::endl;
}
