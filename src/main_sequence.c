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
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "portseq.h"

#define NUMBER_OF_PORTS 10
static uint64_t nonce = 0x65;


int main_sequence(int argc, char** argv) {
    time_t now = time(NULL);

    uint64_t minutes_per_interval = 1;
    uint64_t timestamp_interval = minutes_per_interval * 60;

    printf("Sequence of ports for timestamp %lu and nonce 0x%lx:\n", now, nonce);
    for(int i = 0; i < NUMBER_OF_PORTS; i++) {
        uint16_t hash = portseq_get_timestamp_hash(now, minutes_per_interval, nonce);
        printf("+%3ld min: %u\n", i * minutes_per_interval, portseq_get_port_from_hash(hash));
        now += timestamp_interval;
    }

    return 0;
}

// Define main_sequence() as alternative main() for when building split binaries
int __attribute__((weak, alias("main_sequence"))) main(int argc, char** argv);
