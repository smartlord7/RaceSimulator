#ifndef RACESIMULATOR_READ_LINE_H
#define RACESIMULATOR_READ_LINE_H

#include <stdio.h>
#define BUFFER_SIZE_EXCEEDED -2
#define LINE_READ_SUCCESSFULLY 1

int read_line(char * buffer, FILE * file, int buffer_size);

#endif //RACESIMULATOR_READ_LINE_H
