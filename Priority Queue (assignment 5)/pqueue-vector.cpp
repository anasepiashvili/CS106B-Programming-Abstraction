/*************************************************************
 * File: pqueue-vector.cpp
 *
 * Implementation file for the VectorPriorityQueue
 * class.
 */
 
#include "pqueue-vector.h"
#include "error.h"

VectorPriorityQueue::VectorPriorityQueue() {
	
}

VectorPriorityQueue::~VectorPriorityQueue() {
	vect.clear();
}

int VectorPriorityQueue::size() {
	return vect.size();
}

bool VectorPriorityQueue::isEmpty() {
	return size() == 0;
}

void VectorPriorityQueue::enqueue(string value) {
	vect.add(value);
}

string VectorPriorityQueue::peek() {
	if(isEmpty()) 
		error("Queue is empty.");
	
	string result = vect[0];
	for (int i = 0; i < size(); i++) {
		if (vect[i] < result) 
			result = vect[i];
	}
	return result;
}

string VectorPriorityQueue::dequeueMin() {
	if(isEmpty()) 
		error("Queue is empty.");
	
	string result = vect[0];
	int index = 0;
	for (int i = 0; i < size(); i++) {
		if (vect[i] < result) {
			result = vect[i];
			index = i;
		}
	}
	vect.remove(index);
	return result;
}

