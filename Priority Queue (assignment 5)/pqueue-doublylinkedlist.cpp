/*************************************************************
 * File: pqueue-doublylinkedlist.cpp
 *
 * Implementation file for the DoublyLinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-doublylinkedlist.h"
#include "error.h"

DoublyLinkedListPriorityQueue::DoublyLinkedListPriorityQueue() {
	head = NULL;
	cellToRemove = NULL;
	length = 0;
}

DoublyLinkedListPriorityQueue::~DoublyLinkedListPriorityQueue() {
	while (head != NULL) {
		Cell* prev = head->prev;
		delete head;
		head = prev;
	}
	length = 0;
}

int DoublyLinkedListPriorityQueue::size() {
	return length;
}

bool DoublyLinkedListPriorityQueue::isEmpty() {
	return size() == 0;
}

void DoublyLinkedListPriorityQueue::enqueue(string value) {
	length++;
	Cell* cell = new Cell;
	cell->value = value;
	cell->next = NULL;

	if (head == NULL) { // when first element is added to list
		cell->prev = NULL;
		head = cell;
	} else { // when element is added to not empty list
		head->next = cell;
		cell->prev = head;
		head = cell;
	}
}

string DoublyLinkedListPriorityQueue::peek() {
	if (isEmpty())
		error("Queue is empty.");

	string result = head->value;
	Cell* cur = head;
	cellToRemove = cur;
	while (cur != NULL) { // iterates throught list
		if (result > cur->value) { // finds lexicographically smallest element
			result = cur->value;
			cellToRemove = cur;
		}
		cur = cur->prev;
	}
	return result;
}

string DoublyLinkedListPriorityQueue::dequeueMin() {
	string result = peek();
	length--;
	
	// cellToRemove is spliced out of list and previous and next cells are linked to each other

	if (cellToRemove->prev != NULL) { 
		(cellToRemove->prev)->next = cellToRemove->next; 
	}
	if (cellToRemove->next != NULL) {
		(cellToRemove->next)->prev = cellToRemove->prev;
	} else { // if first element is deleted
		if (cellToRemove->prev != NULL) {
			head = cellToRemove->prev;
		} else {
			head = NULL;
		}
	}

	delete cellToRemove;

	return result;
}

