#ifndef STRUCTS_MALFUNCTION_MSG_T_H
#define STRUCTS_MALFUNCTION_MSG_T_H

#include "../../util/global.h"

typedef struct malfunction_t malfunction_t;

struct malfunction_t {
    long car_id;
    char description[LARGE_SIZE + MAX_LABEL_SIZE];
};

#endif //STRUCTS_MALFUNCTION_MSG_T_H
