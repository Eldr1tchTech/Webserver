#include "http_parser.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

request* parse_request(char* raw_request) {
    request* req = malloc(sizeof(request));
    if (!req) {
        return NULL;
    }

    char* l1 = strtok(raw_request, "\n");
    char* l2 = strtok(NULL, "\n");

    req->method = parse_method(strtok(l1, " "));
    req->path = strtok(NULL, " ");
    req->version = strtok(NULL, " ");

    strtok(l2, " ");
    req->host = strtok(NULL, "\n");

    return req;
}

enum method parse_method(char* raw_method) {
    if (strcmp(raw_method, "GET") == 0)
    {
        return METHOD_GET;
    } else {
        printf("parse_method - Unable to parse %s.\n", raw_method);
        return METHOD_UNKNOWN;
    }
}