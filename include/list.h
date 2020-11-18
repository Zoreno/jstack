/**
 * @file list.h
 * @author Joakim Bertils
 * @version 0.1
 * @date 2020-11-18
 * 
 * @brief Double linked list data structure interface
 * 
 * @copyright Copyright (C) 2020,
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https: //www.gnu.org/licenses/>.
 * 
 */

#ifndef _LIST_H
#define _LIST_H

#include <stdint.h>
#include <stddef.h>

//==============================================================================
// Definitions
//==============================================================================

/**
 * @brief Internal list node.
 * 
 * 
 */
typedef struct _list_node
{
    /**
     * @brief Pointer to the next node.
     * 
     * 
     */
    struct _list_node *next;

    /**
     * @brief Pointer to the previous node.
     * 
     * 
     */
    struct _list_node *prev;

    /**
     * @brief Pointer to the data contained in this node.
     * 
     * 
     */
    void *payload;
} list_node_t;

/**
 * @brief List head structure.
 * 
 * Contains pointer to the beginning and the end of a list.
 * Also contains the length of the list for fast query.
 */
typedef struct
{
    /**
     * @brief Pointer to the beginning of the list.
     * 
     * 
     */
    list_node_t *head;

    /**
     * @brief Pointer to the end of the list.
     * 
     * 
     */
    list_node_t *tail;

    /**
     * @brief The size of the list, in number of elements.
     * 
     * 
     */
    size_t length;
} list_t;

/**
 * @brief Compare function for use in the find function.
 * 
 * Should return zero on equality, and non-zero otherwise.
 * Implementation assumes that the function is commutative and pure.
 */
typedef int (*compare_func_t)(void *, void *);

//==============================================================================
// Interface functions
//==============================================================================

/**
 * @brief Creates a new list, allocating the list head on the heap.
 * 
 * 
 * @return Pointer to the newly allocated struct.
 */
list_t *list_create();

/**
 * @brief Deallocates all payloads in the list.
 * 
 * @param list Pointer to the list.
 * 
 */
void list_destroy(list_t *list);

/**
 * @brief Deallocates all internal list structures.
 * 
 * @param list Pointer to the list.
 * 
 */
void list_free(list_t *list);

/**
 * @brief Appends a node to the list.
 * 
 * @param list Pointer to the list.
 * @param item Pointer to the node to add.
 * 
 */
void list_append(list_t *list, list_node_t *item);

/**
 * @brief Appends an item to the list, allocating the internal node structure.
 * 
 * @param list Pointer to the list.
 * @param item Pointer to data to add to the list.
 * 
 */
void list_insert(list_t *list, void *item);

/**
 * @brief Finds an element in the list, by simple pointer comparison.
 * 
 * @param list Pointer to the list.
 * @param value Pointer to search for.
 * 
 * @return Pointer to list node if found, else NULL.
 */
list_node_t *list_find(list_t *list, void *value);

/**
 * @brief Finds an element in the list, by comparison function.
 * 
 * @param list Pointer to the list.
 * @param value Pointer to search for.
 * @param func Pointer to function doing the compare
 * 
 * @return Pointer to list node if found, else NULL.
 */
list_node_t *list_find_func(list_t *list, void *value, compare_func_t func);

/**
 * @brief Removes the element at a given index.
 * 
 * @param list Pointer to the list.
 * @param index Index to remove.
 * 
 */
void list_remove(list_t *list, size_t index);

/**
 * @brief Removes a node from the list.
 * 
 * @param list Pointer to the list.
 * @param node Pointer to the node to remove.
 * 
 */
void list_delete(list_t *list, list_node_t *node);

/**
 * @brief Removes the last element in the list.
 * 
 * @param list Pointer to the list.
 * 
 * @return Pointer to the removed list node.
 */
list_node_t *list_pop(list_t *list);

/**
 * @brief Removes the first element in the list.
 * 
 * @param list Pointer to the list.
 * 
 * @return Pointer to the removed list node.
 */
list_node_t *list_dequeue(list_t *list);

/**
 * @brief Creates a shallow copy of the list.
 * 
 * @param list Pointer to the list.
 * 
 * @return Pointer to the new list.
 */
list_t *list_copy(list_t *list);

/**
 * @brief Merges two list, appending all element from source to the dest and
 * freeing source.
 *
 * @param dest Pointer to the destination list.
 * @param source Pointer to the source list
 *
 */
void list_merge(list_t *dest, list_t *source);

#endif

//==============================================================================
// End of file.
//==============================================================================
