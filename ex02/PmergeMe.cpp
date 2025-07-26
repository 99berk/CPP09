#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <climits>

PmergeMe::PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &other) : _vectorData(other._vectorData), _dequeData(other._dequeData) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
    if (this != &other)
    {
        _vectorData = other._vectorData;
        _dequeData = other._dequeData;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

bool PmergeMe::isValidNumber(const std::string &str)
{
    if (str.empty())
        return false;

    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!std::isdigit(str[i]))
            return false;
    }

    long long num = 0;
    for (size_t i = 0; i < str.length(); ++i)
    {
        num = num * 10 + (str[i] - '0');
        if (num > INT_MAX)
            return false;
    }

    return true;
}

void PmergeMe::parseInput(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (!isValidNumber(argv[i]))
            throw std::runtime_error("Error: Invalid input");

        int num = std::atoi(argv[i]);
        _vectorData.push_back(num);
        _dequeData.push_back(num);
    }
}

void PmergeMe::displaySequence(const std::string &label, const std::vector<int> &sequence)
{
    std::cout << label;

    size_t displayLimit = 4;
    for (size_t i = 0; i < sequence.size() && i < displayLimit; ++i)
        std::cout << " " << sequence[i];

    if (sequence.size() > displayLimit)
        std::cout << " [...]";

    std::cout << std::endl;
}

std::vector<int> PmergeMe::generateJacobsthalSequence(int n)
{
    std::vector<int> jacobsthal;
    if (n <= 0)
        return jacobsthal;

    jacobsthal.push_back(0);
    if (n > 1)
        jacobsthal.push_back(1);

    for (int i = 2; i < n; ++i)
    {
        int next = jacobsthal[i - 1] + 2 * jacobsthal[i - 2];
        if (next > n)
            break;
        jacobsthal.push_back(next);
    }

    return jacobsthal;
}

void PmergeMe::insertionSortVector(std::vector<int> &arr, int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

std::vector<int> PmergeMe::mergeInsertSortVector(std::vector<int> &arr)
{
    int n = arr.size();
    if (n <= 1)
        return arr;

    if (n <= 20)
    {
        insertionSortVector(arr, 0, n - 1);
        return arr;
    }

    std::vector<std::pair<int, int>> pairs;
    int unpaired = -1;

    for (int i = 0; i < n - 1; i += 2)
    {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }

    if (n % 2 == 1)
        unpaired = arr[n - 1];

    for (size_t i = 1; i < pairs.size(); ++i)
    {
        std::pair<int, int> key = pairs[i];
        int j = i - 1;

        while (j >= 0 && pairs[j].first > key.first)
        {
            pairs[j + 1] = pairs[j];
            --j;
        }
        pairs[j + 1] = key;
    }

    std::vector<int> mainChain;
    std::vector<int> pending;

    for (size_t i = 0; i < pairs.size(); ++i)
    {
        mainChain.push_back(pairs[i].first);
        pending.push_back(pairs[i].second);
    }

    if (!pending.empty())
    {
        mainChain.insert(mainChain.begin(), pending[0]);
        pending.erase(pending.begin());
    }

    std::vector<int> jacobsthal = generateJacobsthalSequence(pending.size());
    std::vector<bool> inserted(pending.size(), false);

    for (size_t i = 1; i < jacobsthal.size(); ++i)
    {
        int end = jacobsthal[i];
        int start = jacobsthal[i - 1] + 1;

        for (int j = std::min(end, (int)pending.size()) - 1; j >= start - 1 && j >= 0; --j)
        {
            if (j < (int)pending.size() && !inserted[j])
            {
                std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pending[j]);
                mainChain.insert(pos, pending[j]);
                inserted[j] = true;
            }
        }
    }

    for (size_t i = 0; i < pending.size(); ++i)
    {
        if (!inserted[i])
        {
            std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pending[i]);
            mainChain.insert(pos, pending[i]);
        }
    }

    if (unpaired != -1)
    {
        std::vector<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), unpaired);
        mainChain.insert(pos, unpaired);
    }

    return mainChain;
}

void PmergeMe::insertionSortDeque(std::deque<int> &arr, int left, int right)
{
    for (int i = left + 1; i <= right; ++i)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= left && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

std::deque<int> PmergeMe::mergeInsertSortDeque(std::deque<int> &arr)
{
    int n = arr.size();
    if (n <= 1)
        return arr;

    if (n <= 20)
    {
        insertionSortDeque(arr, 0, n - 1);
        return arr;
    }

    std::vector<std::pair<int, int>> pairs;
    int unpaired = -1;

    for (int i = 0; i < n - 1; i += 2)
    {
        if (arr[i] > arr[i + 1])
            pairs.push_back(std::make_pair(arr[i], arr[i + 1]));
        else
            pairs.push_back(std::make_pair(arr[i + 1], arr[i]));
    }

    if (n % 2 == 1)
        unpaired = arr[n - 1];

    for (size_t i = 1; i < pairs.size(); ++i)
    {
        std::pair<int, int> key = pairs[i];
        int j = i - 1;

        while (j >= 0 && pairs[j].first > key.first)
        {
            pairs[j + 1] = pairs[j];
            --j;
        }
        pairs[j + 1] = key;
    }

    std::deque<int> mainChain;
    std::deque<int> pending;

    for (size_t i = 0; i < pairs.size(); ++i)
    {
        mainChain.push_back(pairs[i].first);
        pending.push_back(pairs[i].second);
    }

    if (!pending.empty())
    {
        mainChain.push_front(pending[0]);
        pending.pop_front();
    }

    std::vector<int> jacobsthal = generateJacobsthalSequence(pending.size());
    std::vector<bool> inserted(pending.size(), false);

    for (size_t i = 1; i < jacobsthal.size(); ++i)
    {
        int end = jacobsthal[i];
        int start = jacobsthal[i - 1] + 1;

        for (int j = std::min(end, (int)pending.size()) - 1; j >= start - 1 && j >= 0; --j)
        {
            if (j < (int)pending.size() && !inserted[j])
            {
                std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pending[j]);
                mainChain.insert(pos, pending[j]);
                inserted[j] = true;
            }
        }
    }

    for (size_t i = 0; i < pending.size(); ++i)
    {
        if (!inserted[i])
        {
            std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), pending[i]);
            mainChain.insert(pos, pending[i]);
        }
    }

    if (unpaired != -1)
    {
        std::deque<int>::iterator pos = std::lower_bound(mainChain.begin(), mainChain.end(), unpaired);
        mainChain.insert(pos, unpaired);
    }

    return mainChain;
}

void PmergeMe::processInput(int argc, char *argv[])
{
    if (argc < 2)
        throw std::runtime_error("Error: No input provided");

    parseInput(argc, argv);

    if (_vectorData.empty())
        throw std::runtime_error("Error: No valid numbers provided");
}

void PmergeMe::sortAndDisplay()
{
    displaySequence("Before:", _vectorData);

    std::vector<int> vectorCopy = _vectorData;
    clock_t start = clock();
    std::vector<int> sortedVector = mergeInsertSortVector(vectorCopy);
    clock_t end = clock();
    double vectorTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;

    std::deque<int> dequeCopy = _dequeData;
    start = clock();
    std::deque<int> sortedDeque = mergeInsertSortDeque(dequeCopy);
    end = clock();
    double dequeTime = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000;

    std::vector<int> dequeResult(sortedDeque.begin(), sortedDeque.end());
    displaySequence("After:", dequeResult);

    std::cout << std::fixed << std::setprecision(5);
    std::cout << "Time to process a range of " << _vectorData.size()
              << " elements with std::vector : " << vectorTime << " us" << std::endl;
    std::cout << "Time to process a range of " << _dequeData.size()
              << " elements with std::deque : " << dequeTime << " us" << std::endl;
}
