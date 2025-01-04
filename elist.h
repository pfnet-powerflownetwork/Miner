#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

/*
 * Doubly Linked List Implementation
 * 
 * This alternative implementation retains the core functionality while
 * organizing the structure and logic in a different style.
 */

struct list_head {
	struct list_head *next;
	struct list_head *prev;
};

/* Initialize a list head */
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); \
	(ptr)->prev = (ptr); \
} while (0)

/* Internal function for insertion between two entries */
static inline void __list_insert(struct list_head *new_entry,
				 struct list_head *prev,
				 struct list_head *next) {
	prev->next = new_entry;
	new_entry->prev = prev;
	new_entry->next = next;
	next->prev = new_entry;
}

/* Add a new entry at the start of the list */
static inline void list_add(struct list_head *new_entry, struct list_head *head) {
	__list_insert(new_entry, head, head->next);
}

/* Add a new entry at the end of the list */
static inline void list_add_tail(struct list_head *new_entry, struct list_head *head) {
	__list_insert(new_entry, head->prev, head);
}

/* Internal function to unlink a list entry */
static inline void __list_remove(struct list_head *prev, struct list_head *next) {
	prev->next = next;
	next->prev = prev;
}

/* Remove a list entry */
static inline void list_remove(struct list_head *entry) {
	__list_remove(entry->prev, entry->next);
	entry->next = NULL;
	entry->prev = NULL;
