#include <stdlib.h>
#include <time.h>

#include "Arduino.h"

/**
 * Initializes the random seed for the random number generator.
 * Using an unconnected analog pin.
 */
void initRandomSeed() { randomSeed(analogRead(0)); }

/**
 * @brief Generates a random integer within a specified range.
 *
 * This function generates a random integer between the specified minimum
 * and maximum values, inclusive. It uses the standard library's rand()
 * function to generate the random number.
 *
 * @param min The minimum value of the range.
 * @param max The maximum value of the range.
 * @return A random integer between min and max, inclusive.
 */
int randInt(int min, int max) { return min + (random() % (max - min + 1)); }