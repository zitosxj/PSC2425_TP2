#ifndef HTTP_JSON_H
#define HTTP_JSON_H

json_t *http_get_json(const char *url);
json_t *http_post_json(const char *uri, json_t *data);

#endif // HTTP_JSON_H
