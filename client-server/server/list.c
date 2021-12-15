#include "list.h" 

list *create(int data) {
	
	list *tmp = (list*)malloc(sizeof(list));
	tmp->value = data;	
	tmp->next = NULL;

	return(tmp);
}

void add_element_end(int data, list *head) {
	list *tmp = (list*)malloc(sizeof(list));
	tmp->value = data;
	tmp->next = NULL;
	list *p = head;
	while (p->next != NULL) p = p->next;
	p->next = tmp;
}

list *remove_all(list *head) {
	
	while (head != NULL) {
		list *p = head;
		head = head->next;
		free(p);
	}
	return NULL;
}

list *remove_element(int data, list *head) {

	list *tmp = head, *p = NULL;

	if (head == NULL) return NULL;

	while (tmp && tmp -> value != data) {
		p = tmp;
		tmp = tmp -> next;
	}
	// If the item to be removed is the first
	if (tmp == head) {
		free(head);
		return tmp->next;
	}

	if (!tmp) return head;
	p->next = tmp->next;	
	free(tmp);
	return head;
}

void print_list(list *head){
	list *tmp = head;
	while (tmp != NULL) {
		printf("%d ", tmp->value);
		tmp = tmp->next;
	}
}
