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

/**
 * Constructor reads CNF in DIMACS format.
 */
SatInstance::SatInstance(const char * fileName) {
    std::vector<std::string> * params = NULL;
    SatInstance * inst = NULL;

    if (ExistFile(fileName)) {
        std::ifstream file(fileName);
        std::string line; 

        ReadHead(file);
        this->weights = ReadWeights(file);
        this->formula = ReadBooleanFormula(file);
        this->length = weights.size();

        // sums weights of suboptimal solution given by input file
        SumSuboptimalWeights();

        SumMaxWeights();
    }
    else {
        std::cerr << "Given file does not exist!" << std::endl;
        exit(FILE_NOT_EXIST);
    }
}

/** Destructor */
SatInstance::~SatInstance() {}

/** 
 * Check if given file exists.
 */
inline bool SatInstance::ExistFile(const char * fileName) {
    std::ifstream ifs(fileName);

    if (ifs.good()) {
        ifs.close();
        return true;
    } 
    else {
        ifs.close();
        return false;
    }   
}

/** 
 * Skips first four rows containing head.
 * The second row is consisted from suboptimal solution,
 * which is recorded for later purpose.
 */
void SatInstance::ReadHead(std::ifstream & file) {
    std::string line;
    std::vector<bool> suboptimalSolution;

    for (int i = 0; i < 4; ++i) {
       std::getline(file, line);

       if (i == 1) {
        suboptimalSolution = ReadSuboptimalSolution(line);
        SetSuboptimalSolution(suboptimalSolution);
        }
    }
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

int SatInstance::GetLength() {
    return this->length;
}

/**
 * Read line containing suboptimal solution given in format:
 * c solution = 110011110010100
 */
std::vector<bool> SatInstance::ReadSuboptimalSolution(std::string & line) {
    std::string delimiter = " ";
    std::string token;

    // delete the name of line
    size_t pos = line.find(delimiter);
    assert(line.substr(0, pos) == "c");
    line.erase(0, pos + delimiter.length());

    // parsing
    while ((pos = line.find(delimiter)) != std::string::npos) {
        token = line.substr(0, pos);
        line.erase(0, pos + delimiter.length());
    } 

    return ParseBinaryVector(line);
}

/** 
 * Parse a string containing binary vector.
 */
std::vector<bool> SatInstance::ParseBinaryVector(std::string & line) {
    std::vector<bool> binaryVector;

    for(char c : line)
        binaryVector.push_back(CharToBool(c));

    return binaryVector;
}

/**
 * Char to bool conversion.
 */
bool SatInstance::CharToBool(char c) {
    if (c == '0')
        return 0;
    else
        return 1;
}

/**
 * Setter of private variable suboptimalSolution.
 */
void SatInstance::SetSuboptimalSolution(std::vector<bool> & suboptimalSolution) {
    this->suboptimalSolution = suboptimalSolution;
}

/**
 * Sums the weights of suboptimal solution.
 */
void SatInstance::SumSuboptimalWeights() {
    int weightSum = 0;

    auto w_it = this->weights.begin();
    for (auto b : this->suboptimalSolution) {
        if (b == 1)
            weightSum += *w_it;

        w_it++;
    }

    this->suboptimalWeightSum = weightSum;
}

/**
 * Getter of private variable suboptimalSolution.
 */
std::vector<bool> SatInstance::GetSuboptimalSolution() {
    return this->suboptimalSolution;
}

/**
 * Getter of private variable suboptimalWeightSum.
 */
int SatInstance::GetSuboptimalWeightSum() {
    return this->suboptimalWeightSum;
}

/**
 * Sums all weights.
 */
void SatInstance::SumMaxWeights() {
    int maxWeightSum = 0;

    for (auto w : this->weights)
        maxWeightSum += w;

    SetMaxWeightSum(maxWeightSum);
}

/**
 * Getter of private variable maxWeightSum.
 */
int SatInstance::GetMaxWeightSum() {
    return this->maxWeightSum;
}

/**
 * Setter of private variable maxWeightSum.
 */
int SatInstance::SetMaxWeightSum(int maxWeightSum) {
    this->maxWeightSum = maxWeightSum;
}
