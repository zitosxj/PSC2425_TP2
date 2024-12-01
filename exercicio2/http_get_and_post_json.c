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
	curl_easy_setopt(hcurl, CURLOPT_VERBOSE, 3L);

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

struct read_buffer {
	char *buffer;
	int current, max;
};

static size_t read_callback(char *dest, size_t size, size_t nmemb, void *userp)
{
	struct read_buffer *rd = (struct read_buffer *)userp;
	size_t buffer_size = size * nmemb;

	if(rd->current < rd->max) {
		/* copy as much as possible from the source to the destination */
		size_t copy_this_much = rd->max - rd->current;
		if (copy_this_much > buffer_size)
			copy_this_much = buffer_size;
		memcpy(dest, rd->buffer + rd->current, copy_this_much);

		rd->current -= copy_this_much;
		return copy_this_much; /* we copied this many bytes */
	}

	return 0; /* no more data left to deliver */
}

json_t *http_post_json(const char *uri, json_t *data) {

	curl_global_init(CURL_GLOBAL_DEFAULT);
	CURL *curl = curl_easy_init();

	if (curl != NULL) {

		struct curl_slist *list = NULL;
		list = curl_slist_append(list, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
		curl_easy_setopt(curl, CURLOPT_URL, uri);
		curl_easy_setopt(curl, CURLOPT_POST, 1L);
		curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);

		char *buffer = malloc(BUFFER_CHUNK);
		if (NULL == buffer)
			goto error;

		struct read_buffer read_data = {
			.buffer = buffer,
			.current = 0,
			.max = BUFFER_CHUNK
		};

		read_data.max = json_dumpb(data, read_data.buffer, read_data.max, 0);
		curl_easy_setopt(curl, CURLOPT_READDATA, &read_data);
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 3L);
		curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)read_data.max);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);

		buffer = malloc(BUFFER_CHUNK);
		if (NULL == buffer)
			goto error;

		struct write_buffer write_result = {
			.buffer = buffer,
			.current = 0,
			.max = BUFFER_CHUNK
		};

		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &write_result);

		CURLcode curl_result = curl_easy_perform(curl);
		free(read_data.buffer);

		curl_easy_cleanup(curl);

		if (CURLE_OK != curl_result) {
			fprintf(stderr, "curl told us %d\n", curl_result);
			goto error;
		}

		write_result.buffer[write_result.current] = '\0';

		json_error_t error;
		json_t *result = json_loadb(write_result.buffer, write_result.current, 0, &error);
		free(write_result.buffer);
		if( NULL == result)
			fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
		curl_global_cleanup();
		return result;
	}
error:
	curl_global_cleanup();
	return NULL;
}


//int main(int argc, char *argv[]) {

	//if (argc != 2) {
		//fprintf(stderr, "usage: %s <userId>\n", argv[0]);
		//exit(EXIT_FAILURE);
	//}
	//char uri[100];
	////sprintf(uri, "https://dummyjson.com/carts/user/%s", argv[1]);
	//sprintf(uri, "https://dummyjson.com/products/%s", argv[1]);
	//json_t *products_json = http_get_json(uri);
	//if (products_json == NULL) {
		//fprintf(stderr, "Error: http_get_json(%s)\n", uri);
		//exit(EXIT_FAILURE);
	//}
	//printf("%s", json_dumps(products_json, JSON_INDENT(4)));
	//json_decref(products_json);
    //return 0;
//}

//int main(int argc, char *argv[]) {

	//if (argc != 2) {
		//fprintf(stderr, "usage: %s <userId>\n", argv[0]);
		//exit(EXIT_FAILURE);
	//}
	//char uri[100];
	//sprintf(uri, "https://dummyjson.com/carts/user/%s", argv[1]);
	//json_t *carts_json = http_get_json(uri);
	//if (carts_json == NULL) {
		//fprintf(stderr, "Error: http_get_json(%s)\n", uri);
		//exit(EXIT_FAILURE);
	//}
	//printf("%s", json_dumps(carts_json, JSON_INDENT(4)));
	
	//return 0;
//}


//int main(int argc, char *argv[]) {

	//if (argc != 2) {
		//fprintf(stderr, "usage: %s <userId>\n", argv[0]);
		//exit(EXIT_FAILURE);
	//}

	///*------------------------------------------------------------------------*/
	//json_error_t error;
	//json_t *cart_json = json_loads("{\"userId\":1,\"products\":"
									//"[{\"id\":144,\"quantity\":3},"
									 //"{\"id\":97,\"quantity\":1},"
									 //"{\"id\":98,\"quantity\":2}]}", 0, &error);
	//if (cart_json == NULL) {
		//fprintf(stderr, "Error: on line %d: %s\n", error.line, error.text);
		//exit(EXIT_FAILURE);
	//}
	//json_t *new_cart_json = http_put_json("https://dummyjson.com/carts/add", cart_json);

	//printf("%s", json_dumps(new_cart_json, JSON_INDENT(4)));
//}
