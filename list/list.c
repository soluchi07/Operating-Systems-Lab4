// list/list.c
// 
// Implementation for linked list.
//
// <Author>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

/*
 * This code is written with the following assumptions:
 * - Lists are 1-indexed.
 * - Index-based functions return -1 when the requested index is invalid.
 */

/*
 * Allocates and initializes an empty list.
 */
list_t *list_alloc() {
  list_t *mylist = malloc(sizeof(list_t));

  if (!mylist) {
    return NULL;
  }

  mylist->head = NULL;
  return mylist;
}

/*
 * Frees every node in the list and then frees the list itself.
 */
void list_free(list_t *l) {
  if (!l) {
    return;
  }

  node_t *curr = l->head;

  while (curr) {
    node_t *temp = curr;
    curr = curr->next;
    free(temp);
  }

  free(l);
}

/*
 * Prints the list using listToString().
 */
void list_print(list_t *l) {
  char *text = listToString(l);

  if (text) {
    printf("Here is the printed list: %s\n", text);
    free(text);
  }
}

/*
 * Converts the list to a string in the format:
 * value->value->value->NULL
 *
 * The buffer grows dynamically as needed.
 */
char *listToString(list_t *l) {
  if (!l) {
    return NULL;
  }

  size_t capacity = 64;
  size_t length = 0;

  char *buf = malloc(capacity);

  if (!buf) {
    return NULL;
  }

  buf[0] = '\0';

  node_t *curr = l->head;

  while (curr != NULL) {
    char tbuf[32];

    int written = snprintf(tbuf, sizeof(tbuf), "%d->", curr->value);

    if (written < 0) {
      free(buf);
      return NULL;
    }

    size_t needed = length + (size_t)written + 1;

    /*
     * Increase the buffer until it is large enough for the
     * next value and the terminating null character.
     */
    while (needed > capacity) {
      capacity *= 2;

      char *temp = realloc(buf, capacity);

      if (!temp) {
        free(buf);
        return NULL;
      }

      buf = temp;
    }

    strcat(buf, tbuf);
    length += (size_t)written;

    curr = curr->next;
  }

  const char *end = "NULL";
  size_t end_length = strlen(end);
  size_t needed = length + end_length + 1;

  while (needed > capacity) {
    capacity *= 2;

    char *temp = realloc(buf, capacity);

    if (!temp) {
      free(buf);
      return NULL;
    }

    buf = temp;
  }

  strcat(buf, end);

  return buf;
}

/*
 * Returns the number of nodes in the list.
 */
int list_length(list_t *l) {
  if (!l || !l->head) {
    return 0;
  }

  int count = 0;
  node_t *curr = l->head;

  while (curr != NULL) {
    count++;
    curr = curr->next;
  }

  return count;
}

/*
 * Adds a node to the end of the list.
 */
void list_add_to_back(list_t *l, elem value) {
  if (!l) {
    return;
  }

  node_t *new_node = getNode(value);

  if (!new_node) {
    return;
  }

  /*
   * If the list is empty, the new node becomes the head.
   */
  if (!l->head) {
    l->head = new_node;
    return;
  }

  node_t *curr = l->head;

  while (curr->next) {
    curr = curr->next;
  }

  curr->next = new_node;
}

/*
 * Adds a node to the beginning of the list.
 */
void list_add_to_front(list_t *l, elem value) {
  if (!l) {
    return;
  }

  node_t *cur_node = getNode(value);

  if (!cur_node) {
    return;
  }

  cur_node->next = l->head;
  l->head = cur_node;
}

/*
 * Creates and initializes a new node.
 *
 * This helper is private to this source file.
 */
node_t *getNode(elem value) {
  node_t *mynode = malloc(sizeof(node_t));

  if (!mynode) {
    return NULL;
  }

  mynode->value = value;
  mynode->next = NULL;

  return mynode;
}

/*
 * Adds a node at the specified 1-based index.
 *
 * Index 1 inserts at the front.
 * An index greater than length + 1 is ignored.
 */
void list_add_at_index(list_t *l, elem value, int index) {
  if (!l || index < 1) {
    return;
  }

  if (index == 1) {
    list_add_to_front(l, value);
    return;
  }

  /*
   * To insert at index i, we need to reach the node
   * at index i - 1.
   */
  node_t *curr = l->head;
  int counter = 1;

  while (curr != NULL && counter < index - 1) {
    curr = curr->next;
    counter++;
  }

  /*
   * If curr is NULL, the requested index is out of bounds.
   */
  if (!curr) {
    return;
  }

  node_t *new_node = getNode(value);

  if (!new_node) {
    return;
  }

  new_node->next = curr->next;
  curr->next = new_node;
}

/*
 * Removes and returns the last element in the list.
 *
 * Returns -1 if the list is empty or NULL.
 */
elem list_remove_from_back(list_t *l) {
  if (!l || !l->head) {
    return -1;
  }

  /*
   * Handle a one-node list separately.
   */
  if (!l->head->next) {
    node_t *removed_node = l->head;
    elem value = removed_node->value;

    l->head = NULL;
    free(removed_node);

    return value;
  }

  /*
   * Stop at the second-to-last node.
   */
  node_t *curr = l->head;

  while (curr->next->next) {
    curr = curr->next;
  }

  node_t *removed_node = curr->next;
  elem value = removed_node->value;

  curr->next = NULL;
  free(removed_node);

  return value;
}

/*
 * Removes and returns the first element in the list.
 *
 * Returns -1 if the list is empty or NULL.
 */
elem list_remove_from_front(list_t *l) {
  if (!l || !l->head) {
    return -1;
  }

  node_t *removed_node = l->head;
  elem value = removed_node->value;

  l->head = removed_node->next;

  free(removed_node);

  return value;
}

/*
 * Removes and returns the element at the specified 1-based index.
 *
 * Returns -1 if the index is invalid.
 */
elem list_remove_at_index(list_t *l, int index) {
  if (!l || !l->head || index < 1) {
    return -1;
  }

  if (index == 1) {
    return list_remove_from_front(l);
  }

  /*
   * To remove index i, we need to reach the node
   * at index i - 1.
   */
  node_t *curr = l->head;
  int counter = 1;

  while (curr != NULL && counter < index - 1) {
    curr = curr->next;
    counter++;
  }

  /*
   * curr must exist, and there must be a node after it
   * at the requested index.
   */
  if (!curr || !curr->next) {
    return -1;
  }

  node_t *removed_node = curr->next;
  elem value = removed_node->value;

  curr->next = removed_node->next;

  free(removed_node);

  return value;
}

/*
 * Returns true if value exists in the list.
 */
bool list_is_in(list_t *l, elem value) {
  if (!l || !l->head) {
    return false;
  }

  node_t *curr = l->head;

  while (curr) {
    if (curr->value == value) {
      return true;
    }

    curr = curr->next;
  }

  return false;
}

/*
 * Returns the element at the specified 1-based index.
 *
 * Returns -1 if the index is invalid.
 */
elem list_get_elem_at(list_t *l, int index) {
  if (!l || !l->head || index < 1) {
    return -1;
  }

  node_t *curr = l->head;
  int counter = 1;

  while (curr != NULL && counter < index) {
    curr = curr->next;
    counter++;
  }

  if (!curr) {
    return -1;
  }

  return curr->value;
}

/*
 * Returns the 1-based index of the first occurrence of value.
 *
 * Returns -1 if value is not in the list.
 */
int list_get_index_of(list_t *l, elem value) {
  if (!l || !l->head) {
    return -1;
  }

  int index = 1;
  node_t *curr = l->head;

  while (curr) {
    if (curr->value == value) {
      return index;
    }

    curr = curr->next;
    index++;
  }

  return -1;
}

/*
 * Notes:
 * - Lists are 1-indexed.
 * - Index-based functions return -1 for invalid indexes.
 * - getNode() is private to this source file.
 */