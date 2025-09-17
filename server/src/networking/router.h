#pragma once

#include "http_parser.h"
#include "response.h"
#include "containers/darray.h"

#define ROUTER_MAX_ROUTES 256

typedef void (*route_handler)(int client_fd);

typedef enum segment_type {
    SEGMENT_STATIC,
    SEGMENT_PARAM
} segment_type;

typedef struct route_segment
{
    segment_type type;
    char* value;
} route_segment;

typedef struct route {
    route_segment *segments;
    int segment_count;
    void (*handler)(int client_fd, ...);
} route;

typedef struct router {
    int num_routes;
    route routes[ROUTER_MAX_ROUTES];
    char* public_dir;
    char* path_404;
} router;

router* router_create(char* public_dir, char*);

void router_add_route(router* r, enum method method, char* path, route_handler handler);

void router_handle_route(router* r, int client_fd, enum method method, char* path);

void router_send_file(int client_fd, char* file_path, response_status_line* res_stat_lin);

void router_destroy(router* r);