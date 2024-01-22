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
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/socket.h>

#include "timers.h"
#include "net.h"
#include "portseq.h"

int listen_fd = -1;

unsigned int minutes_per_interval = 1; // minutes
uint64_t nonce = 0x65;

void timer_handler(int a) {
    puts("close");
    unsigned short port = portseq_get_port_from_hash(portseq_get_timestamp_hash((uint64_t) time(NULL), (uint64_t) minutes_per_interval, nonce));
    close(listen_fd);
    listen_fd = create_listening_socket(port);
    printf("listen_fd = %d\n", listen_fd);
}

int main_listen(int argc, char** argv) {
    timer_t timer_id = { 0 };

    if(!timers_is_valid_interval_mins(minutes_per_interval)) {
        puts("Invalid minutes_per_interval");
        return 0;
    }

    int res = timers_make_timer(&timer_id);
    printf("make timer res: %d\n", res);

    uint16_t hash = portseq_get_timestamp_hash((uint64_t) time(NULL), (uint64_t) minutes_per_interval, nonce);
    unsigned short port = portseq_get_port_from_hash(hash);
    listen_fd = create_listening_socket(port);

    puts("Expected sequence of ports:");
    for(int i = 0; i < 20; i++) {
        hash = portseq_get_timestamp_hash((uint64_t) time(NULL) + (i * minutes_per_interval * 60), (uint64_t) minutes_per_interval, nonce);
        port = portseq_get_port_from_hash(hash);
        printf("%u\n", port);
    }

    res = timers_install_handler(timer_handler);
    printf("install handler res: %d\n", res);

    res = timers_start_timer_at_interval_mins(timer_id, minutes_per_interval);
    printf("start timer res: %d\n", res);

    while(1) {
        pause();
    }

    return 0;
}

// Define main_listen() as alternative main() for when building split binaries
int __attribute__ ((weak, alias("main_listen"))) main(int argc, char** argv);
