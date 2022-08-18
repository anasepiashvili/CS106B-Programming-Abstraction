/*************************************************************
 * File: pqueue-linkedlist.cpp
 *
 * Implementation file for the LinkedListPriorityQueue
 * class.
 */
 
#include "pqueue-linkedlist.h"
#include "error.h"

LinkedListPriorityQueue::LinkedListPriorityQueue() {
	head = NULL;
	length = 0;
}

LinkedListPriorityQueue::~LinkedListPriorityQueue() {
	while (head != NULL) {
		Cell* next = head->next;
		delete head;
		head = next;
	}
	length = 0;
}

int LinkedListPriorityQueue::size() {
	return length;
}

bool LinkedListPriorityQueue::isEmpty() {
	return length == 0;
}

void LinkedListPriorityQueue::insert(Cell* cell, string value) {
	Cell* prev = head;  // previous element
	Cell* cur = head->next; // current element
	bool notLast = false;
	while(cur != NULL) {   // iterates through list
		if (value < cur->value) { // element is inserted in the middle
			notLast = true; // bool becomes true when it's not added last
			cell->next = cur;
			prev->next = cell;
			break;
		}
		prev = cur;
		cur = cur->next;
	}
	if (!notLast) { // element is added last
		prev->next = cell;
		cell->next = NULL;
		notLast = false;
	}
}

void LinkedListPriorityQueue::enqueue(string value) {
	length++;
	Cell* cell = new Cell;
	cell->value = value;
	if (head == NULL) {  // when first element is added to empty list
		cell->next = NULL;
		head = cell;
	} else { // when element is added to not empty list
	
		if (value < head->value) { // when new element is added first on the list
			cell->next = head;
			head = cell;
		} else insert(cell, value);
	}
}

string LinkedListPriorityQueue::peek() {
	if (isEmpty())
		error("Queue is empty.");

	return head->value;
}

string LinkedListPriorityQueue::dequeueMin() {
	string result = peek();
	length--;
	
	Cell* next = head->next;
	delete head;
	head = next;

	return result;
}

