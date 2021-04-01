#include <stdlib.h>
#include "race_config_reader.h"

int main() {
    race_config_reader_init("test.txt");
    race_config * cfg = read_race_config();
    race_config_reader_reset();

    show_race_config(cfg);

    free(cfg);
}
