#include "router.h"

#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/sendfile.h>
#include <unistd.h>

router *router_create(char *public_dir, char *path_404)
{
    router *r = malloc(sizeof(router));
    r->public_dir = malloc(strlen(public_dir) + 1);
    r->public_dir = public_dir;
    r->path_404 = malloc(strlen(path_404) + 1);
    r->path_404 = path_404;
    r->routes = darray_create(sizeof(route), 0);
    return r;
}

void router_add_route(router *r, enum method method, char* path, route_handler handler)
{
    route n_route;
    n_route.method = method;
    n_route.path = path;
    n_route.handler = handler;
    darray_add(r->routes, &n_route);
}

void router_handle_route(router *r, int client_fd, enum method method, char *path)
{
    char *file_path;
    response_status_line* res_stat_lin = NULL;
    
    if (strcmp(path, "/") == 0)
    {
        size_t buffer_size = strlen(r->public_dir) + strlen("/index.html") + 1;
        file_path = malloc(buffer_size);
        snprintf(file_path, buffer_size, "%s/index.html", r->public_dir);
    }
    else
    {
        size_t buffer_size = strlen(r->public_dir) + strlen(path) + 1;
        file_path = malloc(buffer_size);
        snprintf(file_path, buffer_size, "%s%s", r->public_dir, path);
    }
    
    int file_fd = open(file_path, O_RDONLY);
    
    if (file_fd < 0)
    {
        
        if (r->routes->size > 0)
        {
            for (int i = 0; i < r->routes->size; i++)
            {
                route* c_route = darray_get(r->routes, i);
                if (c_route->method == method && strcmp(c_route->path, path) == 0)
                {
                    void (*r_handler)(int) = c_route->handler;
                    r_handler(client_fd);
                    free(file_path);
                    return;
                }
            }
        }
        
        free(file_path);
        size_t buffer_size = strlen(r->public_dir) + strlen("/404.html") + 1;
        file_path = malloc(buffer_size);
        snprintf(file_path, buffer_size, "%s/404.html", r->public_dir);
        file_fd = open(file_path, O_RDONLY);
        res_stat_lin = response_status_line_create("HTTP/1.1", 404, "Not Found");
    }
    else
    {
        res_stat_lin = response_status_line_create("HTTP/1.1", 200, "OK");
    }
    
    char *response = response_create(res_stat_lin, file_fd, file_path);
    if (response)
    {
        send(client_fd, response, strlen(response), 0);
        
        if (file_fd >= 0) {
            struct stat file_stat;
            if (fstat(file_fd, &file_stat) == 0) {
                long file_size = file_stat.st_size;
                sendfile(client_fd, file_fd, 0, file_size);
            }
        }
        free(response);
    }
    
    if (file_fd >= 0)
    {
        close(file_fd);
    }
    free(file_path);
    
    if (res_stat_lin) {
        response_status_line_destroy(res_stat_lin);
    }
}

void router_send_file(int client_fd, char* file_path, response_status_line* res_stat_lin) {
    int file_fd = open(file_path, O_RDONLY);

    char *response = response_create(res_stat_lin, file_fd, file_path);
    if (response)
    {
        send(client_fd, response, strlen(response), 0);
        struct stat file_stat;
        fstat(file_fd, &file_stat);
        long file_size = file_stat.st_size;
        sendfile(client_fd, file_fd, 0, file_size);
        free(response);
    }

    if (file_fd >= 0)
    {
        close(file_fd);
    }
}

void router_destroy(router *r)
{
    darray_destroy(r->routes);
    r->routes = 0;
    free(r->public_dir);
    r->public_dir = 0;
    free(r->path_404);
    r->path_404 = 0;
    free(r);
    r = 0;
}