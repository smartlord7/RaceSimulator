/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef RACE_HELPERS_SIGNAL_MANAGER_H
#define RACE_HELPERS_SIGNAL_MANAGER_H

// region public functions prototypes

/**
 * @def signal_handler
 * @brief Function that filters the response according to the signal captured.
 *
 * @param signum
 * Signal captured.
 *
 */
extern void signal_handler(int signum);

// endregion public functions prototypes

// region constants

#define SIGNAL_IGNORE "SIGNAL IGNORED"

// endregion constants

#endif //RACE_HELPERS_SIGNAL_MANAGER_H
