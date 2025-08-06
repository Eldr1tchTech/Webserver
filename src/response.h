#pragma once

typedef struct response_status_line {
    char* version;
    int status_code;
    char* reason;
} response_status_line;

typedef struct header_line
{
    char* name;
    char* value;
} header_line;

typedef struct response {
    header_line* h_lin;
    
} response;

response_status_line* response_status_line_create(char* version, int status_code, char* reason);

void response_status_line_destroy(response_status_line* r_s_l);

char* response_create(response_status_line* status_line, int file_fd, char* file_path);

char* status_line_as_raw(response_status_line* status_line);