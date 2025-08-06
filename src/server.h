#pragma once

#include "router.h"
#include "http_parser.h"

typedef struct {
    int socket_fd;
    int port;
    router* r;
} server;

server* server_create(int port, char* public_dir, char* path_404);

void server_start(server* server);

void server_destroy(server* server);