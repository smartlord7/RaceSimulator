/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef UTIL_NUMBERS_H
#define UTIL_NUMBERS_H

//region constants

#define true 1
#define false 0
#define NUM_EVENT_VALUES 100

//endregion constants

//region public functions prototypes

/**
 * @def random_int
 * @brief Function that generates a random int from an interval.
 *
 * @param min
 * Lowest value of the interval.
 *
 * @param max
 * Highest value of the interval.
 *
 * @return Int contained in the interval.
 */
extern int random_int(int min, int max);

/**
 * @def random_uniform_event
 * @brief Function that generates a random event based on the given probability.
 *
 * @param probability
 * Probability of the event.
 *
 * @return
 */
extern int random_uniform_event(float probability);

/**
 * @def ms_sleep
 * @brief Function that puts a process or thread on a sleep state.
 *
 * @param ms
 * Duration of the sleep in milliseconds.
 *
 */
extern void ms_sleep(uint ms);

//endregion public functions prototypes

#endif //UTIL_NUMBERS_H
