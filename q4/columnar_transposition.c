
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

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

char *encrypt_grid(struct Grid *grid) {
	char *processed_str = (char *)calloc((grid->shape[0]-1)*grid->shape[1], sizeof(char));

	for (int row=0; row<grid->shape[0]; row++) {
		for (int col=0; col<grid->shape[1]; col++) {
			processed_str[row*grid->shape[1] + col] = *get_grid_element(grid, row+1, grid->col_order[col]);

		}
	}

	return processed_str;
}

char *decrypt_grid(struct Grid *grid) {
	char *processed_str = (char *)calloc((grid->shape[0]-1)*grid->shape[1], sizeof(char));

	for (int row=0; row<grid->shape[0]; row++) {
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

	int padding_count = str_size - strlen(combined_str);
	while (padding_count > 0) {
		combined_str[str_size - padding_count] = 'X';
		padding_count--;
	}

	free(msg);
	free(key);

	return combined_str;
	
}

void encryption_find_dimensions(int *shape, char *msg, char *key) {
	int col_len = strlen(key);
	int msg_len = strlen(msg);
	
	while (msg_len % col_len != 0) {
		msg_len++;
	}
	
	int msg_rows = msg_len/col_len;

	shape[0] = msg_rows +1; //+1 for the key row
	shape[1] = col_len;

}

char *read_string(const char *filename){
	size_t file_size = 10000000;

	FILE *fptr = fopen(filename, "r");

	char *buffer = (char *)calloc(file_size, sizeof(char));
	char *buffer_processed = (char *)calloc(file_size, sizeof(char));

	fread(buffer, 1, file_size, fptr);
	if (strlen(buffer) == 0) {
		free(buffer);		
		return buffer_processed;
	}

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
        
}

int decrypt_columnar(const char *message_filename, const char *key_filename, char **result){
	char *msg = read_string(message_filename);
	char *key = read_string(key_filename);

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

	return 1;
}

void test() {

}

int main() {
	char *test;
	FILE *fptr = fopen("msg_encrypted.txt", "w");


	encrypt_columnar("msg.txt", "key.txt", &test);
	printf("encrypted str is: %s\n", test);

	fputs(test, fptr);
	free(test);
	fclose(fptr);

	int success = decrypt_columnar("msg_encrypted.txt", "key.txt", &test);
	printf("success result is: %d\n", success);
	printf("decrypted str is: %s\n", test);

	
	
	return 0;
}	

