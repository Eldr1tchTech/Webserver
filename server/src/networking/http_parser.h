#pragma once

enum method
{
    METHOD_GET,
    METHOD_POST,
    METHOD_PUT,
    METHOD_DELETE,
    METHOD_UNKNOWN
};

typedef struct request {
    enum method method;
    char* path;
    char* version;
    char* host;
} request;

request* parse_request(char* raw_request);

enum method parse_method(char* raw_method);