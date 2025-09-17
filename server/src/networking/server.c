#include "server.h"
#include "router.h"
#include "http_parser.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

server* server_create(int port, char* public_dir, char* path_404) {
    server* s = calloc(sizeof(server));

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr));

    s->port = port;
    s->socket_fd = socket_fd;

    s->r = router_create(public_dir, path_404);

    return s;
}

void server_start(server* server) {
    listen(server->socket_fd, 10);

    while (1)
    {
        int client_fd = accept(server->socket_fd, 0, 0);
        if (client_fd < 0)
        {
            continue;
        }

        char buffer[1024] = {0};
        int bytes_received = recv(client_fd, buffer, 1024, 0);

        if (bytes_received <= 0)
        {
            close(client_fd);
            continue;
        }

        buffer[bytes_received] = '\0';

        request* req = parse_request(buffer);
        if (req)
        {
            router_handle_route(server->r, client_fd, req->method, req->path);
        }
        
        free(req);

        close(client_fd);
    }
}

void server_destroy(server* s) {
    close(s->socket_fd);
    router_destroy(s->r);
    s->port = 0;
    s->socket_fd = 0;
    free(s);
    s = 0;
}