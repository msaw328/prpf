// prpf - psuedo random port forwarding
// Copyright (C) 2023 Maciej Sawka <maciejsawka@gmail.com> <msaw328@kretes.xyz>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#include "timers.h"

#include <stddef.h>
#include <signal.h>
#include <time.h>

// Setup of POSIX timers and signal handling

#define TIMERS_SIGNAL_NUM SIGUSR2

int timers_install_handler(void (*timer_handler) (int)) {
    struct sigaction timer_handler_sig = {
        .sa_handler = timer_handler
    };

    return sigaction(TIMERS_SIGNAL_NUM, &timer_handler_sig, NULL);
}

int timers_is_valid_interval_mins(unsigned int minutes) {
    return 60 % minutes == 0; // Interval should divide an hour
}

int timers_make_timer(timer_t* ptimer) {
    struct sigevent sevp = {
        .sigev_notify = SIGEV_SIGNAL,
        .sigev_signo = TIMERS_SIGNAL_NUM
    };

    return timer_create(CLOCK_REALTIME, &sevp, ptimer);
}

int timers_start_timer_at_interval_mins(timer_t ptimer, unsigned int minutes) {
    time_t interval = minutes * 60; // 60 secs in a min

    time_t first_delay = interval - (time(NULL) % interval); // first delay may be different, cause we wanna sync to full hour 0:00

    struct itimerspec timer_spec = {
        .it_interval = {
            .tv_sec = interval
        },
        .it_value = {
            .tv_sec = first_delay
        }
    };

    return timer_settime(ptimer, 0, &timer_spec, NULL);
}
