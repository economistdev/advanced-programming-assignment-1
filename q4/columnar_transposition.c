
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

struct Grid {
	int shape[2];
	char **arr;
}

struct Grid *init_grid(int *shape) {
	
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

	//printf("this is the process buffer: %s\n", buffer_processed);

	return buffer_processed;

}


void encrypt_columnar(const char *message_filename, const char *key_filename, char **result){
        
}


int decrypt_columnar(const char *message_filename, const char *key_filename, char **result){
    return 1;   
}

void test() {

}

int main() {
	read_string("read_string.txt");
	
	return 0;
}	

