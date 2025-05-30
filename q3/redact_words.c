
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

void clear_word_buffer(char *word_buffer) {
	int size_word = strlen(word_buffer);
	memset(word_buffer, '\0', size_word);
}

struct WordNode {
	char *word;
	struct WordNode *next;
};

struct WordList {
	struct WordNode *head;
};

struct WordList *new_word_list(void) {
	struct WordList *new_list = (struct WordList *)malloc(sizeof(struct WordList));
	new_list->head = NULL;
	return new_list;
}

void free_word_list(struct WordList *list) {
	struct WordNode *curr_node = list->head;
	list->head = NULL;

	struct WordNode *next_node;
	while(curr_node != NULL) {
		next_node = curr_node->next;
		free(curr_node);
		curr_node = next_node;
	}
}

void add_word_to_list(struct WordList *list, const char *word) {

	struct WordNode *new_node = (struct WordNode *)malloc(sizeof(struct WordNode));
	new_node->next = NULL;
	size_t word_len = strlen(word);
	new_node->word = (char *)malloc(word_len+1);
	strcpy(new_node->word, word);

	if (list->head == NULL) {
		list->head = new_node;
	} else {
		struct WordNode *curr_node = list->head;
		while (curr_node->next != NULL) {
			curr_node = curr_node->next;
		}
		curr_node->next = new_node;
	}
}

bool should_redact_word(struct WordList *redact_list, const char *word) {
	struct WordNode *curr_node = redact_list->head;
	while (curr_node != NULL) {
		if (strcasecmp(curr_node->word, word) == 0) return true;
		curr_node = curr_node->next;
	}
	return false;
}

struct WordList *parse_redacted_words(char *redact_buffer) {
	struct WordList *word_list = new_word_list();
	
	char curr_char = redact_buffer[0];
	size_t curr_index = 0;
	char word_buffer[100000] = {0};
	size_t word_buffer_index = 0;

	int size_of_file = strlen(redact_buffer);

	while (true) {
		if (size_of_file == curr_index+1) {
			if (word_buffer_index > 0) {
				add_word_to_list(word_list, word_buffer);
			}
			break;
		}
		if (!isalpha(curr_char) && (word_buffer_index > 0)) {
			word_buffer_index = 0;
			add_word_to_list(word_list, word_buffer);
			clear_word_buffer(word_buffer);
		} else if (isalpha(curr_char)) {
			word_buffer[word_buffer_index++] = curr_char;
		}

		curr_char = redact_buffer[++curr_index];
	}

	return word_list;
}

void process_word(char *word_buffer, struct WordList *redact_list) {
	if (should_redact_word(redact_list, word_buffer)) {
		int word_size = strlen(word_buffer);
		memset(word_buffer, '*', word_size);
	} 
}

void process_redaction(const char *input_buffer, char *output_buffer,struct WordList *redact_list) {
	strcpy(output_buffer, input_buffer);

	char curr_char = input_buffer[0];
	size_t curr_char_index = 0;

	char word_buffer[100000] = {0};
	size_t word_buffer_index = 0;

	size_t start_word_index = 0;

	int size_of_file = strlen(output_buffer);
	
	while(true) {
		if ((size_of_file == curr_char_index+1)) {
			if (word_buffer_index > 0) {
				process_word(word_buffer, redact_list);
				memcpy(output_buffer+start_word_index, word_buffer, strlen(word_buffer));
			}
			break;
		} else if (!isalpha(curr_char) && (word_buffer_index > 0)) {
			process_word(word_buffer, redact_list);
			memcpy(output_buffer+start_word_index, word_buffer, strlen(word_buffer));
			word_buffer_index = 0;
			clear_word_buffer(word_buffer);
			
		} else if (isalpha(curr_char)) {
			if (word_buffer_index == 0) start_word_index = curr_char_index;
			word_buffer[word_buffer_index++] = curr_char;

		}
		curr_char = input_buffer[++curr_char_index];
	}
}

void redact_words(const char *text_filename, const char *redact_words_filename) {

	FILE *input_fptr = fopen(text_filename, "r");
	FILE *redact_fptr = fopen(redact_words_filename, "r");
	FILE *output_fptr = fopen("result.txt", "w");

	size_t file_size = 10000000;
	char *input_buffer = (char *)calloc(file_size, sizeof(char));
	char *redact_buffer = (char *)calloc(file_size, sizeof(char));
	char *output_buffer = (char *)calloc(file_size, sizeof(char));

	fread(input_buffer, 1, file_size, input_fptr);
	fread(redact_buffer,1, file_size, redact_fptr);

	struct WordList *redact_list = parse_redacted_words(redact_buffer);

	process_redaction(input_buffer, output_buffer, redact_list);

	free_word_list(redact_list);

	fputs(output_buffer, output_fptr);

	fclose(input_fptr);
	fclose(redact_fptr);
	fclose(output_fptr);

	free(input_buffer);
	free(redact_buffer);
	free(output_buffer);

	
}

void test() {

	
	//printf("a=%d, and b=%d \n", a, b);

}

int main() {
	
	redact_words("input.txt", "redact.txt");

	
	return 0;
}	

