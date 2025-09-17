#include "response.h"
#include "utils.h"

#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

response_status_line *response_status_line_create(char *version, int status_code, char *reason)
{
    // First, allocate memory for the struct itself
    response_status_line *r_s_l = malloc(sizeof(response_status_line));
    if (!r_s_l)
    {
        return NULL; // Handle allocation failure
    }

    // Allocate memory for version string (+1 for null terminator)
    r_s_l->version = malloc(strlen(version) + 1);
    if (!r_s_l->version)
    {
        free(r_s_l); // Clean up if allocation fails
        return NULL;
    }
    strcpy(r_s_l->version, version); // Copy the string content

    // Set the status code
    r_s_l->status_code = status_code;

    // Allocate memory for reason string (+1 for null terminator)
    r_s_l->reason = malloc(strlen(reason) + 1);
    if (!r_s_l->reason)
    {
        free(r_s_l->version); // Clean up previously allocated memory
        free(r_s_l);
        return NULL;
    }
    strcpy(r_s_l->reason, reason); // Copy the string content

    return r_s_l;
}

void response_status_line_destroy(response_status_line *r_s_l)
{
    free(r_s_l);
    r_s_l = 0;
}

char *response_create(response_status_line *status_line, int file_fd, char *file_path)
{
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    long file_size = file_stat.st_size;

    header_line content_type = {0};
    content_type.name = "Content-Type";
    char *dot = strrchr(file_path, '.');
    if (!dot)
    {
        return NULL;
    }
    char *file_extension = dot + 1;
    if (strcmp(file_extension, "html") == 0)
    {
        content_type.value = "text/html";
    }
    else
    {
        printf("response_create - File extension '%s' currently not supported. Setting to 'text/html'.\n", file_extension);
        content_type.value = "text/html";
    }

    header_line content_length = {0};
    content_length.name = "Content-Length";
    size_t buffer_size = numdig(file_size) + 1;
    content_length.value = malloc(buffer_size);
    if (!content_length.value)
    {
        return NULL;
    }
    snprintf(content_length.value, buffer_size, "%ld", file_size);

    char *raw_status_line = status_line_as_raw(status_line);
    if (!raw_status_line)
    {
        free(content_length.value);
        return NULL;
    }

    size_t response_size = strlen(raw_status_line) + strlen(content_type.name) +
                           strlen(content_type.value) + strlen(content_length.name) +
                           strlen(content_length.value) + 13 + 1;

    char *raw_response = malloc(response_size);
    if (!raw_response)
    {
        free(content_length.value);
        free(raw_status_line);
        return NULL;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-truncation"
    snprintf(raw_response, response_size, "%s\r\n%s: %s\r\n%s: %s\r\n\r\n",
             raw_status_line, content_type.name, content_type.value,
             content_length.name, content_length.value);
#pragma GCC diagnostic pop

    free(raw_status_line);
    free(content_length.value);
    return raw_response;
}

char *status_line_as_raw(response_status_line *status_line)
{
    char *raw_status_line;

    int stat_lin_len = strlen(status_line->version) + 1 + numdig((long)status_line->status_code) + 1 + strlen(status_line->reason) + 1;
    raw_status_line = malloc(stat_lin_len);
    if (raw_status_line)
    {
        snprintf(raw_status_line, stat_lin_len, "%s %d %s", status_line->version, status_line->status_code, status_line->reason);
    }

    return raw_status_line;
}