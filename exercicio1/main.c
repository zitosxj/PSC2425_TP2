#include <stdio.h>
#include <jansson.h>
#include "http_get_json.h"

#define MAX_URI 256

#define PRODUCT_REVIEWS_KEY				"reviews"
#define PRODUCT_TITLE_KEY				"title"
#define REVIEW_RATING_KEY				"rating"
#define REVIEW_REVIEWER_NAME_KEY		"reviewerName"
#define REVIEW_COMMENT_KEY				"comment"

typedef struct {
	int rating;
	char *reviewer_name;
	char *comment;
} product_review_t;


//Process review array
product_review_t *product_review_get(json_t *reviews_array) {
	
	static json_t *st_reviews_array = NULL;
	static int idx = 0;
	if (reviews_array != NULL) {
		st_reviews_array = reviews_array;
		idx = 0;
	}
//	printf("st_reviews_array: %s\n", json_dumps( st_reviews_array, JSON_INDENT(4)));

	json_t *review = json_array_get(st_reviews_array, idx);
	if (review == NULL) 
		return NULL;

//	printf("review: %s\n", json_dumps( review, JSON_INDENT(4)));
		
	idx += 1;

	product_review_t *product_review = (product_review_t*)malloc(
		sizeof(product_review_t));

	int r = json_unpack(review, "{s:i, s:s, s:s}", 
		REVIEW_RATING_KEY, &product_review->rating,
		REVIEW_COMMENT_KEY, &product_review->comment,
		REVIEW_REVIEWER_NAME_KEY, &product_review->reviewer_name
		 );

	if (r != 0)
		return NULL;

	product_review->rating = json_integer_value(
		json_object_get(review, REVIEW_RATING_KEY)
	);

	return product_review;
}


//Process get request 
void request_json_data_from_api(char *uri){

	json_t *get_reponse_json = http_get_json(uri);
	
	if (get_reponse_json == NULL)
		goto on_error;

	json_t *reviews_data;
	char * title;

	int r = json_unpack(get_reponse_json, "{s:s, s:o}", 
		PRODUCT_TITLE_KEY, &title,
		PRODUCT_REVIEWS_KEY, &reviews_data);

	if (r != 0)
		goto on_error;

//	printf("%s\n", json_dumps(get_reponse_json, JSON_INDENT(4)));
		
	printf("Title: %s\n", title);

	product_review_t *review = product_review_get(reviews_data);
	while (review != NULL) {
		printf("\tRating: %d\n", review->rating);
		printf("\tComment: %s\n", review->comment);
		printf("\tReviewerName: %s\n", review->reviewer_name);
		printf("------------------------------\n");
		free(review);
		review = product_review_get(NULL);
	}

on_error:
	json_decref(get_reponse_json);
}

int main(){

	char uri[MAX_URI] = "https://dummyjson.com/products/1";

	request_json_data_from_api(uri);

	return 0;
}
