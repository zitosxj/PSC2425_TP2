#ifndef HTTP_POST_JSON_H
#define HTTP_POST_JSON_H

#include <jansson.h>

json_t *http_post_json(const char *uri, json_t *data);

#endif // HTTP_POST_JSON_H
