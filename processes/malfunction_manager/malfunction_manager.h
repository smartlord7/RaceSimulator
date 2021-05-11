/** Project RaceSimulator - LEI, University of Coimbra, 2nd year, 2nd semester - Operating Systems
*
* @author
*  - Joao Filipe Guiomar Artur, 2019217853
*  - Sancho Amaral Simoes, 2019217590
*
* @date 01/04/2021
*/

#ifndef RACESIMULATOR_C_MALFUNCTION_MANAGER_H
#define RACESIMULATOR_C_MALFUNCTION_MANAGER_H

// region constants

#define MALFUNCTION_MANAGER "MALFUNCTION_MANAGER"
#define NUM_MALFUNCTIONS 9

// region malfunction messages

#define MSG_PREFIX "[" MALFUNCTION_MANAGER " -> CAR %d] "
#define ENGINE_SPUTTERING_MSG MSG_PREFIX "THE ENGINE IS SPUTTERING! IT MIGHT BE RECEIVING AN ABNORMAL AIR QUANTITY!"
#define STEERING_WHEEL_SHAKING_MSG MSG_PREFIX "LOOKS LIKE STEERING WHEEL IS SHAKING DUE TO SOME DAMAGE CAUSED IN THE WHEEL BEARINGS!"
#define TIRES_FLAT_MSG MSG_PREFIX "IT'S NOT POSSIBLE TO FULLY ENSURE THE CAR'S CONTROL SINCE THE TIRES ARE FLAT AND MUST BE CHANGED!"
#define ALTERNATOR_FAILING_MSG MSG_PREFIX "A BATTERY WARNING APPEARED IN THE CAR CONTROL PANEL! THE ALTERNATOR MIGHT BE FAILING!"
#define RADIATOR_LEAKING_MSG MSG_PREFIX "THE CAR'S TEMPERATURE WENT SUDDENLY UP! THE RADIATOR MUST BE LEAKING!"
#define TRANSMISSION_FLUID_LEAKING_MSG MSG_PREFIX "THE TRANSMISSION SEEMS TO BE A BIT AFFECTED! THE CONTROL PANEL POINTS TO A POSSIBLE TRANSMISSION FLUID LEAKING!"
#define LIGHTS_FAILING_MSG MSG_PREFIX "THE MAIN AND SECONDARY LIGHTS WENT OFFLINE!"
#define EXCESSIVE_OIL_CONSUMPTION_MSG MSG_PREFIX "THE OIL CONSUMPTION IS EXCESSIVELY HIGH!"
#define BRAKES_FAILING MSG_PREFIX "IT SEEMS THE BRAKES AREN'T WORKING WELL!"

// endregion malfunction messages

// endregion constants

// region public functions prototypes

/**
 * @def malfunction_manager
 * @brief Worker function executed when the malfunction manager process is created.
 *
 */
_Noreturn void malfunction_manager();

// endregion public functions prototypes

#endif //RACESIMULATOR_C_MALFUNCTION_MANAGER_H
