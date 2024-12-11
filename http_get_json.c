/* *************************************************************************************
 * PSC 2425i - Grupo 3 (13909 Dinis Lopes, 46974 Luís Alves)  
 * *************************************************************************************/

#include <curl/curl.h>
#include <jansson.h>
#include <string.h>

#define BUFFER_CHUNK (4 * 1024)

struct write_buffer {
	char *buffer;
	int current, max;
};

static size_t write_callback(void *ptr, size_t size, size_t nmemb, void *stream) {
	struct write_buffer *result = (struct write_buffer *)stream;

	while (result->current + size * nmemb >= result->max - 1) {
		result->buffer = realloc(result->buffer, result->max + BUFFER_CHUNK);
		if (NULL == result->buffer) {
			fprintf(stderr, "Out of memory\n");
			return 0;
		}
		result->max += BUFFER_CHUNK;
	}

	memcpy(result->buffer + result->current, ptr, size * nmemb);
	result->current += size * nmemb;

	return size * nmemb;
}

json_t *http_get_json(const char *url) {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *hcurl = curl_easy_init();
    if (hcurl == NULL) {
        fprintf(stderr, "Error in curl_easy_init function\n");
        goto error;
    }
	char error_buf[CURL_ERROR_SIZE];
    curl_easy_setopt(hcurl, CURLOPT_URL, url);
	curl_easy_setopt(hcurl, CURLOPT_ERRORBUFFER, error_buf);
    curl_easy_setopt(hcurl, CURLOPT_WRITEFUNCTION, write_callback);

    char *buffer = malloc(BUFFER_CHUNK);
    if (NULL == buffer)
        goto error;

    struct write_buffer write_result = {
        .buffer = buffer,
        .current = 0,
        .max = BUFFER_CHUNK
    };

    curl_easy_setopt(hcurl, CURLOPT_WRITEDATA, &write_result);
	curl_easy_setopt(hcurl, CURLOPT_VERBOSE, 0L);

	// Send request and returns only after the answer is received
	int curl_res = curl_easy_perform(hcurl);
	// Cleanup after using libcurl
	curl_easy_cleanup(hcurl);

	if (CURLE_OK != curl_res) {
		printf("Curl error [#%X]: \"%s\"\n", curl_res, error_buf);
        goto error;
	}

    write_result.buffer[write_result.current] = '\0';

    json_error_t error;
    json_t *result = json_loadb(write_result.buffer, write_result.current, 0, &error);
    if( NULL == result)
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
    free(write_result.buffer);
    curl_global_cleanup();
    return result;

error:
    curl_global_cleanup();
    return NULL;
}
