#ifndef RACESIMULATOR_RACE_CONFIG_READER_H
#define RACESIMULATOR_RACE_CONFIG_READER_H

#include "structs/race_config_t.h"

void race_config_reader_init(char * cfg_file_path);
void race_config_reader_reset();
race_config_t * read_race_config();
void show_race_config(race_config_t * cfg);

#endif //RACESIMULATOR_RACE_CONFIG_READER_H
