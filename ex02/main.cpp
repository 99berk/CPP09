#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    try
    {
        PmergeMe pmerge;
        pmerge.processInput(argc, argv);
        pmerge.sortAndDisplay();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
