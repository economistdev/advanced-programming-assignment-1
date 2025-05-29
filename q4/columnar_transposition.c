
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
	for (int i=0; i < grid->shape[0]; i++) {
		printf("i = %d\n", i);
		for (int j=0; j < grid->shape[1]; j++) {
			printf("j = %d\n", j);
			printf("element = %s\n", msg);
			*get_grid_element(grid, i, j) = msg[i*grid->shape[1] + j];
		}
	}
}


int *sort_asc(char *arr) {
	int *sorted_idx = (int *)calloc(strlen(arr), sizeof(char));
	
	for (int i = 0; i < (strlen(arr)-1); i++) {
		int smaller_idx = i;
		for (int j = i+1; j < strlen(arr); j++) {
			if (arr[i] > arr[j]) {
				smaller_idx = j;
			}
		}
		char temp = arr[i];
		arr[i] = arr[smaller_idx];
		arr[smaller_idx] = temp;
		sorted_idx[i] = smaller_idx; //place the index in the sorted idx arr
	}

	return sorted_idx;
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
	char *encrypted_str = (char *)calloc(grid->size, sizeof(char));

	for (int row=0; row<grid->shape[0]; row++) {
		for (int col=0; col<grid->shape[1]; col++) {
			encrypted_str[row*grid->shape[0] + col] = *get_grid_element(grid, row, grid->col_order[col]);

		}
	}

	return encrypted_str;
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

	shape[0] = msg_rows+1; //+1 for the key row
	shape[1] = col_len;

}

char *read_string(const char *filename){
	size_t file_size = 100000000;

	FILE *fptr = fopen(filename, "r");

	char *buffer = (char *)calloc(file_size, sizeof(char));
	char *buffer_processed = (char *)calloc(file_size, sizeof(char));

	fread(buffer, 1, file_size, fptr);

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

	int shape[2];
	encryption_find_dimensions(shape, msg, key);

	char *combined_msg = combine_key_msg(shape, msg, key);

	struct Grid *grid = init_grid(shape, combined_msg);

	*result = encrypt_grid(grid);
        
}

int decrypt_columnar(const char *message_filename, const char *key_filename, char **result){
    return 1;   
}

void test() {

}

int main() {
	char *test;
	encrypt_columnar("msg.txt", "key.txt", &test);
	printf("encrypted str is: %s\n", test);
	
	return 0;
}	

