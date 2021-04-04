#ifndef RACESIMULATOR_RACE_CONFIG_READER_H
#define RACESIMULATOR_RACE_CONFIG_READER_H

#include "structs/race_config_t.h"

void race_config_reader_init(char * cfg_file_path);
race_config_t * read_race_config();

#endif //RACESIMULATOR_RACE_CONFIG_READER_H