#include "errno.h"
#include "../../util/global.h"
#include "../log_generator/log_generator.h"

static void ms_sleep(uint ms);

void pause_and_restart_clock() {
    shm->thread_clock.clock_paused = true;
    shm->thread_clock.global_time = 0;

    SYNC_CLOCK_RISE
    notify_cond_all(&shm->thread_clock.clock_rise_cond);
    END_SYNC_CLOCK_RISE
}

void unpause_clock() {
    shm->thread_clock.clock_paused = false;

    SYNC_CLOCK_VALLEY
    notify_cond(&shm->thread_clock.clock_valley_cond);
    END_SYNC_CLOCK_VALLEY
}

void end_clock() {
    shm->thread_clock.clock_on = false;
    shm->thread_clock.clock_paused = false;

    SYNC_CLOCK_VALLEY
    notify_cond(&shm->thread_clock.clock_valley_cond); // notify the clock that the race is over.
    END_SYNC_CLOCK_VALLEY

            SYNC_CLOCK_RISE
    notify_cond_all(&shm->thread_clock.clock_rise_cond); // notify all the threads waiting for the next clock that the race is over.
    END_SYNC_CLOCK_RISE
}

void init_global_clock() {

    shm->thread_clock.clock_on = true;
    shm->thread_clock.clock_paused = false;

    DEBUG_MSG(GLOBAL_CLOCK_START, TIME, "")

    int interval_ms = (int) ((double) 1 / config.time_units_per_sec * pow(10, 3));

    while (true) {
        SYNC_CLOCK_VALLEY // wait for all the car threads and malfunction manager to arrive and wait for the next clock
        while ((shm->thread_clock.num_clock_waiters < shm->num_cars_on_track + 1 || shm->thread_clock.clock_paused) && shm->thread_clock.clock_on) {
            DEBUG_MSG(GLOBAL_CLOCK_RECEIVED, TIME, shm->thread_clock.num_clock_waiters,
                      (shm->num_cars_on_track + 1) - shm->thread_clock.num_clock_waiters);
            wait_cond(&shm->thread_clock.clock_valley_cond, &shm->thread_clock.clock_valley_mutex);
        }
        END_SYNC_CLOCK_VALLEY

        //check if the clock is supposed to be active
        if (!shm->thread_clock.clock_on) {
            return;
        }

        DEBUG_MSG(GLOBAL_CLOCK_READY, TIME, "")
        DEBUG_MSG(GLOBAL_CLOCK_VALLEY, TIME, "");

        ms_sleep((uint) interval_ms);

        generate_log_entry(CLOCK, NULL, NULL);

        DEBUG_MSG(GLOBAL_CLOCK_RISE, TIME, "")

        // start next clock period and notify all of the threads waiting for the next clock rise
        SYNC_CLOCK_RISE
        shm->thread_clock.global_time++;
        DEBUG_MSG(GLOBAL_CLOCK_TIME, TIME, shm->thread_clock.global_time)
        notify_cond_all(&shm->thread_clock.clock_rise_cond);
        END_SYNC_CLOCK_RISE

        DEBUG_MSG(GLOBAL_CLOCK_RELEASE, TIME, "");
    }
}

void sync_sleep(int time_units) {
    int time_counted = 0, prev_time = shm->thread_clock.global_time;

    while (time_counted < time_units) {

        //notify the clock
        SYNC_CLOCK_VALLEY
        shm->thread_clock.num_clock_waiters++;
        notify_cond(&shm->thread_clock.clock_valley_cond);
        END_SYNC_CLOCK_VALLEY

        DEBUG_MSG(GLOBAL_CLOCK_WAITERS, TIME, shm->thread_clock.num_clock_waiters + 1);

        //wait for the next clock rise
        SYNC_CLOCK_RISE
        while (prev_time == shm->thread_clock.global_time && shm->thread_clock.clock_on && !shm->thread_clock.clock_paused) {
            wait_cond(&shm->thread_clock.clock_rise_cond, &shm->thread_clock.clock_rise_mutex);
        }
        END_SYNC_CLOCK_RISE

        //check if the clock is deactivated or paused.
        if (!shm->thread_clock.clock_on || shm->thread_clock.clock_paused) {
            return;
        }

        //notify the clock
        SYNC_CLOCK_VALLEY
        shm->thread_clock.num_clock_waiters--;
        notify_cond(&shm->thread_clock.clock_valley_cond);
        END_SYNC_CLOCK_VALLEY

        prev_time = shm->thread_clock.global_time;

        time_counted++;
    }
}

/**
 * @def ms_sleep
 * @brief Function that puts a process or thread on a sleep state.
 *
 * @param ms
 * Duration of the sleep in milliseconds.
 *
 */
static void ms_sleep(uint ms) {
    int result;
    struct timespec ts_remaining = {ms / 1000, (ms % 1000) * 1000000L};

    do {
        struct timespec ts_sleep = ts_remaining;
        result = nanosleep(&ts_sleep, &ts_remaining);
    } while ((EINTR == errno) && (-1 == result));
}

