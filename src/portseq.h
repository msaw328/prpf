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
#ifndef _PRPF_PORTSEQ_H_
#define _PRPF_PORTSEQ_H_

#include <stdint.h>

uint16_t portseq_get_timestamp_hash(uint64_t tick64, uint64_t minutes_per_interval, uint64_t nonce);

unsigned short portseq_get_port_from_hash(uint16_t hash);

#endif
