/**
 * @file
 * @author Martin Kersner <m.kersner@gmail.com>
 * @date 12/27/2014
 */

#ifndef SAT_HH
#define SAT_HH

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <assert.h>
#include <stdlib.h>

typedef std::vector<int> Clause;
typedef std::vector<Clause> BooleanFormula;

const int FILE_NOT_EXIST = 1;

class SatInstance {
    public:
        SatInstance(const char * file_name);
        ~SatInstance();
        void PrintFormula();
        void PrintWeights();
        int GetLength();

        std::vector<bool> GetSuboptimalSolution();
        int GetSuboptimalWeightSum();
        int SetMaxWeightSum(int maxWeightSum);
        int GetMaxWeightSum();

        std::vector<int> weights;
        BooleanFormula formula;

    private:
        inline bool ExistFile(const char * file_name);
        void ReadHead(std::ifstream & file);
        std::vector<int> ReadWeights(std::ifstream & file);
        BooleanFormula ReadBooleanFormula(std::ifstream & file);
        std::vector<int> ParseWeights(std::string & line);
        Clause ParseClause(std::string &line);
        std::vector<bool> ReadSuboptimalSolution(std::string & line);
        std::vector<bool> ParseBinaryVector(std::string & line);
        bool CharToBool(char c);
        void SetSuboptimalSolution(std::vector<bool> & suboptimalSolution);
        void SumSuboptimalWeights();
        void SumMaxWeights();

        int length; // length of solutions; number of variables

        std::vector<bool> suboptimalSolution; // given solution read from file
        int suboptimalWeightSum;

        int maxWeightSum;
};
#endif //SAT_HH
