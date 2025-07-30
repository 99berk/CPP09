#ifndef PMERGE_ME_HPP
#define PMERGE_ME_HPP

#include <vector>
#include <deque>
#include <string>

class PmergeMe
{
    private:
        std::vector<int> _vectorData;
        std::deque<int> _dequeData;

        std::vector<int> mergeInsertSortVector(std::vector<int> &arr);
        std::deque<int> mergeInsertSortDeque(std::deque<int> &arr);

        std::vector<int> generateJacobsthalSequence(int n);

        void parseInput(int argc, char *argv[]);
        bool isValidNumber(const std::string &str);
        void displaySequence(const std::string &label, const std::vector<int> &sequence);
        double getCurrentTime();
        
    public:
        PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void processInput(int argc, char *argv[]);
        void sortAndDisplay();
};

#endif