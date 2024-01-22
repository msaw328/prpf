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
#include "net.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>

int create_listening_socket(unsigned short port) {
    printf("creating socket on port %u\n", port);
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    printf("socket res: %d\n", fd);

    int do_reuse_addr = 1;
    int res = setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &do_reuse_addr, sizeof(int));
    printf("setsockopt res: %d\n", res);

    struct sockaddr_in listen_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr = {
            .s_addr = INADDR_ANY
        }
    };
    res = bind(fd, (const struct sockaddr *) &listen_addr, sizeof(struct sockaddr_in));
    printf("bind res: %d\n", fd);

    res = listen(fd, 16);
    printf("listen res: %d\n", res);

    return fd;
}
