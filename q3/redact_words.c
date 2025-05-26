
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void clear_word_buffer(char *word_buffer) {
	memset(word_buffer, '\0', sizeof(word_buffer));
}

struct RedactNode {
	char *word;
	struct RedactNode *next;
}

struct RedactList {
	struct RedactNode *head;
}

struct RedactList *create_redact_list(void) {
	struct RedactList *new_list = (struct RedactList *)malloc(sizeof(struct RedactList));
	new_list->head = NULL;
} 

void add_redact_word(struct RedactList list, const char *word) {

	struct RedactNode *new_node = (struct RedactNode *)malloc(sizeof(struct RedactNode));
	new_node->next = NULL;
	size_t word_len = strlen(word);
	new_node->word = (char *)malloc(word_len+1);
	strcpy(new_node->word, word);

	if (list.head == NULL) {
		list.head = new_node;
	} else {
		struct RedactNode *curr_node = list.head;
		while (curr_node->next != NULL) {
			curr_node = curr_node->next;
		}
		curr_node->next = new_node;
	}
}

bool should_redact_word(struct RedactList list, const char *word) {
	struct RedactNode *curr_node = list.head;
	while (curr_node != NULL) {
		if (strcasecmp(curr_node->word, word) == 0) return true;
		curr_node = curr_node->next;
	}
	return false;
}

void redact_words(const char *text_filename, const char *redact_words_filename) {

	FILE *file = NULL;
	char input_buffer[1000];
	char output_buffer[1000];
	char word_buffer[50];

	fegts()




}

void test() {

	
	//printf("a=%d, and b=%d \n", a, b);

}

int main() {
	
	//printf("%d\n", );
	//test();
	
	return 0;
}	

