#include "server.h"
#include "router.h"

void ROUTE_test_htmx (int client_fd) {
    response_status_line* res_stat_lin;
    res_stat_lin = response_status_line_create("HTTP/1.1", 200, "OK");
    char* file_path = "dynamic/htmx_test.html";
    router_send_file(client_fd, file_path, res_stat_lin);
}

int main () {
    server* s = server_create(8080, "public", "404.html");

    router_add_route(s->r, METHOD_GET, "/test-htmx", ROUTE_test_htmx);

    server_start(s);
    server_destroy(s);

    return 1;
}