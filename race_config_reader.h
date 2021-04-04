/*
 * Authors:
 *  - Joao Filipe Guiomar Artur, 2019217853
 *  - Sancho Amaral Simoes, 2019217590
 *
 * Date of creation: 02/04/2021
 */

#ifndef RACESIMULATOR_RACE_CONFIG_READER_H
#define RACESIMULATOR_RACE_CONFIG_READER_H

#include "structs/race_config_t.h"

/**
 * Initialize configuration file reading functionality.
 * @param cfg_file_path Path of configuration file.
 */
void race_config_reader_init(char * cfg_file_path);

/**
 * Read the configuration file.
 * @return Pointer to config memory space.
 */
race_config_t * read_race_config();

#endif //RACESIMULATOR_RACE_CONFIG_READER_H