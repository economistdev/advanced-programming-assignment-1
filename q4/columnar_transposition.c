
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LINE_SIZE 1024

struct Grid {
	int shape[2];
	int size;
	char *arr;
	int *col_order;
};

char *get_grid_element(struct Grid *grid, int row, int col) {
	int offset = row*grid->shape[1] + col;
	return &(grid->arr[offset]);
}

void populate_grid(struct Grid *grid, char *msg) {
	for (int row=0; row < grid->shape[0]; row++) {
		for (int col=0; col < grid->shape[1]; col++) {
			*get_grid_element(grid, row, col) = msg[row*grid->shape[1] + col];
		}
	}
}


int *sort_asc(char *key) {
	char *key_sorted = (char *)calloc(strlen(key)+1, sizeof(char));
	memcpy(key_sorted, key, strlen(key));
	int *key_sorted_idx = (int *)calloc(strlen(key), sizeof(int));
	
	for (int i = 0; i < (strlen(key)); i++) {
		int smaller_idx = i;
		char smaller_val = key_sorted[i];
		for (int j = i; j < strlen(key); j++) {
			if (smaller_val > key_sorted[j]) {
				smaller_idx = j;
				smaller_val = key_sorted[j];
			}
		}
		char temp = key_sorted[i];
		key_sorted[i] = smaller_val;
		key_sorted[smaller_idx] = temp;

	}
	//find orig idx of smaller val
	for (int i=0; i<strlen(key); i++) {
		for (int j=0; j<strlen(key); j++) {
			if (key_sorted[i] == key[j]) {
				key_sorted_idx[i] = j;
			}
		}
	}

	return key_sorted_idx;
}

void calculate_col_order_from_key(struct Grid *grid) {
	char *key = (char *)calloc(grid->shape[1]+1, sizeof(char));
	memcpy(key, grid->arr, grid->shape[1]);
	grid->col_order = sort_asc(key);

	free(key);
}

struct Grid *init_grid(int *shape, char *msg) { //shape[0] = rows, shape[1] == columns
	struct Grid *grid = (struct Grid *)malloc(sizeof(struct Grid));
	grid->shape[0] = shape[0];
	grid->shape[1] = shape[1];
	grid->size = shape[0]*shape[1];
	grid->arr = (char *)calloc(shape[0]*shape[1], sizeof(char)); 
	populate_grid(grid, msg);
	calculate_col_order_from_key(grid);
	
	return grid;
}

void free_grid(struct Grid *grid) {
	free(grid->arr);
	free(grid->col_order);
	free(grid);
}

char *encrypt_grid(struct Grid *grid) {
	char *processed_str = (char *)calloc((grid->shape[0]-1)*grid->shape[1], sizeof(char));

	for (int row=0; row<grid->shape[0]-1; row++) {
		for (int col=0; col<grid->shape[1]; col++) {
			processed_str[row*grid->shape[1] + col] = *get_grid_element(grid, row+1, grid->col_order[col]);

		}
	}

	return processed_str;
}

char *decrypt_grid(struct Grid *grid) {
	char *processed_str = (char *)calloc((grid->shape[0]-1)*grid->shape[1], sizeof(char));

	for (int row=0; row<grid->shape[0]-1; row++) {
		for (int col=0; col<grid->shape[1]; col++) {
			processed_str[row*grid->shape[1] + grid->col_order[col]] = *get_grid_element(grid, row+1, col);

		}
	}

	return processed_str;
}

char *combine_key_msg(int *shape, char *msg, char *key) {
	int str_size = shape[0]*shape[1];
	char *combined_str = (char *)calloc(str_size+1, sizeof(char));

	memcpy(combined_str, key, strlen(key));
	memcpy(combined_str+strlen(key), msg, strlen(msg));

	for (size_t i = strlen(key) + strlen(msg); i < str_size; i++) {
    	combined_str[i] = 'X';
	}

	free(msg);
	free(key);

	return combined_str;
	
}

void encryption_find_dimensions(int *shape, char *msg, char *key) {
	int col_len = strlen(key);
	int msg_len = strlen(msg);

	if (col_len == 0) {
		shape[0] = 0;
		shape[1] = 0;
	}
	
	while (msg_len % col_len != 0) {
		msg_len++;
	}
	
	int msg_rows = msg_len/col_len;

	shape[0] = msg_rows +1; //+1 for the key row
	shape[1] = col_len;

}

char *read_string(const char *filename){
	size_t buffer_size = 10000;
	
	FILE *fptr = fopen(filename, "r");
	if (fptr == NULL) {
		return NULL;
	}
	
	
	char *buffer = (char *)calloc(buffer_size, sizeof(char));
	if (buffer == NULL) {
		fclose(fptr);
		return NULL;
	}
	
	char line_buffer[MAX_LINE_SIZE] = {0};
	size_t current_buffer_size = buffer_size;
	size_t buffer_content_len = 0;
	
	while(fgets(line_buffer, MAX_LINE_SIZE, fptr) != NULL) {
		size_t line_buffer_content_len = strlen(line_buffer);
		while(current_buffer_size < (buffer_content_len+line_buffer_content_len+1)) {
			buffer = realloc(buffer, current_buffer_size*2);
			if (buffer == NULL){
				free(buffer);
				fclose(fptr);
				return NULL;
			}
			current_buffer_size *= 2;
		}

		strcpy(buffer+buffer_content_len, line_buffer);
		buffer_content_len += line_buffer_content_len;
	}
	fclose(fptr);
	
	char *buffer_processed = (char *)calloc(buffer_content_len+1, sizeof(char));

	size_t buffer_idx = 0;
	size_t buffer_processed_idx = 0;
	char curr_char = buffer[0];
	while (curr_char != '\0') {
		if (isalnum(curr_char)) {
			buffer_processed[buffer_processed_idx++] = toupper(curr_char);
		}
		curr_char = buffer[++buffer_idx];
	}

	free(buffer);

	return buffer_processed;

}

void encrypt_columnar(const char *message_filename, const char *key_filename, char **result){

	char *msg = read_string(message_filename);
	char *key = read_string(key_filename);

	if (key == NULL) {
		*result = NULL;
		return;
	}
	if (strlen(key) == 0) {
		free(key);
		free(msg);
		*result = NULL;
		return;
	}

	int shape[2];
	encryption_find_dimensions(shape, msg, key);

	char *combined_msg = combine_key_msg(shape, msg, key);

	struct Grid *grid = init_grid(shape, combined_msg);

	*result = encrypt_grid(grid);
	
	free(combined_msg);
	free_grid(grid);        
}

int decrypt_columnar(const char *message_filename, const char *key_filename, char **result){
	char *msg = read_string(message_filename);
	char *key = read_string(key_filename);

	if (key == NULL) {
		*result = NULL;
		return 0;
	}
	if (strlen(key) == 0) {
		free(key);
		free(msg);
		*result = NULL;
		return 0;
	}

	int shape[2];
	encryption_find_dimensions(shape, msg, key);

	char *combined_msg = combine_key_msg(shape, msg, key);

	struct Grid *grid = init_grid(shape, combined_msg);

	*result = decrypt_grid(grid);

	free(combined_msg);
	free_grid(grid);

	return 1;
}

void test() {

}

int main() {
	char *test;
	FILE *fptr = fopen("msg_encrypted.txt", "w");


	encrypt_columnar("msg.txt", "key.txt", &test);

	if (test != "") {
		fputs(test, fptr);
		free(test);
	}
	fclose(fptr);

	int success = decrypt_columnar("msg_encrypted.txt", "key.txt", &test);
	printf("success result is: %d\n", success);
	//printf("decrypted str is: %s\n", test);

	
	
	return 0;
}	

