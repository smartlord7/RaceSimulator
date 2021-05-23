/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @authors
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 22/05/2021
*/

#ifndef STRUCTS_MALFUNCTION_MSG_T_H
#define STRUCTS_MALFUNCTION_MSG_T_H

// region dependencies

#include "../../util/global.h"

// endregion dependencies

// region structures

/**
 * @struct malfunction_t
 * @brief struct that holds the information of a malfunction assigned random car.
 *
 * @var malfunction_t::car_id
 * ID of the car affected by the malfunction.
 *
 * @var malfunction_t::description
 * Description of the malfunction.
 *
 */
typedef struct malfunction_t {
    long car_id;
    char description[LARGE_SIZE + MAX_LABEL_SIZE];
} malfunction_t;

//endregion structures

#endif //STRUCTS_MALFUNCTION_MSG_T_H
