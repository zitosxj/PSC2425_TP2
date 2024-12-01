#ifndef HTTP_GET_JSON_H
#define HTTP_GET_JSON_H

#include <jansson.h>

json_t *http_get_json(const char *url);

#endif // HTTP_GET_JSON_H
