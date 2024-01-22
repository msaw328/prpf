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
#include "portseq.h"

#include <stdint.h>
#include <stddef.h>

// Port sequence generation - based on current time

// A hash function based on DJB2, except changed to return 16-bit values and incrementally consume bytes from input
#define DJB2_16_INITIAL_VAL 5381
static uint16_t __djb2_16(uint16_t hash, uint8_t* bytes, size_t size) {
    for(size_t i = 0; i < size; i++)
        hash = ((hash << 5) + hash) + bytes[i]; /* hash * 33 + c */

    return hash;
}

// Get the 16-bit hash based on current time and a nonce
uint16_t portseq_get_timestamp_hash(uint64_t tick64, uint64_t minutes_per_interval, uint64_t nonce) {
    uint64_t interval = minutes_per_interval * 60;
    uint64_t time_seed = tick64 - (tick64 % interval);

    uint16_t hash = DJB2_16_INITIAL_VAL;
    hash = __djb2_16(hash, (uint8_t*) &time_seed, sizeof(uint64_t)); // Consume time_seed first
    hash = __djb2_16(hash, (uint8_t*) &nonce, sizeof(uint64_t)); // Consume nonce afterwards

    return hash;
}

// TODO: Implement proper actual port range support
#define PORT_RANGE_START 10000
#define PORT_RANGE_LEN 55000

unsigned short portseq_get_port_from_hash(uint16_t hash) {
    return PORT_RANGE_START + (hash % PORT_RANGE_LEN);
}
