#include <malloc.h>

typedef struct Node
{
	int value;
	struct Node *next;
} list;

list *create(int data);
list *remove_all(list *head);
list *remove_element(int data, list *head);
void add_element_end(int data, list *head);
void print_list(list *head);