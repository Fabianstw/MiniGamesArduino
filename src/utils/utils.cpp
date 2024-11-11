#include <cstdlib>
#include <ctime>

/** Helper function for uniform random numbers in cpp */
int randInt(int min, int max)
{
    return min + (rand() % (max - min + 1));
}