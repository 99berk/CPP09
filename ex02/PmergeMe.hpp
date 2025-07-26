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
        void insertionSortVector(std::vector<int> &arr, int left, int right);
        std::vector<int> generateJacobsthalSequence(int n);

        std::deque<int> mergeInsertSortDeque(std::deque<int> &arr);
        void insertionSortDeque(std::deque<int> &arr, int left, int right);

        bool isValidNumber(const std::string &str);
        void parseInput(int argc, char *argv[]);
        void displaySequence(const std::string &label, const std::vector<int> &sequence);

    public:
        PmergeMe();
        PmergeMe(const PmergeMe &other);
        PmergeMe &operator=(const PmergeMe &other);
        ~PmergeMe();

        void processInput(int argc, char *argv[]);
        void sortAndDisplay();
};

#endif