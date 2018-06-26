/********************************************************************
 *
 * $Id: List.h 1989 2015-10-16 18:19:52Z phil $
 *
 ********************************************************************
 */
/**
 *
 * Creation of a generic (simply linked) List structure.
 *
 * To create a list, one must provide two functions (one function to
 * compare / order elements, one function to display them). Unlike arrays,
 * indices begins with 1.
 *
 ********************************************************************/

#ifndef __List_H
#define __List_H

#include <stdlib.h>
#include "status.h"

/** Typical simple link structure: a Node is a "value / next" pair */
typedef struct Node {
  void    	*val;
  struct Node    *next;
} Node;

/** Comparison function for list elements.
 * Must follow the "strcmp" convention: result is negative if e1 is less
 * than e2, null if they are equal, and positive otherwise.
 */
typedef int (*compFun)   (void* e1, void* e2);

/** Display function for list elements */
typedef void(*prFun)   (void*);

/** The list embeds a counter for its size and the two function pointers */
typedef struct List {
  int nelts;
  Node * head;
  compFun sort;
  compFun comp;
  prFun pr;
} List;


/** Empty List creation by dynamic memory allocation (O(1)).
 * @param comp comparison function between elements (ala strcmp()) to sort by
 * @param comp comparison function between elements (ala strcmp()) to identify by
 * @param pr display function for list elements
 * @return a new (empty) list if memory allocation OK
 * @return 0 otherwise
 */
List*    newList	(compFun,compFun,prFun);

/** destroy the list by deallocating used memory (O(N)).
 * @param l the list to destroy */
void     delList	(List*);		

/** get the Nth element of the list (O(N)).
 * @param l the list
 * @param n the index of the element in list
 * @param e (out) the searched element
 * @return OK if element found
 * @return ERRINDEX if index out of list bounds
 */
status     nthInList	(List*,int,void**);	

/** Insert element at a given position in the list (O(N)).
 * @param l the list to store the element in
 * @param p the position of the insertion point
 * @param e the element to insert
 * @return ERRINDEX if position is out of list bounds
 * @return ERRALLOC if memory allocation failed
 * @return OK otherwise
 */
status     addListAt	(List*,int,void*);	

/** remove the element located at a given position in list (O(N)).
 * @param l the list to remove the element from
 * @param p the position of the element to remove
 * @param e (out) the removed element
 * @return ERRINDEX if position is out of list bounds
 * @return OK otherwise
 */
status     remFromListAt	(List*,int,void**);	

/** remove given element from given list (O(N)).
 * implies the user has given a comparison function.
 * @param l the list to remove the element from
 * @param e the element to remove
 * @return ERRABSENT if element is not found in list
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
 */
status     remFromList	(List*,void*);	

/** display list elements as "[ e1 ... eN ]" (O(N)).
 * The display of the element is delegated to the prFun function
 * @param l the list to display.
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
*/
status     displayList	(List*);		

/** sequencially call given function with each element of given list (O(NxF)).
 * @param l the list
 * @param f the function
 */
void    forEach		(List*,void(*)(void*));	

/** compute and return the number of elements in given list (O(1)).
 * @param l the list
 * @return the number of elements in given list
 */
int    lengthList	(List*);	

/** add given element to given list according to compFun function (O(N)).
 * @param l the list (supposedly sorted according to compFun function)
 * @param e the element to add
 * @return ERRALLOC if memory allocation failed
 * @return ERRUNABLE if no comparison function has been provided
 * @return OK otherwise
 */
status    addList	(List*,void*);	

/** tests whether the list contains given element (O(N)).
 * @param l the list
 * @param e the searched element
 * @return 0 if element is not found in list
 * @return 1 if element is at the head of the list (no predecessor)
 * @return (a pointer to) the predecessor of the search element otherwise
 */
Node*    isInList	(List*,void*);	

#endif
