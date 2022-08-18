/******************************************************************************
 * File: Trailblazer.cpp
 *
 * Implementation of the graph algorithms that comprise the Trailblazer
 * assignment.
 */

#include "Trailblazer.h"
#include "TrailblazerGraphics.h"
#include "TrailblazerTypes.h"
using namespace std;

/* Creates final path, puts chosen locations in vector in order from
 * start to end.
 */
void makePath(Vector<Loc>& path, Loc curr, Grid<node>& nodes, Loc start) {
	Stack<Loc> stack;  // is used to reverse order of elements 
	while (curr != start) {  
		stack.push(curr);
		curr = nodes[curr.row][curr.col].parent;
	}
	stack.push(start);
	while (!stack.isEmpty()) {
		path.add(stack.pop());
	}
}

/* For each node v connected to curr by an edge of length L:
 * If v is gray: colors v yellow, sets v's candidate distance to be dist + L,
 * sets v's parent to be curr and enqueue v into the priority queue with priority dist + L.
 * Otherwise, if v is yellow and the candidate distance to v is greater than dist + L:
 * sets v's candidate distance to be dist + L, sets v's parent to be curr and updates v's 
 * priority in the priority queue to dist + L
 */
void addNeighbours(Grid<double>& world, Loc curr, Loc end, TrailblazerPQueue<Loc>& toVisit, Grid<node>& nodes,
					double costFn(Loc from, Loc to, Grid<double>& world),
					double heuristic(Loc start, Loc end, Grid<double>& world)) {
	
	// For each node v connected to curr by an edge of length 1
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (world.inBounds(curr.row + i, curr.col + j)) { 
				Loc v = makeLoc(curr.row + i, curr.col + j); 
				if (nodes[curr.row + i][curr.col + j].color == GRAY) { // if v is gray
					colorCell(world, v, YELLOW);
					nodes[v.row][v.col].color = YELLOW;  // update color of node in grid
					nodes[v.row][v.col].cost = nodes[curr.row][curr.col].cost + costFn(curr, v, world);
					nodes[v.row][v.col].parent = curr; // set v's parent to be curr
					toVisit.enqueue(v, nodes[v.row][v.col].cost + heuristic(v, end, world)); 
				} 
				if (nodes[v.row][v.col].color == YELLOW) { // if v is yellow
					double newPriority = nodes[curr.row][curr.col].cost + costFn(curr, v, world);
					double prevPriority = nodes[v.row][v.col].cost;
					if (prevPriority > newPriority) { // if priority in queue is larger than new priority
						nodes[v.row][v.col].parent = curr;
						nodes[v.row][v.col].cost = newPriority;
						toVisit.decreaseKey(v, nodes[v.row][v.col].cost + heuristic(v, end, world));
					}
				}
			}
		}
	}
}


/* Function: shortestPath
 * 
 * Finds the shortest path between the locations given by start and end in the
 * specified world.	 The cost of moving from one edge to the next is specified
 * by the given cost function.	The resulting path is then returned as a
 * Vector<Loc> containing the locations to visit in the order in which they
 * would be visited.	If no path is found, this function should report an
 * error.
 *
 * In Part Two of this assignment, you will need to add an additional parameter
 * to this function that represents the heuristic to use while performing the
 * search.  Make sure to update both this implementation prototype and the
 * function prototype in Trailblazer.h.
 */
Vector<Loc>
shortestPath(Loc start,
             Loc end,
             Grid<double>& world,
             double costFn(Loc from, Loc to, Grid<double>& world),
			 double heuristic(Loc start, Loc end, Grid<double>& world)) {
	
	Vector<Loc> path;
	TrailblazerPQueue<Loc> toVisit;
	Grid<node> nodes(world.numRows(), world.numCols());  // keeps track of each cell's parent, color and cost
	
	// color all nodes grey
	for (int i = 0; i < nodes.numRows(); i++) {
		for (int j = 0; j < nodes.numCols(); j++) {
			nodes[i][j].color = GRAY;
		}
	}
	
	nodes[start.row][start.col].cost = 0;
	nodes[start.row][start.col].color = YELLOW;
	toVisit.enqueue(start, nodes[start.row][start.col].cost + heuristic(start, end, world)); // enqueue startNode
	colorCell(world, start, YELLOW); // colors startNode yellow

	while (!toVisit.isEmpty()) {
		Loc curr = toVisit.dequeueMin();
		colorCell(world, curr, GREEN);
		nodes[curr.row][curr.col].color = GREEN;  // updates info about Loc in grid
		
		if (curr == end) {
			makePath(path, end, nodes, start);
			return path;
		}

		addNeighbours(world, curr, end, toVisit, nodes, costFn, heuristic);
	}

	// error("shortestPath is not implemented yet.");
}


/* Inserts all edges in the graph into a priority queue
 */
void generateEdges(TrailblazerPQueue<Edge>& edges, int numRows, int numCols) {
	for (int i = 0; i < numRows; i++) {
		for (int j = 0; j < numCols; j++) {
			Loc startLoc = makeLoc(i, j);
			if (i + 1 < numRows) {
				Loc endLoc = makeLoc(i+1, j);
				Edge newEdge = makeEdge(startLoc, endLoc);
				int priority = randomInteger(1, 10);
				edges.enqueue(newEdge, priority);
			}
			if (j + 1 < numCols) {
				Loc endLoc = makeLoc(i, j+1);
				Edge newEdge = makeEdge(startLoc, endLoc);
				int priority = randomInteger(1, 10);
				edges.enqueue(newEdge, priority);
			}
		}	
	}
}

/* Finds which cluster Loc belongs to (dsu algorithm) 
 */
Loc findParent(Loc vert, Grid<Loc>& parentGrid){
	if (parentGrid[vert.row][vert.col] == vert) return vert;
    return parentGrid[vert.row][vert.col] = findParent(parentGrid[vert.row][vert.col], parentGrid); 
}


/* Sets up parentGrid in a way that every node has different parent 
 * (value of each cell is parent Loc), so in every cell is written the cluster it belongs to.
 */
void makeClusters(Grid<Loc>& parentGrid) {
	for (int i = 0; i < parentGrid.numRows(); i++) {
		for (int j = 0; j < parentGrid.numCols(); j++) {
			Loc loc;
			loc.row = i;
			loc.col = j;
			parentGrid[i][j] = loc;
		}
	}
}

/* Function: createMaze
 * 
 * Creates a maze of the specified dimensions using a randomized version of
 * Kruskal's algorithm, then returns a set of all of the edges in the maze.
 *
 * As specified in the assignment handout, the edges you should return here
 * represent the connections between locations in the graph that are passable.
 * Our provided starter code will then use these edges to build up a Grid
 * representation of the maze.
 */
Set<Edge> createMaze(int numRows, int numCols) {
	Set<Edge> finalEdges;
	
	TrailblazerPQueue<Edge> edges;
	generateEdges(edges, numRows, numCols);
	
	Grid<Loc> parentGrid(numRows, numCols);
	makeClusters(parentGrid);
	
	while (!edges.isEmpty()) {
		Edge e = edges.dequeueMin();
    
		Loc parent1 = findParent(e.start, parentGrid);
        Loc parent2 = findParent(e.end, parentGrid);

        if (parent1 != parent2) { // checks if endpoints of e are in the same cluster
            parentGrid[parent2.row][parent2.col] = parent1;  // merges clusters
            finalEdges.add(e);  // adds e to the resulting spannig tree
		}
	}

	return finalEdges;

	// error("createMaze is not implemented yet.");   
}